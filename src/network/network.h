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

// authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu

/**
 * Function for creating a new poll file descriptor struct with a given fd number and array index.
 * @param fd_number The poll fd number.
 * @param index The index that this new struct should go in the pfds_ list.
 * @param pfds List of pollfds to be added to.
 * @param pfds_list List of pollfd pointers needed to delete the structs, also needs to hold the new struct.
 */
void create_new_pollfd(int fd_number, int index, struct pollfd *pfds, struct pollfd **pfds_list) {

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

//====================================================================================================

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
     * @return returns the server file descriptor.
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
     * @param size Size of the message buffer to send.
     * @return Returns the bytes sent, -1 if error.
     */
    int socket_send(int sock, char *message, size_t size) {
        return send(sock, message, size, 0);
    }

};

//===================================================================================================

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
     * Constructor so that this class can be abstract.
     */
    Client() {}

    /**
     * Read from a the server.
     * @param buffer Buffer to hold the incoming message.
     * @param buffer_size Size of the buffer.
     * @return Int that is positive for success and -1 if error reading.
     */
    virtual int socket_read(char *buffer, int buffer_size) {
        return read(sock_, buffer, buffer_size);
    }

    /**
     * Read from a server but with a dont wait flag so the program is not stuck.
     * @param buffer Buffer to hold the incoming message.
     * @param buffer_size Size of the buffer.
     * @return Return int that is positive for success and -1 if error reading.
     */
    virtual int socket_recv(char *buffer, int buffer_size) {
        return recv(sock_, buffer, buffer_size, MSG_DONTWAIT);
    }

    /**
     * Send a message to the server.
     * @param message Message to send.
     */
    virtual int socket_send(char *message, size_t size) {
        return send(sock_, message, size, 0);
    }

};

/**
 * Object for wrapping a Server so that it can act like a client.
 */
class ServerWrapper : public Client {
public:

    Server *server_; // The server object held inside (Does not gain ownership).
    int sock_num_; // The sock number to use for the read and write functions.

    /**
     * Constructor for a ServerWrapper.
     * @param server The server object to be held inside.
     * @param sock_num // The sock number to use for the read and write functions.
     */
    ServerWrapper(Server *server, int sock_num) {
        server_ = server;
        sock_num_ = sock_num;
    }

    /**
     * Destructor, nothing to delete because no ownership of Server*.
     */
    ~ServerWrapper() {}

    /**
     * Read a message from a client to the server held inside.
     * @param buffer Buffer to read the message onto.
     * @param buffer_size Size of the buffer to know much how to read in.
     * @return
     */
    int socket_read(char *buffer, int buffer_size) {
        return server_->socket_read(sock_num_, buffer, buffer_size);
    }

    /**
     * Send a message to a client from the server.
     * @param message Message to send.
     * @param size size of the message to send.
     */
    int socket_send(char *message, size_t size) {
        return server_->socket_send(sock_num_, message, size);
    }

};

//===========================================================================================

/**
 * Node class
 * Has ownership of all pointers except the map.
 */
class Node : public Object {
public:

    int max_clients_;// The maximum clients that can join the server.

    char *client_ip_; // Ip of this node.
    int port_; // Port for this node.

    int nodes_count_; // Total other clients known to the network.
    String **nodes_ip_list_; // List of the total client ips known to the network.
    size_t *nodes_port_list_; // List of total ports for ips known to the network.
    size_t *nodes_id_list_; // List of total nodes for nodes known to the network.

    // Member variables for the internal clients (IC)
    Client **IC_client_connections_; // Client objects for connecting to other nodes.
    char **IC_client_connections_ips_; // Ips of client objects.
    int IC_client_connections_count_; // Count of client objects for connecting to other nodes.

