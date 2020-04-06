// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>
#include <thread>
#include "../seriazation_and_messages/serial.h"
#include "../seriazation_and_messages/message.h"
#include "network-helper.h"

/**
 * Function for creating a new poll file descriptor struct with a given fd number and array index.
 * @param fd_number The poll fd number.
 * @param index The index that this new struct should go in the pfds_ list.
 * @param pfds List of pollfds to be added to.
 * @param pfds_list List of pollfd pointers needed to delete the structs, also needs to hold the new struct.
 */
void create_new_pollfd(int fd_number, int index, struct pollfd *pfds,
                       struct pollfd **pfds_list) {

    // Create the struct
    struct pollfd *new_fd = new struct pollfd;
    // Set the fd to the given fd number.
    new_fd->fd = fd_number;
    // Set the events to POLLIN so it can be polled.
    new_fd->events = POLLIN;
    // Put the struct in the array of structs.
    pfds[index] = *new_fd;
    pfds_list[index] = new_fd;
}

/**
 * Server object that encapsulates server/socket functionality.
 */
class Server {
public:

    int server_fd_; // server file descriptor
    struct sockaddr_in address_; // server struct
    int addrlen_; // length of the server address
    int opt_; // server opt number

    // Main constructor that takes in an ip and port.
    Server(char *ip, int port) {

        // Set length as the size of the server struct.
        // Also set the opt to be default 1.
        addrlen_ = sizeof(address_);
        opt_ = 1;
        // Set the file descriptor
        if ((server_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        // Set the sock opt.
        if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt_, sizeof(opt_))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        // Set the ip address and port fields for the server struct.
        address_.sin_family = AF_INET;
        address_.sin_addr.s_addr = inet_addr(ip);
        address_.sin_port = htons(port);

    }

    /**
     * Binds the server to this file descriptor
     */
    void socket_bind() {
        if (bind(server_fd_, (struct sockaddr *) &address_,
                 sizeof(address_)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Has server start listening and returns the file descriptor to
     * be the listener int value that is used to tell where messages are coming from.
     * @return
     */
    int socket_listen() {
        if (listen(server_fd_, 10) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        return server_fd_;
    }

    /**
     * Accept an incoming connection, returns int for the socket number.
     * @return Socket number or -1 if error accepting.
     */
    int socket_accept() {
        return accept(server_fd_, (struct sockaddr *) &address_,
                      (socklen_t * ) & addrlen_);
    }

    /**
     * Read from a socket that is already connected.
     * @param sock The socket to read.
     * @param buffer Buffer to hold the incoming message.
     * @param buffer_size Size of the buffer.
     * @return Int that is positive for success and 0 if error reading.
     */
    int socket_read(int sock, char *buffer, int buffer_size) {
        return read(sock, buffer, buffer_size);
    }

    /**
     * Send a message to the given socket.
     * @param sock Socket to send to.
     * @param message Message to send.
     */
    void socket_send(int sock, char *message, size_t size) {
        send(sock, message, size, 0);
    }

};

/**
 * Client object that connects to the known server.
 */
class Client {
public:

    struct sockaddr_in client_addr_; // client struct.
    int server_addrlen_; // server address struct length.
    int sock_; // socket this client is on

    /**
     *  Main constructor that takes in an ip that this client is running on and a port.
     *  Also has the servers ip to connect to.
     */
    Client(char *own_ip, int port, char *server_ip) {

        // Set sock to default and the server_length.
        sock_ = 0;
        server_addrlen_ = sizeof(client_addr_);

        // Setting up socket
        if ((sock_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(EXIT_FAILURE);
        }

        // Setting the client struct
        client_addr_.sin_family = AF_INET;
        client_addr_.sin_port = htons(port);

        // check if server address is available to connect to.
        if (inet_pton(AF_INET, server_ip, &client_addr_.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported \n");
            exit(EXIT_FAILURE);
        }

        // Connect to the server address.
        if (connect(sock_, (struct sockaddr *) &client_addr_, sizeof(client_addr_)) < 0) {
            printf("Connection failed\n");
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Read from a the server.
     * @param buffer Buffer to hold the incoming message.
     * @param buffer_size Size of the buffer.
     * @return Int that is positive for success and 0 if error reading.
     */
    int socket_read(char *buffer, int buffer_size) {
        return read(sock_, buffer, buffer_size);
    }

    int socket_recv(char *buffer, int buffer_size) {
        return recv(sock_, buffer, buffer_size, MSG_DONTWAIT);
    }

    /**
     * Send a message to the server.
     * @param message Message to send.
     */
    void socket_send(char *message, size_t size) {
        send(sock_, message, size, 0);
    }

};

//============================================================================

/**
 * Rendezvous Server object that acts as the registrar server.
 * When the server dies the clients will no longer be able to read from it and
 * so they shut down.
 */
class RendezvousServer : public Object {
public:

    Server *internalServer_; // The server acting as the registrar server.
    int max_clients_; // The maximum clients that can join the server.
    struct pollfd *pfds_; // Structs used for polling messages on the server.
    struct pollfd **pfds_list_; // List of pointers to pfds created since the
    // main pfds_ needs to be a list of structs instead of pointers.
    // Need this to delete the created structs.
    bool listener_added_; // boolean to keep track of if run happened to make
    // sure the listener pfd is only deleted if it was created.
    int *sockets_; // List of socket values for every client that has connected.

    // Directory Values
    String **ip_list_; // List of ip addresses for clients that have joined.
    size_t *port_list_; // List of ports for clients that have joined.
    size_t *nodes_list_; // List of node values for clients that have joined;
    int total_socket_count_; // Count of clients that have connected on a socket.

    /**
     * Main constructor for the Rendezvous Server
     * @param ip The ip that this server will be run on.
     * @param port The port that this server will be run on.
     * @param max_clients The maximum number of clients that can connect to this server.
     */
    RendezvousServer(char *ip, int port, int max_clients) {
        // Create a server to be held in this class and bind it.
        internalServer_ = new Server(ip, port);
        internalServer_->socket_bind();
        // Set the max clients up along with the poll file descriptor list,
        // the list of ips of clients, the socket number list, and the total socket count
        // should be 0 because there are no clients connected yet.
        max_clients_ = max_clients;
        pfds_ = new struct pollfd[max_clients_ + 1];
        pfds_list_ = new struct pollfd *[max_clients_ + 1];
        ip_list_ = new String *[max_clients_];
        port_list_ = new size_t[max_clients_];
        nodes_list_ = new size_t[max_clients_];
        sockets_ = new int[max_clients_];
        total_socket_count_ = 0;
    }

    /**
     * Destructor for the RS, deletes the internal server and the three lists held inside.
     */
    ~RendezvousServer() {
        delete internalServer_;
        delete[] pfds_;
        for (int i = 0; i < total_socket_count_; i++) {
            delete ip_list_[i];
            delete pfds_list_[i + 1];
        }
        if (listener_added_) {
            delete pfds_list_[0];
        }
        delete[] pfds_list_;
        delete[] ip_list_;
        delete[] sockets_;
        delete[] port_list_;
        delete[] nodes_list_;
    }


    /**
     * Function for handling getting a message on the listener socket.
     */
    void message_on_listener() {

        // Hold a buffer for incoming messages.
        char buffer[1024] = {0};

        // Accept the new client that is sending a message to the listener.
        accept_new_client(buffer);

        // Create a new pollfd for the socket that client connected on.
        // Pass in total_socket_count + 1 because the + 1 accounts for the listener also being in
        // the pfds list.
        create_new_pollfd(sockets_[total_socket_count_], total_socket_count_ + 1, pfds_, pfds_list_);

        // Increase the socket count.
        total_socket_count_++;

        // Reset buffer so that new messages don't overlap.
        memset(buffer, 0, 1024);
    }

    /**
    * Handles a client joining the network
    * @param s Server object.
    * @param sockets List of sockets.
    * @param total_socket_count Number of sockets not the listener.
    * @param fd_count Count of all sockets including the listener.
    * @param buffer A string buffer that is being passed around for handling most messages.
    * @param ip_list List of ips that have connected to the server.
    */
    void accept_new_client(char *buffer) {
        // Accept the socket or error if bad acception which is a return of -1
        if ((sockets_[total_socket_count_] = internalServer_->socket_accept()) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read in the message that should be the ip of the client.
        internalServer_->socket_read(sockets_[total_socket_count_], buffer, 1024);

        // Create a new string for this ip and store it.
        char *serial_string = new char[1024];
        strcpy(serial_string, buffer);

        // Create a serializer to then deserialize into a Register.
        Serializer *ser = new Serializer(reinterpret_cast<unsigned char *>(serial_string));
        Register *reg = dynamic_cast<Register *>(ser->deserialize());

        // Print the ip of the newly accepted client
        printf("Client accepted : %s\n", reg->client_ip_->c_str());

        // Add register to lists for client information.
        ip_list_[total_socket_count_] = reg->client_ip_->clone();
        port_list_[total_socket_count_] = reg->port_;
        nodes_list_[total_socket_count_] = reg->sender_;

        delete ser;
        delete reg;

        // Send the register to all clients besides the new one.
        for (int i = 0; i < total_socket_count_; i++) {
            internalServer_->socket_send(sockets_[i], buffer, 1024);
        }

        Directory *dir = new Directory(3, 10, 9999, 3, total_socket_count_, port_list_,
                                       total_socket_count_, ip_list_, total_socket_count_, nodes_list_);
        Serializer *directorySer = new Serializer();
        char *serialized_directory = directorySer->serialize(dir);

        internalServer_->socket_send(sockets_[total_socket_count_], serialized_directory, 1024);

        delete dir;
        delete directorySer;
    }

    /**
     * Function for running the rendezvous server.
     */
    void run(char *kill_switch) {

        // Begin listening.
        int listener = internalServer_->socket_listen();

        // Create the pollfd struct for the listener socket.
        create_new_pollfd(listener, 0, pfds_, pfds_list_);
        listener_added_ = true;

        // Loop on true to continuously run the server.
        while (kill_switch[0] != '1') {

            // Poll the server activity.
            int poll_count = poll(pfds_, total_socket_count_ + 1, 0);
            if (poll_count == -1) {
                kill_switch[0] = '1';
            }
            // Run through the existing connections looking for data to read.
            for (int i = 0; i < total_socket_count_ + 1; i++) {
                // Check if a port is ready to be read from
                if (pfds_[i].revents & POLLIN) {
                    // If the server listener is ready to read from then handle the new
                    // message on this listener socket.
                    if (pfds_[i].fd == listener) {
                        message_on_listener();
                    } else {
                        // Else if a message is not on the listener than this is unexpected as of now.
                        printf("%s\n", "Received unexpected message from already connected client");
                    }
                }
            }
        }
    }
};

//===========================================================================================

/**
 * Node class
 */
class Node : public Object {
public:

    int max_clients_;// The maximum clients that can join the server.

    Client *internalClient_; // Internal client for hitting the rendezvous server.
    char *client_ip_; // Ip of this node.
    int port_; // Port for this node.
    int IC_other_total_client_count_; // Total other clients known to the network.
    char **IC_ip_list_; // List of the total client ips known to the network.
    size_t *IC_port_list_; // List of total ports for ips known to the network.
    size_t *IC_nodes_list_; // List of total nodes for nodes known to the network.
    Client **IC_other_client_connections_; // Client objects for connecting to other nodes.
    int IC_other_client_connections_count_; // Count of client objects for connecting to other nodes.
    char **IC_other_client_connections_ips_;

    // Member variables for the internal server.
    Server *internalServer_; // The internal server object for receiving messages from other clients.
    struct pollfd *IS_pfds_; // Structs used for polling messages on the server.
    struct pollfd **IS_pfds_list_; // List of pointers to pfds created since the
    // main pfds_ needs to be a list of structs instead of pointers.
    // Need this to delete the created structs.
    bool IS_listener_added_; // boolean to keep track of if run happened to make
    // sure the listener pfd is only deleted if it was created.
    char **IS_ip_list_; // List of ip addresses for clients that have joined.
    int *IS_sockets_; // List of socket values for every client that has connected.
    int IS_total_socket_count_; // Count of clients that have connected on a socket.

    Map *store_map_; // store map from application.
    size_t node_; // node number for the application


    /**
     * Main Node Constructor
     * @param client_ip The ip for this node.
     * @param port The port for this node.
     * @param server_ip The ip of the rendezvous server.
     * @param max_clients The maximum number of clients that can be added.
     */
    Node(char *client_ip, int port, char *server_ip, int max_clients, Map *map, size_t node) {

        max_clients_ = max_clients;

        // Internal client config
        char *client_ip_copy = new char[strlen(client_ip) + 1];
        strcpy(client_ip_copy, client_ip);
        client_ip_ = client_ip_copy;
        port_ = port;
        IC_other_total_client_count_ = 0;
        internalClient_ = new Client(client_ip_, port_, server_ip);
        IC_other_client_connections_ = new Client *[max_clients_];
        IC_other_client_connections_ips_ = new char *[max_clients_];
        IC_other_client_connections_count_ = 0;
        IC_ip_list_ = new char *[max_clients_];
        IC_port_list_ = new size_t[max_clients_];
        IC_nodes_list_ = new size_t[max_clients_];

        // Internal Server config
        internalServer_ = new Server(client_ip_, port_);
        internalServer_->socket_bind();
        IS_pfds_ = new struct pollfd[max_clients_ + 1];
        IS_pfds_list_ = new struct pollfd *[max_clients_ + 1];
        IS_sockets_ = new int[max_clients_];
        IS_ip_list_ = new char *[max_clients_];

        IS_total_socket_count_ = 0;

        store_map_ = map;
        node_ = node;
    }

    /**
     * Destructor for the Node
     */
    ~Node() {

        // Delete Internal Client members
        delete internalClient_;
        delete[] client_ip_;
        for (int i = 0; i < IC_other_client_connections_count_; i++) {
            delete IC_other_client_connections_[i];
        }
        delete[] IC_other_client_connections_;
        for (int i = 0; i < IC_other_total_client_count_; i++) {
            delete[] IC_ip_list_[i];
        }
        delete[] IC_ip_list_;
        delete[] IC_port_list_;
        delete[] IC_nodes_list_;

        // Delete Internal Server members
        delete internalServer_;
        delete[] IS_pfds_;
        delete[] IS_sockets_;
        for (int i = 0; i < IS_total_socket_count_; i++) {
            delete[] IS_ip_list_[i];
            // Other clients will be at index i + 1 because the first index is the listener.
            delete IS_pfds_list_[i + 1];
        }
        if (IS_listener_added_) {
            // Delete the listener if one was added.
            delete IS_pfds_list_[0];
        }
        delete[] IS_ip_list_;
        delete[] IS_pfds_list_;
    }

    /**
     * Function for running the node implementation.
     * @param kill_switch Int for being able to kill the program gracefully.
     */
    void run(char *kill_switch) {

        // Start two threads, one for the internal server, and one for the
        // rendezvous server connection.
        std::thread internal_server_thread(&Node::run_internal_server, this,
                                           kill_switch);
        std::thread rendezvous_server_thread(&Node::run_rendezvous_communication,
                                             this, kill_switch);

        // Join the threads when the program is done running.
        internal_server_thread.join();
        rendezvous_server_thread.join();
    }

    //--------------------------------------------------------------------------

    // Put Handling Methods

    /**
     * Function for handling a put request coming in.
     * @param socket_idx index of the socket to interact with
     * @param target target node id.
     */
    void handle_put(int socket_idx, size_t target) {

        // Create a buffer for reading and writing messages.
        char buffer[1024] = {0};

        // Send an ack message to acknowledge that the put was received.
        Serializer *ackSer = get_ack_serializer(node_, target, 0);
        char *serialized_ack = ackSer->buffer_;
        strcpy(buffer, serialized_ack);
        internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);

        // Reset the buffer to then read in the next message.
        memset(buffer, 0, 1024);
        internalServer_->socket_read(IS_sockets_[socket_idx], buffer, 1024);
        Object *key = deserialize_buffer(buffer);

        // Check if next message is a status.
        if (dynamic_cast<Status *>(key) != nullptr) {
            Status *stat = dynamic_cast<Status *>(key);
            // Continue to read in the dataframe.
            handle_put_receive_dataframe(socket_idx, target, stat->msg_->c_str(), stat->id_);
        } else {
            // Not a status then error out something went wrong.
            exit(1);
        }

        // Delete created object.s
        delete ackSer;
        delete key;
    }

    /**
     * Function for handling needing to receive a DataFrame for a put.
     * @param socket_idx index of socket to interact with.
     * @param target target node id.
     * @param key_name name of the key needed for the put.
     * @param df_length
     */
    void handle_put_receive_dataframe(int socket_idx, size_t target, char *key_name, size_t df_length) {

        // Buffer for reading in message,
        char *buffer = new char[1024];

        // Boolean for keeping track of whether more columns of the receiving dataframe need to be read.
        bool end_reached = false;

        // Create new dataframe
        Schema s("");
        DataFrame *df = new DataFrame(s);

        // While the end of the dataframe messages are not reached.
        while (!end_reached) {

            // Read in message
            memset(buffer, 0, 1024);
            internalServer_->socket_read(IS_sockets_[socket_idx], buffer, 1024);

            // Deserialized message should be a Message. The kind is then what is important.
            Message *deserialized_mes = dynamic_cast<Message *>(deserialize_buffer(buffer));

            // If put message then a column needs to be read
            if (deserialized_mes->kind_ == MsgKind::Put) {
                handle_put_receive_column(target, socket_idx, deserialized_mes->id_, df);
                end_reached = true;
            } else {
                end_reached = true;
            }

            // Create a key and add it with the completed dataframe to the store.
            Key *new_key = new Key(key_name, node_);
            store_map_->put(new_key, df);

            std::cout << "Finished put" << std::endl;

            delete deserialized_mes;
        }
    }

    /**
     * Handle receiving a single column of a DataFrame.
     * @param target Target node id.
     * @param socket_idx Index of socket that is being interacted with.
     * @param size_of_column_message Size of column char* that needs to be received.
     * @param df DataFrame to add column to.
     */
    void handle_put_receive_column(size_t target, size_t socket_idx, size_t size_of_column_message, DataFrame *df) {

        // Buffer for sending ack to start sequence.
        char buffer[1024] = {0};

        // Create a buffer for receiving the column based on the info
        // from the received put message. Need size of at least 1024 to read in successfully.
        size_t size_of_message = size_of_column_message;
        if (size_of_message < 1024) {
            size_of_message = 1024;
        }
        char *new_buffer = new char[size_of_message + 1];
        memset(new_buffer, 0, size_of_message + 1);

        // Create and send an ack message to then initiate the start of receiving the column.
        Serializer *ack_ser = get_ack_serializer(node_, target, 0);
        strcpy(buffer, ack_ser->buffer_);
        internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);

        // Keep a count of how many reads have been done for this column for use in helper function.
        size_t recv_count = 0;
        // Keep track of if the while loop should continue.
        bool message_unfinished = true;

        while (message_unfinished) {
            // Read in the column message, if not a part of the column then that column is over.
            message_unfinished = read_in_put_column(new_buffer, socket_idx, recv_count);
            recv_count++;
        }

        // Create a serializer for the column and then add the deserialized column to the dataframe.
        Serializer *col_ser = new Serializer(reinterpret_cast<unsigned char *>(new_buffer));
        df->add_column(dynamic_cast<Column *>(col_ser->deserialize()));

        delete col_ser;
        delete ack_ser;
    }

    /**
     * Fucction for reading in a part of a column during a put.
     * @param append_buffer Buffer to append incoming messages to to build DataFrame.
     * @param socket_idx Index of socket to interact with.
     * @param recv_count Count of read iterations.
     * @return Returns a bool, true if continue reading, false to stop.
     */
    bool read_in_put_column(char *append_buffer, size_t socket_idx, size_t recv_count) {

        // Create a buffer to read in the message.
        char buffer[1024] = {0};
        internalServer_->socket_read(IS_sockets_[socket_idx], buffer, 1024);

        // If the recv_count > 0 then check if the end column full message is sent.
        if (recv_count > 0) {
            Object *deserialized_mes = deserialize_buffer(buffer);
            // If deserialize works then the message received was the end column message.
            if (deserialized_mes != nullptr) {
                delete deserialized_mes;
                // return false to stop the loop.
                return false;
            }
            delete deserialized_mes;
        }

        // If first recv then copy, else concat onto end of the buffer used to form the whole column.
        if (recv == 0) {
            strncpy(append_buffer, buffer, 1024);
        } else {
            strncat(append_buffer, buffer, 1024);
        }

        // return true to continue loop.
        return true;
    }


    /**
     *  Handle a wait and get message being received.
     * @param socket_idx Index of socket to iteract with.
     * @param target Id of target node.
     */
    void handle_wait_and_get(int socket_idx, size_t target) {

        // Create a buffer for sending and reading messages.
        char buffer[1024] = {0};

        // Create and send ack to acknowledge that the handle and wait has been received.
        Serializer *ack_ser = get_ack_serializer(node_, target, 0);
        strcpy(buffer, ack_ser->buffer_);
        internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);
        delete ack_ser;

        // Reset the buffer to then receive what is hopefully a status message
        // with information on the key that is being requested.
        memset(buffer, 0, 1024);
        internalServer_->socket_read(IS_sockets_[socket_idx], buffer, 1024);
        Object *key = deserialize_buffer(buffer);

        // If status received then it is the message we want with the information of the key.
        if (dynamic_cast<Status *>(key) != nullptr) {
            // Continue to then handle sending the DataFrame.
            handle_wait_and_get_dataframe_send(socket_idx, target, dynamic_cast<Status *>(key)->msg_->c_str());
            delete key;
        } else {
            // Error if anything else received here. Should not happen.
            delete key;
            exit(1);
        }
    }


    /**
     * Function for handling sending a DataFrame when a wait and get occurs.
     * @param socket_idx Index of socket to interact with.
     * @param target Id of target node.
     * @param key_name Name of key of dataframe.
     */
    void handle_wait_and_get_dataframe_send(int socket_idx, size_t target, char *key_name) {

        char buffer[1024] = {0};

        Key *key = new Key(key_name, node_);

        DataFrame *desired_df = nullptr;

        while (desired_df == nullptr) {

            if (store_map_->contains_key(key)) {
                Object *ob = store_map_->get(key);
                desired_df = dynamic_cast<DataFrame *>(ob);

                //TODO Need to handle telling it how many columns
                // Currently works because only one

                for (int i = 0; i < desired_df->ncols(); i++) {
                    Column *cur_col = desired_df->cols_[i];
                    handle_wait_and_get_send_column(cur_col, target, socket_idx);
                }
            }
        }
    }

    /**
     * Function for handling sending one column for an wait and get interaction.
     * @param cur_col Column to send.
     * @param target Target nodes id.
     * @param socket_idx Index of socket to interact with.
     */
    void handle_wait_and_get_send_column(Column *cur_col, size_t target, size_t socket_idx) {

        // Buffer for sending and receiving messages.
        char buffer[1024] = {0};

        // Create serialized column.
        Serializer *col_ser = new Serializer();
        char *col_string = col_ser->serialize(cur_col);

        // Create Message with id of the length of the column message so that the
        // other node knows how big of a message it will be receiving in chunks.
        Serializer *strlen_ser = get_message_serializer(MsgKind::WaitAndGet, node_, target, strlen(col_string));
        strcpy(buffer, strlen_ser->buffer_);
        internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);

        // TODO Maybe check that this is an ack.
        memset(buffer, 0, 1024);
        internalServer_->socket_read(IS_sockets_[socket_idx], buffer, 1024);

        // Keep track of count of message that has been sent to ensure
        // the entire column is sent.
        size_t count = 0;

        // Send the column in chunks and increment the count to keep track of how much has been sent.
        while (count < strlen(col_string)) {
            memset(buffer, 0, 1024);
            strncpy(buffer, col_string + count, 1023);
            internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);
            count += 1023;
        }

        // Send the same wait and get message from earlier again to show that the column sending is done.
        memset(buffer, 0, 1024);
        strcpy(buffer, strlen_ser->buffer_);
        internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);

        delete col_ser;
        delete strlen_ser;
    }


    /**
     * Handles a client joining the network
     * @param buffer A string buffer that is being passed around for handling most messages.
     */
    void accept_new_client(char *buffer) {
        // Accept the socket or error if bad acception which is a return of -1
        if ((IS_sockets_[IS_total_socket_count_] = internalServer_->socket_accept()) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int current_sock_idx = IS_total_socket_count_;

        // Read in the message that should be the ip of the client.
        internalServer_->socket_read(IS_sockets_[current_sock_idx], buffer, 1024);

        // Deserialize the string.
        char *serial_string = new char[1024];
        strcpy(serial_string, buffer);

        Serializer *ser = new Serializer(reinterpret_cast<unsigned char *>(serial_string));
        Object *deserialized = ser->deserialize();

        if (dynamic_cast<Message *>(deserialized) != nullptr) {
            Message *mes = dynamic_cast<Message *>(deserialized);
            if (mes->kind_ == MsgKind::WaitAndGet) {
                handle_wait_and_get(current_sock_idx, mes->sender_);
            } else if (mes->kind_ == MsgKind::Put) {
                handle_put(current_sock_idx, mes->sender_);
            }
        }

        delete deserialized;
        delete ser;
    }

    /**
     * Function for handling getting a message on the listener socket.
     */
    void message_on_listener() {

        // Hold a buffer for incoming messages.
        char buffer[1024] = {0};

        // Accept the new client that is sending a message to the listener.
        accept_new_client(buffer);

        // Create a new pollfd for the socket that client connected on.
        // Pass in total_socket_count + 1 because the + 1 accounts for the listener also being in
        // the pfds list.
        create_new_pollfd(IS_sockets_[IS_total_socket_count_], IS_total_socket_count_ + 1,
                          IS_pfds_, IS_pfds_list_);

        // Increase the socket count.
        IS_total_socket_count_++;

    }


    /**
     * Function for running the internal server implementation so that this node can
     * receive messages from other nodes.
     * @param kill_switch
     */
    void run_internal_server(char *kill_switch) {

        // Begin listening.
        int listener = internalServer_->socket_listen();

        // Create the listener pollfd struct.
        create_new_pollfd(listener, 0, IS_pfds_, IS_pfds_list_);
        IS_listener_added_ = true;

        // Loop on true to continuously run the server
        while (kill_switch[0] != '1') {

            // Poll the server activity.
            int poll_count = poll(IS_pfds_, IS_total_socket_count_ + 1, 0);
            if (poll_count == -1) {
                perror("poll");
                kill_switch[0] = '1';
                return;
            }
            // Run through the existing connections looking for data to read
            for (int i = 0; i < IS_total_socket_count_ + 1; i++) {
                // Check if a port is ready to be read from
                if (IS_pfds_[i].revents & POLLIN) {
                    // If the server listener is ready to read from then
                    // accept the new connection and handle adding the new socket.
                    if (IS_pfds_[i].fd == listener) {
                        // Call function for handling a message on the listener of a client
                        // trying to connect.
                        message_on_listener();
                    } else {
                        // Else if a message is not on the listener than this is unexpected as of now.
                        printf("%s\n", "Received unexpected message from already connected other client");
                    }
                }
            }
        }
    }

    //------------------------------------------------------------------
    // Functions for communication with registrar.

    /**
     * Function for sending a register message.
     */
    void send_register() {

        // Create a buffer to copy to for sending the message.
        char buffer[1024] = {0};

        // Create register serializer object.
        Serializer *registerSer = get_register_serializer(node_, 3, 0, client_ip_, port_);

        // Copy the register message to the buffer and sent it.
        strcpy(buffer, registerSer->buffer_);
        internalClient_->socket_send(buffer, 1024);

        delete registerSer;
    }

    /**
     * Take in a register as a node and add it to all of the lists needed for
     * knowledge of other nodes.
     * @param reg Register to add.
     */
    void add_register(Register *reg) {

        IC_ip_list_[IC_other_total_client_count_] = duplicate(reg->client_ip_->c_str());
        IC_port_list_[IC_other_total_client_count_] = reg->port_;
        IC_nodes_list_[IC_other_total_client_count_] = reg->sender_;
        IC_other_total_client_count_++;

        // Print the ip of the newly accepted client
        printf("New Node joined : %s\n", reg->client_ip_->c_str());
    }

    /**
     * Take in a directory and add each register from it one at a time.
     * @param dir Directory to add.
     */
    void add_directory(Directory *dir) {

        // Loop through directory and build registers to add.
        for (int i = 0; i < dir->addresses_count_; i++) {
            Register *temp = new Register(dir->nodes_[i], 0, 0, dir->addresses_[i]->c_str(), dir->ports_[i]);
            add_register(temp);
            delete temp;
        }

        printf("Directory of size %d received \n", dir->ports_count_);
    }

    /**
     * Function for running the implementation for the node communicating with the
     * Rendezvous Server.
     * @param kill_switch Int value for knowing when to gracefully kill the program.
     */
    void run_rendezvous_communication(char *kill_switch) {
        // Create buffer for incoming and outgoing messages.
        char buffer[1024] = {0};

        // Start by sending a register message
        send_register();

        // Loop continuously while connected to the server.
        while (kill_switch[0] != '1') {

            // Reset buffer in the while loop so new messages dont have parts of old ones in them.
            memset(buffer, 0, 1024);

            // Read from server, if no response than close because that means the server went down.
            if (internalClient_->socket_recv(buffer, 1024) == 0) {
                // End when the server stops running.
                kill_switch[0] = '1';
                return;
            }

            // Deserialize the buffer
            Object *deserialized = deserialize_buffer(buffer);

            // If a Register is the incoming message then add the new nodes data.
            if (dynamic_cast<Register *>(deserialized) != nullptr) {
                Register *reg = dynamic_cast<Register *>(deserialized);
                add_register(reg);
            }
                // Else if a directory is the incoming message then copy all of the data
                // to the values of this node.
            else if (dynamic_cast<Directory *>(deserialized) != nullptr) {
                Directory *dir = dynamic_cast<Directory *>(deserialized);
                add_directory(dir);
            }
            delete deserialized;
        }
    }

    //---------------------------------------------------------------
    // Application Specific Functions

    /**
     * Function for getting a client object to interact with
     * @param k
     * @return Returns client or nullptr if the other node is not known to this one yet.
     */
    Client *get_client_of_key(Key k) {
        // Set client to nullptr to start.
        Client *client = nullptr;

        // Look through known client for the node id wanted.
        size_t node_idx = max_clients_;
        for (int i = 0; i < IC_other_total_client_count_; i++) {
            if (IC_nodes_list_[i] == k.idx_) {
                // if node id found then set the node idx variable.
                node_idx = i;
                break;
            }
        }

        // If node idx was changed
        if (node_idx != max_clients_) {
            // Looking for client with the ip at the index found.
            char *target_ip = IC_ip_list_[node_idx];
            Client *cur_client;
            for (int i = 0; i < IC_other_client_connections_count_; i++) {
                if (strcmp(target_ip, IC_other_client_connections_ips_[i])) {
                    // If ip is found then this i is where the client already made is.
                    client = IC_other_client_connections_[i];
                }
            }

            // If no client for the ip has been made yet then create one.
            if (client == nullptr) {
                client = new Client(client_ip_, IC_port_list_[node_idx], IC_ip_list_[node_idx]);

                IC_other_client_connections_[IC_other_client_connections_count_] = cur_client;
                IC_other_client_connections_ips_[IC_other_client_connections_count_] = IC_ip_list_[node_idx];
                IC_other_client_connections_count_++;
            }
        }

        // Return the client
        return client;
    }

    /**
     * Handle put functionality.
     * @param k Key of the DataFrame to put.
     * @param df The actual DataFrame to put.
     */
    void put(Key k, DataFrame *df) {

        char buffer[1024] = {0};

        bool message_sent = false;

        // Run a loop until the process has finished.
        while (!message_sent) {

            // Figure out which client object to use for interaction.
            Client *cur_client = get_client_of_key(k);

            if (cur_client != nullptr) {

                message_sent = true;

                // TODO can send df length here
                // Send the starting put message to start everything off.
                Serializer *put_ser = get_message_serializer(MsgKind::Put, node_, k.idx_, 0);
                strcpy(buffer, put_ser->buffer_);
                cur_client->socket_send(buffer, 1024);
                delete put_ser;

                // TODO Check if actually an ack.
                // Read in ack message.
                memset(buffer, 0, 1024);
                cur_client->socket_read(buffer, 1024);

                // Create a status message for sending the char identifier for the key.
                memset(buffer, 0, 1024);
                String *key_string = new String(k.name_);
                Serializer *key_ser = get_status_serializer(node_, k.idx_, k.idx_, key_string);
                strcpy(buffer, key_ser->buffer_);
                cur_client->socket_send(buffer, 1024);
                delete key_ser;

                memset(buffer, 0, 1024);

                // Loop through columns sending one at a time.
                for (int i = 0; i < df->ncols(); i++) {
                    Column *cur_col = df->cols_[i];
                    put_send_column(cur_client, cur_col, k.idx_);
                }
            }
        }
    }

    /**
     * Handle sending one column of a DataFrame for the put functionality.
     * @param cur_client Client to use for sending messages.
     * @param cur_col Column to send.
     * @param target Target node id.
     */
    void put_send_column(Client* cur_client, Column* cur_col, size_t target) {

        // TODO find a way to have this be the same function as handle send column
        // TODO because this is literally the same code just with different sending of messages.

        // Buffer for sending and receiving messages.
        char buffer[1024] = {0};

        // Create serialized column.
        Serializer *col_ser = new Serializer();
        char *col_string = col_ser->serialize(cur_col);

        // Create Message with id of the length of the column message so that the
        // other node knows how big of a message it will be receiving in chunks.
        Serializer *strlen_ser = get_message_serializer(MsgKind::Put, node_, target, strlen(col_string));
        strcpy(buffer, strlen_ser->buffer_);
        cur_client->socket_send( buffer, 1024);

        // TODO Maybe check that this is an ack.
        memset(buffer, 0, 1024);
        cur_client->socket_read( buffer, 1024);

        // Keep track of count of message that has been sent to ensure
        // the entire column is sent.
        size_t count = 0;

        // Send the column in chunks and increment the count to keep track of how much has been sent.
        while (count < strlen(col_string)) {
            memset(buffer, 0, 1024);
            strncpy(buffer, col_string + count, 1023);
            cur_client->socket_send( buffer, 1024);
            count += 1023;
        }

        // Send the same put message from earlier again to show that the column sending is done.
        memset(buffer, 0, 1024);
        strcpy(buffer, strlen_ser->buffer_);
        cur_client->socket_send( buffer, 1024);

        delete col_ser;
        delete strlen_ser;
    }


    DataFrame *waitAndGet(Key k) {

        char buffer[1024] = {0};

        bool message_sent = false;

        // Run a loop until the process has finished.
        while (!message_sent) {

            // Figure out which client object to use for interaction.
            Client *cur_client = get_client_of_key(k);

            if (cur_client != nullptr) {

                message_sent = true;

                // Send the starting wait and get message to start everything off.
                Serializer *put_ser = get_message_serializer(MsgKind::WaitAndGet, node_, k.idx_, 0);
                strcpy(buffer, put_ser->buffer_);
                cur_client->socket_send(buffer, 1024);
                delete put_ser;

                // TODO Check if actually an ack.
                // Read in ack message.
                memset(buffer, 0, 1024);
                cur_client->socket_read(buffer, 1024);

                // Create a status message for sending the char identifier for the key.
                memset(buffer, 0, 1024);
                String *key_string = new String(k.name_);
                Serializer *key_ser = get_status_serializer(node_, k.idx_, k.idx_, key_string);
                strcpy(buffer, key_ser->buffer_);
                cur_client->socket_send(buffer, 1024);
                delete key_ser;


                // Create DataFrame and boolean for going through receving columns.
                bool end_reached = false;
                Schema s("");
                DataFrame *df = new DataFrame(s);

                // TODO handle actually checking how big the dataframe is

                while (!end_reached) {
                    memset(buffer,
                           0, 1024);

                    cur_client->
                            socket_read(buffer,
                                        1024);

// Deserialize the string.
                    char *serial_string = new char[1024];
                    strcpy(serial_string, buffer
                    );

                    Serializer *ser = new Serializer(reinterpret_cast<unsigned char *>(serial_string));
                    Message *deserialized_mes = dynamic_cast<Message *>(ser->deserialize());
                    if (deserialized_mes->kind_ == MsgKind::WaitAndGet) {
                        size_t size_of_message = deserialized_mes->id_;

                        if (size_of_message < 1024) {
                            size_of_message = 1024;
                        }

                        Ack *ack = new Ack(node_, k.idx_, 0);

                        Serializer *ackSer = new Serializer();
                        char *serialized_ack = ackSer->serialize(ack);

// Copy the ack message to the buffer and sent it.
                        strcpy(buffer, serialized_ack
                        );

                        cur_client->
                                socket_send(buffer,
                                            1024);

                        delete
                                ack;
                        delete
                                ackSer;

                        char *new_buffer = new char[size_of_message + 1];

                        std::cout << size_of_message << " size " <<
                                  std::endl;


                        size_t count = 0;

                        size_t recv_count = 0;

                        bool message_unfinished = false;

                        while (!message_unfinished) {

                            std::cout << "Hello!!!!!!!!!!!!!" <<
                                      std::endl;


                            memset(buffer,
                                   0, 1024);
                            cur_client->
                                    socket_read(buffer,
                                                1024);

                            if (recv_count > 0) {
                                std::cout << buffer <<
                                          std::endl;
                                char *serial_string2 = new char[1024];
                                strcpy(serial_string2, buffer
                                );

                                Serializer *messsss = new Serializer(reinterpret_cast<unsigned char *>(serial_string2));
                                if (messsss->

                                        deserialize()

                                    != nullptr) {
                                    delete
                                            messsss;
                                    break;
                                }
                                delete
                                        messsss;
                            }

                            if (count == 0) {
                                strncpy(new_buffer, buffer,
                                        1024);
                            } else {
                                strncat(new_buffer, buffer,
                                        1024);
                            }
                            count += 1023;
                            recv_count++;
                        }

                        std::cout << new_buffer <<
                                  std::endl;

                        std::cout << "Hello@@@@@@" <<
                                  std::endl;

                        Serializer *col_ser = new Serializer(reinterpret_cast<unsigned char *>(new_buffer));

                        df->add_column(dynamic_cast<Column *>(col_ser->

                                deserialize()

                                       ));

                        delete
                                col_ser;


                        end_reached = true;


                    }

                    delete
                            ser;
                    delete
                            deserialized_mes;

                    std::cout << "Hello!!!!!!!!!!!!!" <<
                              std::endl;


                    return
                            df;
                }
            }
        }
    }
};