    // Member variables for the internal server (IS)
    Server *internalServer_; // The internal server object for receiving messages from other clients.
    struct pollfd *IS_pfds_; // Structs used for polling messages on the server.
    struct pollfd **IS_pfds_list_; // List of pointers to pfds created since the
    // main pfds_ needs to be a list of structs instead of pointers.
    // Need this to delete the created structs.
    bool IS_listener_added_; // boolean to keep track of if run happened to make
    // sure the listener pfd is only deleted if it was created.
    int *IS_sockets_; // List of socket values for every client that has connected.
    int IS_total_socket_count_; // Count of clients that have connected on a socket.

    Map *store_map_; // store map from application. Does not have ownership.
    size_t node_; // node number for the application
    bool connected_to_rendezvous; // Boolean for knowing whether the rendezvous connection has finished.
    // this is important because if a node tries to do a put/wait and get with node 0 before finishing the
    // initial connection, then threading can cause the wrong message to go to node 0 first when it is expecting
    // a Register.
    bool lock_rendezvous_connection; // Boolean for locking the rendezvous connection while
    // there is a put or wait_and_get with node 0.


    /**
     * Main Node Constructor
     * @param client_ip The ip for this node.
     * @param port The port for this node.
     * @param server_ip The ip of the rendezvous server.
     * @param max_clients The maximum number of clients that can be added.
     * @param map The store map to be passed in.
     * @param node The node index for the application.
     */
    Node(char *client_ip, int port, char *server_ip, int max_clients, Map *map, size_t node) {

        max_clients_ = max_clients;

        // Internal client config
        char *client_ip_copy = new char[strlen(client_ip) + 1];
        strcpy(client_ip_copy, client_ip);
        client_ip_ = client_ip_copy;
        port_ = port;
        Client *rendezvousServerClient = new Client(client_ip_, port_, server_ip);
        IC_client_connections_ = new Client *[max_clients_];
        IC_client_connections_ips_ = new char *[max_clients_];
        // Add rendezvousServerClient as the first client connection
        IC_client_connections_[0] = rendezvousServerClient;
        IC_client_connections_ips_[0] = duplicate(server_ip);
        IC_client_connections_count_ = 1;

        nodes_ip_list_ = new String *[max_clients_];
        nodes_port_list_ = new size_t[max_clients_];
        nodes_id_list_ = new size_t[max_clients_];
        // Add rendezvousServerClient values to the lists below.
        nodes_ip_list_[0] = new String(server_ip);
        nodes_port_list_[0] = 8081;
        nodes_id_list_[0] = 0;
        nodes_count_ = 1;


        // Internal Server config
        internalServer_ = new Server(client_ip_, port_);
        internalServer_->socket_bind();
        IS_pfds_ = new struct pollfd[max_clients_ + 1];
        IS_pfds_list_ = new struct pollfd *[max_clients_ + 1];
        IS_sockets_ = new int[max_clients_];
        IS_total_socket_count_ = 0;

        // Set more application specific values.
        store_map_ = map;
        node_ = node;
        lock_rendezvous_connection = false;
        connected_to_rendezvous = false;
    }

    /**
    * Rendezvous Server Node Constructor
    * No client ip needed because this is the rendezvous server so no need to start with that.
    * @param server_ip The ip of the rendezvous server.
    * @param port The port for this node.
    * @param max_clients The maximum number of clients that can be added.
    * @param map The store map to be passed in.
    * @param node The node index for the application.
    */
    Node(char *server_ip, int port, int max_clients, Map *map, size_t node) {

        max_clients_ = max_clients;

        // Internal client config
        port_ = port;
        IC_client_connections_ = new Client *[max_clients_];
        IC_client_connections_ips_ = new char *[max_clients_];
        IC_client_connections_count_ = 0;

        nodes_ip_list_ = new String *[max_clients_];
        nodes_port_list_ = new size_t[max_clients_];
        nodes_id_list_ = new size_t[max_clients_];
        nodes_count_ = 0;

        // Internal Server config
        internalServer_ = new Server(server_ip, port_);
        internalServer_->socket_bind();
        IS_pfds_ = new struct pollfd[max_clients_ + 1];
        IS_pfds_list_ = new struct pollfd *[max_clients_ + 1];
        IS_sockets_ = new int[max_clients_];
        IS_total_socket_count_ = 0;

        // Set more application specific values.
        store_map_ = map;
        node_ = node;
        connected_to_rendezvous = true; // this is the rendezvous server so this is true.
        lock_rendezvous_connection = false;
    }

    /**
     * Destructor for the Node
     */
    ~Node() {

        // Delete Internal Client members
        delete[] client_ip_;
        for (int i = 0; i < IC_client_connections_count_; i++) {
            delete IC_client_connections_[i];
            delete[] IC_client_connections_ips_[i];
        }
        delete[] IC_client_connections_;
        delete[] IC_client_connections_ips_;

        for (int i = 0; i < nodes_count_; i++) {
            delete nodes_ip_list_[i];
        }
        delete[] nodes_ip_list_;
        delete[] nodes_port_list_;
        delete[] nodes_id_list_;

        // Delete Internal Server members
        delete internalServer_;
        delete[] IS_pfds_;
        delete[] IS_sockets_;
        for (int i = 0; i < IS_total_socket_count_; i++) {
            // Other clients will be at index i + 1 because the first index is the listener.
            delete IS_pfds_list_[i + 1];
        }
        if (IS_listener_added_) {
            // Delete the listener if one was added.
            delete IS_pfds_list_[0];
        }
        delete[] IS_pfds_list_;
    }

    /**
     * Function for running the node implementation.
     * @param kill_switch Int for being able to kill the program gracefully.
     */
    virtual void run(char *kill_switch) {

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

    // Functions related to receiving messages on the internal server.

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
                        // Accept the new client and pass on the sock index to the function for
                        // handling a client message.
                        int current_sock_idx = accept_new_client();
                        client_message_received(current_sock_idx);
                    } else {
                        // Not a new client, so sock idx is the i - 1 since the i = 0 is the listener
                        // causes all of the sock indices to be shifted by 1 with their poll indices.
                        client_message_received(i - 1);
                    }
                }
            }
        }
    }

    /**
    * Handles a client joining the network.
    * @return Returns the sock index of the newly accepted client.
    */
    virtual size_t accept_new_client() {
        // Accept the socket or error if bad acception which is a return of -1
        if ((IS_sockets_[IS_total_socket_count_] = internalServer_->socket_accept()) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Current sock idx is the total socket count before incrementing it.
        int current_sock_idx = IS_total_socket_count_;

        // Create a new pollfd for the socket that client connected on.
        // Pass in total_socket_count + 1 because the + 1 accounts for the listener also being in
        // the pfds list.
        create_new_pollfd(IS_sockets_[IS_total_socket_count_], IS_total_socket_count_ + 1,
                          IS_pfds_, IS_pfds_list_);

        // Increase the socket count.
        IS_total_socket_count_++;

        // Return the socket index to be used.
        return current_sock_idx;
    }

    /**
     * Handles a client joining the network
     * @param buffer A string buffer that is being passed around for handling most messages.
     */
    void client_message_received(size_t current_sock_idx) {

        // Buffer for holding message data.
        char buffer[1024] = {0};

        // Read in the intial message with should be one of a put or wait_and_get.
        internalServer_->socket_read(IS_sockets_[current_sock_idx], buffer, 1024);
        Object *deserialized = deserialize_buffer(buffer);

        // Make sure the object is a message and then run the respective function based on its type.
        if (dynamic_cast<Message *>(deserialized) != nullptr) {
            Message *mes = dynamic_cast<Message *>(deserialized);
            if (mes->kind_ == MsgKind::WaitAndGet) {
                handle_wait_and_get(current_sock_idx, mes->sender_);
            } else if (mes->kind_ == MsgKind::Put) {
                handle_put(current_sock_idx, mes->sender_);
            }
        }

        // Delete the message object.
        delete deserialized;
    }

    /**
     * Function for handling a put request coming in.
     * @param socket_idx index of the socket to interact with.
     * @param target target node id.
     */
    void handle_put(int socket_idx, size_t target) {

        // Create a server wrapper for this client interaction.
        ServerWrapper *sw = new ServerWrapper(internalServer_, IS_sockets_[socket_idx]);

        // Get the initial object sent from the other client.
        Object *ob = handle_initial_response(sw, target);

        // Have a DataFrame pointer to set below.
        DataFrame *df;

        // Check if next message is a status.
        if (dynamic_cast<Status *>(ob) != nullptr) {
            Status *stat = dynamic_cast<Status *>(ob);
            // Continue to read in the DataFrame.
            df = receive_dataframe(sw, target);
        } else {
            // Not a status then error out something went wrong.
            exit(1);
        }

        // Add DataFrame to the store.
        Key *new_key = new Key(dynamic_cast<Status *>(ob)->msg_->c_str(), node_);
        store_map_->put(new_key, df);
        delete new_key;

        printf("%s\n", "Put finished");

        // Delete created objects
        delete sw;
        delete ob;
    }

    /**
     * Handle a wait and get message being received.
     * @param socket_idx Index of socket to interact with.
     * @param target Id of target node.
     */
    void handle_wait_and_get(int socket_idx, size_t target) {

        // Buffer to read or write data onto for message sending/receiving.
        char buffer[1024] = {0};

        // Wrap the server-client interaction within this to use in helper functions.
        ServerWrapper *sw = new ServerWrapper(internalServer_, IS_sockets_[socket_idx]);

        // Get the initial object sent from the other client.
        Object *ob = handle_initial_response(sw, target);

        // If status received then it is the message we want with the information of the key.
        if (dynamic_cast<Status *>(ob) != nullptr) {
            // Create key to check if the store has the requested DataFrame.
            Key *key = new Key(dynamic_cast<Status *>(ob)->msg_->c_str(), node_);

            // If the DataFrame is there then proceed.
            if (store_map_->contains_key(key)) {
                Object *ob = store_map_->get(key);
                DataFrame *desired_df = dynamic_cast<DataFrame *>(ob);

                //TODO Need to handle telling it how many columns
                // Currently works because only one

                for (int i = 0; i < desired_df->ncols(); i++) {
                    Column *cur_col = desired_df->cols_[i];
                    send_column(sw, cur_col, target, false);
                }

                printf("%s\n", "Wait and Get finished");
            } else {
                // Nack that no DataFrame was found yet.
                Serializer *strlen_ser = get_message_serializer(MsgKind::Nack, node_, target, 0);
                strcpy(buffer, strlen_ser->buffer_);
                internalServer_->socket_send(IS_sockets_[socket_idx], buffer, 1024);
                delete strlen_ser;
            }

            // delete key object created.
            delete key;

        } else {
            // Error if anything else received here. Should not happen.
            delete ob;
            delete sw;
            exit(1);
        }

        // Delete created objects.
        delete sw;
        delete ob;
    }

    /**
     * Helper function of repeated code for initial interaction after receiving a put or wait and get message.
     * @param cur_client ServerWrapper will be used for sending and receiving messages.
     * @param target Target nodes id.
     * @return Returns the message that should be a Status that holds more information to continue
     * the interaction.
     */
    Object *handle_initial_response(ServerWrapper *cur_client, int target) {

        // Create a buffer for reading and writing messages.
        char buffer[1024] = {0};

        // Send an ack message to acknowledge that the put was received.
        Serializer *ack_ser = get_ack_serializer(node_, target, 0);
        char *serialized_ack = ack_ser->buffer_;
        strcpy(buffer, serialized_ack);
        cur_client->socket_send(buffer, 1024);

        // Reset the buffer to then read in the next message.
        memset(buffer, 0, 1024);
        cur_client->socket_read(buffer, 1024);
        Object *ob = deserialize_buffer(buffer);

        // delete created serializer.
        delete ack_ser;

        return ob;

    }

    //------------------------------------------------------------------
    // Functions for communication with registrar.

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

            // Make sure the rendezvous connection wasn't locked by a different thread that is interacting
            // with node 0 for a put/wait and get.
            if (!lock_rendezvous_connection) {
                // Reset buffer in the while loop so new messages dont have parts of old ones in them.
                memset(buffer, 0, 1024);

                // Read from server, if no response than close because that means the server went down.
                if (IC_client_connections_[0]->socket_recv(buffer, 1024) == 0) {
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
                    connected_to_rendezvous = true;
                }
                    // Else if a directory is the incoming message then copy all of the data
                    // to the values of this node.
                else if (dynamic_cast<Directory *>(deserialized) != nullptr) {
                    Directory *dir = dynamic_cast<Directory *>(deserialized);
                    add_directory(dir);
                    printf("%s\n", "Connected to Rendezvous");
                    connected_to_rendezvous = true;
                }
                delete deserialized;
            }
        }
    }

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
        IC_client_connections_[0]->socket_send(buffer, 1024);

        delete registerSer;
    }

    /**
     * Take in a register as a node and add it to all of the lists needed for
     * knowledge of other nodes.
     * @param reg Register to add.
     */
    void add_register(Register *reg) {

        nodes_ip_list_[nodes_count_] = reg->client_ip_->clone();;
        nodes_port_list_[nodes_count_] = reg->port_;
        nodes_id_list_[nodes_count_] = reg->sender_;
        nodes_count_++;

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

    //---------------------------------------------------------------
    // Application Specific Functions

    /**
     * Handle put functionality.
     * @param k Key of the DataFrame to put.
     * @param df The actual DataFrame to put.
     */
    void put(Key k, DataFrame *df) {

        // Buffer for reading and sending messages
        char buffer[1024] = {0};

        // Boolean for keeping a while loop until the put has been finished.
        // Could be waiting for the other node to have connected to the network.
        bool message_sent = false;

        // Run a loop until the process has finished.
        while (!message_sent) {

            // Make sure that the node has connected to the rendezvous server.
            if (connected_to_rendezvous) {



                // Figure out which client object to use for interaction.
                Client *cur_client = get_client_of_key(k);

                // Make sure a client was found.
                if (cur_client != nullptr) {

                    message_sent = true;

                    // TODO can send df length here
                    // Send the starting put message to start everything off.
                    Serializer *put_ser = get_message_serializer(MsgKind::Put, node_, k.idx_, 0);
                    strcpy(buffer, put_ser->buffer_);
                    cur_client->socket_send(buffer, 1024);
                    delete put_ser;

                    // If interacting with the rendezvous server then lock the other thread from
                    // attempting to read messages from it.
                    if (k.idx_ == 0) {
                        lock_rendezvous_connection = true;
                    }

                    // Read in ack message.
                    memset(buffer, 0, 1024);
                    cur_client->socket_read(buffer, 1024);

                    Object* ack = deserialize_buffer(buffer);
                    if (dynamic_cast<Ack*>(ack) != nullptr) {
                      // do nothing this is good
                    } else if (dynamic_cast<Register*>(ack) != nullptr) {
                      // a register could have been sent before responding to
                      // our message so need to account for that.
                      add_register(dynamic_cast<Register*>(ack));
                      memset(buffer, 0, 1024);
                      cur_client->socket_read(buffer, 1024);
                    } else {
                      exit(1);
                    }

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
                        send_column(cur_client, cur_col, k.idx_, true);
                    }


                }
            }
        }
        // unlock rendezvous connection once finished just in case.
        lock_rendezvous_connection = false;
    }

    /**
     * Function for doing a wait and get request.
     * @param k Key of requested DataFrame.
     * @return Returns the Dataframe requested.
     */
    DataFrame *waitAndGet(Key k) {

        // Buffer for reading and sending messages
        char buffer[1024] = {0};

        // Run a loop until the process has finished.
        while (true) {

            // Make sure this has connected to the rendezvous
            if (connected_to_rendezvous) {
                // Figure out which client object to use for interaction.
                Client *cur_client = get_client_of_key(k);

                if (cur_client != nullptr) {

                    lock_rendezvous_connection = true;

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

                    // Get the DataFrame
                    DataFrame *df = receive_dataframe(cur_client, k.idx_);

                    // Can unlock the rendezvous connection now that process has finished
                    lock_rendezvous_connection = false;

                    // If the DataFrame returned is nullptr than the other node doesn't have
                    // it yet so don't return yet.
                    if (df != nullptr) {
                        return df;
                    }
                }
            }
        }
    }

    /**
     * Function for getting a client object to interact with
     * @param k Key with information on which node we are looking for.
     * @return Returns client or nullptr if the other node is not known to this one yet.
     */
    Client *get_client_of_key(Key k) {
        // Set client to nullptr to start.
        Client *client = nullptr;

        // Look through known client for the node id wanted.
        size_t node_idx = max_clients_;
        for (int i = 0; i < nodes_count_; i++) {
            if (nodes_id_list_[i] == k.idx_) {
                // if node id found then set the node idx variable.
                node_idx = i;
                break;
            }
        }

        // If node idx was changed
        if (node_idx != max_clients_) {
            // Looking for client with the ip at the index found.
            char *target_ip = nodes_ip_list_[node_idx]->cstr_;
            for (int i = 0; i < IC_client_connections_count_; i++) {
                if (strcmp(target_ip, IC_client_connections_ips_[i]) == 0) {
                    // If ip is found then this i is where the client already made is.
                    client = IC_client_connections_[i];
                }
            }

            // If no client for the ip has been made yet then create one.
            if (client == nullptr) {
                client = new Client(client_ip_, nodes_port_list_[node_idx], nodes_ip_list_[node_idx]->cstr_);

                IC_client_connections_[IC_client_connections_count_] = client;
                IC_client_connections_ips_[IC_client_connections_count_] = duplicate(nodes_ip_list_[node_idx]->cstr_);
                IC_client_connections_count_++;
            }
        }

        // Return the client
        return client;
    }

    /**
     * Handle sending one column of a DataFrame for the put functionality.
     * @param cur_client Client to use for sending messages.
     * @param cur_col Column to send.
     * @param target Target node id.
     * @param put Bool showing whether a put called this or else this is for handling a wait and get.
     */
    void send_column(Client *cur_client, Column *cur_col, size_t target, bool put) {

        // Buffer for sending and receiving messages.
        char buffer[1024] = {0};

        // Create serialized column.
        Serializer *col_ser = new Serializer();
        char *col_string = col_ser->serialize(cur_col);

        // Create Message with id of the length of the column message so that the
        // other node knows how big of a message it will be receiving in chunks.
        // Also set the kind of message to ensure this is the right behavior.
        MsgKind mes_kind = put ? MsgKind::Put : MsgKind::WaitAndGet;
        Serializer *strlen_ser = get_message_serializer(mes_kind, node_, target, strlen(col_string));
        strcpy(buffer, strlen_ser->buffer_);
        cur_client->socket_send(buffer, 1024);

        // TODO Maybe check that this is an ack.
        memset(buffer, 0, 1024);
        cur_client->socket_read(buffer, 1024);

        // Send the column in chunks.
        send_column_chunks(cur_client, cur_col, target, col_string, strlen(col_string));

        // Send the same put message from earlier again to show that the column sending is done.
        memset(buffer, 0, 1024);
        strcpy(buffer, strlen_ser->buffer_);
        cur_client->socket_send(buffer, 1024);

        delete col_ser;
        delete strlen_ser;
    }

    /**
     * Function for sending a column chunk by chunk
     * @param cur_client Client to use for sending messages.
     * @param cur_col Column to send.
     * @param target Target node id.
     * @param col_string_len Length of the column string so that this knows when to stop.
     */
    void send_column_chunks(Client *cur_client, Column *cur_col, size_t target,
      char* col_string , size_t col_string_len) {
        // Buffer for sending and receiving messages.
        char buffer[1024] = {0};

        // Keep track of count of message that has been sent to ensure
        // the entire column is sent.
        size_t count = 0;

        // Send the column in chunks and increment the count to keep track of how much has been sent.
        while (count < col_string_len) {
            memset(buffer, 0, 1024);
            strncpy(buffer, col_string + count, 1022);
            buffer[1023] = '\0';
            cur_client->socket_send(buffer, 1024);
            count += 1022;
        }
    }

    /**
     * Function for receiving the DataFrame during a wait and get.
     * @param cur_client Client currently interacting with.
     * @param target Target node id.
     * @return Returns the DataFrame that is requested.
     */
    DataFrame *receive_dataframe(Client *cur_client, size_t target) {

        // Buffer for receiving and sending messages.
        char buffer[1024] = {0};

        // Create DataFrame and boolean for going through receving columns.
        bool end_reached = false;
        Schema s("");
        DataFrame *df = new DataFrame(s);

        // TODO handle actually checking how big the dataframe is

        // While the end of the dataframe messages are not reached.
        while (!end_reached) {

            // Read in message
            memset(buffer, 0, 1024);
            cur_client->socket_read(buffer, 1024);

            // Deserialized message should be a Message. The kind is then what is important.
            Message *deserialized_mes = dynamic_cast<Message *>(deserialize_buffer(buffer));

            // If wait and get message then a column needs to be read
            if (deserialized_mes->kind_ == MsgKind::WaitAndGet || deserialized_mes->kind_ == MsgKind::Put) {
                receive_column(cur_client, target, deserialized_mes->id_, df);
                end_reached = true;
            } else if (deserialized_mes->kind_ == MsgKind::Nack) {
                // Nack send meaning no df yet so return nullpr;
                delete df;
                delete deserialized_mes;
                return nullptr;
            }

            delete deserialized_mes;
        }

        // return the DataFrame.
        return df;
    }


    /**
     * Receive a column from another node.
     * @param cur_client Client we are interacting with.
     * @param target Target node's id.
     * @param msg_sz Size of the column message that will be received.
     * @param df DataFrame to add the column to.
     */
    void receive_column(Client *cur_client, size_t target, size_t msg_sz, DataFrame *df) {

        // Buffer for sending ack to start sequence.
        char buffer[1024] = {0};

        // Create a buffer for receiving the column based on the info
        // from the received put message. Need size of at least 1024 to read in successfully.
        size_t size_of_message = msg_sz;
        if (size_of_message < 1024) {
            size_of_message = 1024;
        }
        char *new_buffer = new char[size_of_message + 1];
        memset(new_buffer, 0, size_of_message + 1);

        // Create and send an ack message to then initiate the start of receiving the column.
        Serializer *ack_ser = get_ack_serializer(node_, target, 0);
        strcpy(buffer, ack_ser->buffer_);
        cur_client->socket_send(buffer, 1024);
        delete ack_ser;

        // Keep a count of how many reads have been done for this column for use in helper function.
        size_t recv_count = 0;
        // Keep track of if the while loop should continue.
        bool message_unfinished = true;

        while (message_unfinished) {
            // Read in the column message, if not a part of the column then that column is over.
            message_unfinished = read_in_column(cur_client, new_buffer, recv_count);
            recv_count++;
        }

        // Create a serializer for the column and then add the deserialized column to the dataframe.
        Serializer *col_ser = new Serializer(reinterpret_cast<unsigned char *>(new_buffer));
        df->add_column(dynamic_cast<Column *>(col_ser->deserialize()));

        // Delete serializers.
        delete col_ser;
    }

    /**
    * Function for reading in a part of a column.
    * @param cur_client Client currently interacting with.
    * @param append_buffer Buffer to append incoming messages to to build DataFrame.
    * @param recv_count Count of read iterations.
    * @return Returns a bool, true if continue reading, false to stop.
    */
    bool read_in_column(Client *cur_client, char *append_buffer, size_t recv_count) {

        // Buffer for reading in this a chunk of a column.
        char buffer[1024] = {0};
        cur_client->socket_read(buffer, 1024);

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
};

//============================================================================

/**
 * Rendezvous Server object that acts as the registrar server.
 * When the server dies the clients will no longer be able to read from it and
 * so they shut down.
 */
class RendezvousServer : public Node {
public:

    RendezvousServer(char *ip, int port, int max_clients, Map *map, size_t node)
            : Node(ip, port, max_clients, map, node) {}


    /**
     * Function for handling getting a message on the listener socket.
     */
    void message_on_listener() {

        // Accept the new client that is sending a message to the listener.
        accept_new_client();

        // Create a new pollfd for the socket that client connected on.
        // Pass in total_socket_count + 1 because the + 1 accounts for the listener also being in
        // the pfds list.
        create_new_pollfd(IS_sockets_[IS_total_socket_count_], IS_total_socket_count_ + 1, IS_pfds_, IS_pfds_list_);

        // Increase the socket count.
        IS_total_socket_count_++;
    }

    /**
    * Handles a client joining the network
    */
    size_t accept_new_client() {

        // Buffer for reading the register message from the client.
        char buffer[1024] = {0};

        // Accept the socket or error if bad acception which is a return of -1
        if ((IS_sockets_[IS_total_socket_count_] = internalServer_->socket_accept()) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read in the message that should be the ip of the client.
        internalServer_->socket_read(IS_sockets_[IS_total_socket_count_], buffer, 1024);
        Register *reg = dynamic_cast<Register *>(deserialize_buffer(buffer));
        add_register(reg);
        int target = reg->target_;
        delete reg;

        // Send the register to all clients besides the new one.
        for (int i = 0; i < IS_total_socket_count_; i++) {
            internalServer_->socket_send(IS_sockets_[i], buffer, 1024);
        }

        // Create a directory to send to the new client.
        Serializer *directory_ser = get_directory_serializer(node_, target, 9999, node_, IS_total_socket_count_,
                                                             nodes_port_list_,
                                                             IS_total_socket_count_, nodes_ip_list_,
                                                             IS_total_socket_count_, nodes_id_list_);
        strcpy(buffer, directory_ser->buffer_);
        internalServer_->socket_send(IS_sockets_[IS_total_socket_count_], buffer, 1024);
        delete directory_ser;
    }

    /**
     * Function for running the rendezvous server.
     */
    void run(char *kill_switch) {

        // Begin listening.
        int listener = internalServer_->socket_listen();

        // Create the pollfd struct for the listener socket.
        create_new_pollfd(listener, 0, IS_pfds_, IS_pfds_list_);
        IS_listener_added_ = true;

        // Loop on true to continuously run the server.
        while (kill_switch[0] != '1') {

            // Poll the server activity.
            int poll_count = poll(IS_pfds_, IS_total_socket_count_ + 1, 0);
            if (poll_count == -1) {
                kill_switch[0] = '1';
            }
            // Run through the existing connections looking for data to read.
            for (int i = 0; i < IS_total_socket_count_ + 1; i++) {
                // Check if a port is ready to be read from
                if (IS_pfds_[i].revents & POLLIN) {
                    // If the server listener is ready to read from then handle the new
                    // message on this listener socket.
                    if (IS_pfds_[i].fd == listener) {
                        message_on_listener();
                    } else {
                        client_message_received(i - 1);
                    }
                }
            }
        }
    }
};
