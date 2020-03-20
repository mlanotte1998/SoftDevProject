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
#include "network-helper.h"

/**
 * Function for creating a new poll file descriptor struct with a given fd number and array index.
 * @param fd_number The poll fd number.
 * @param index The index that this new struct should go in the pfds_ list.
 * @param pfds List of pollfds to be added to.
 * @param pfds_list List of pollfd pointers needed to delete the structs, also needs to hold the new struct.
 */
void create_new_pollfd(int fd_number, int index, struct pollfd* pfds,
                       struct pollfd** pfds_list) {

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
    void socket_send(int sock, char *message) {
        send(sock, message, strlen(message), 0);
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

    /**
     * Send a message to the server.
     * @param message Message to send.
     */
    void socket_send(char *message) {
        send(sock_, message, strlen(message), 0);
    }

};

//============================================================================

/**
 * Rendezvous Server object that acts as the registrar server.
 * When the server dies the clients will no longer be able to read from it and
 * so they shut down.
 */
class RendezvousServer {
public:

    Server *internalServer_; // The server acting as the registrar server.
    int max_clients_; // The maximum clients that can join the server.
    struct pollfd *pfds_; // Structs used for polling messages on the server.
    struct pollfd **pfds_list_; // List of pointers to pfds created since the
    // main pfds_ needs to be a list of structs instead of pointers.
    // Need this to delete the created structs.
    bool listener_added_; // boolean to keep track of if run happened to make
    // sure the listener pfd is only deleted if it was created.
    char **ip_list_; // List of ip addresses for clients that have joined.
    int *sockets_; // List of socket values for every client that has connected.
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
        ip_list_ = new char *[max_clients_];
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
            delete[] ip_list_[i];
            delete pfds_list_[i + 1];
        }
        if (listener_added_) {
            delete pfds_list_[0];
        }
        delete[] pfds_list_;
        delete[] ip_list_;
        delete[] sockets_;
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

        if (total_socket_count_ == 0) {
            // If the client is the first one to connect then send a First Client message.
            char first_ip[13] = {0};
            strcpy(first_ip, "First Client");
            internalServer_->socket_send(sockets_[total_socket_count_], first_ip);
        } else {
            // If not the first client then send it the list of all previous client's
            // ip addresses.
            char *ip_list_string = get_ip_list_string(ip_list_, total_socket_count_);
            internalServer_->socket_send(sockets_[total_socket_count_], ip_list_string);
            // delete this the list of all ips.
            delete[] ip_list_string;
        }

        // Create a new string for this ip and store it.
        char *new_ip = store_client_ip(buffer, ip_list_, total_socket_count_);
        printf("Client accepted : %s\n", new_ip);

        // Send the new ip to all clients besides the new one.
        for (int i = 0; i < total_socket_count_; i++) {
            internalServer_->socket_send(sockets_[i], buffer);
        }
    }

    /**
     * Function for running the rendezvous server.
     */
    void run(int kill_switch) {

        // Begin listening.
        int listener = internalServer_->socket_listen();

        // Create the pollfd struct for the listener socket.
        create_new_pollfd(listener, 0, pfds_, pfds_list_);
        listener_added_ = true;

        // Loop on true to continuously run the server.
        while (kill_switch != 1) {
            // Poll the server activity.
            int poll_count = poll(pfds_, total_socket_count_ + 1, -1);
            if (poll_count == -1) {
                kill_switch = 1;
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
class Node {
public:

    int max_clients_;// The maximum clients that can join the server.

    Client *internalClient_; // Internal client for hitting the rendezvous server.
    char *client_ip_; // Ip of this node.
    int port_; // Port for this node.
    int IC_other_total_client_count_; // Total other clients known to the network.
    char **IC_ip_list_; // List of the total client ips known to the network.
    Client **IC_other_client_connections_; // Client objects for connecting to other nodes.
    int IC_other_client_connections_count_; // Count of client objects for connecting to other nodes.

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


    /**
     * Main Node Constructor
     * @param client_ip The ip for this node.
     * @param port The port for this node.
     * @param server_ip The ip of the rendezvous server.
     * @param max_clients The maximum number of clients that can be added.
     */
    Node(char *client_ip, int port, char *server_ip, int max_clients) {

        max_clients_ = max_clients;

        // Internal client config
        char *client_ip_copy = new char[strlen(client_ip) + 1];
        strcpy(client_ip_copy, client_ip);
        client_ip_ = client_ip_copy;
        port_ = port;
        IC_other_total_client_count_ = 0;
        internalClient_ = new Client(client_ip_, port_, server_ip);
        IC_other_client_connections_ = new Client *[max_clients_];
        IC_other_client_connections_count_ = 0;
        IC_ip_list_ = new char *[max_clients_];

        // Internal Server config
        internalServer_ = new Server(client_ip_, port_);
        internalServer_->socket_bind();
        IS_pfds_ = new struct pollfd[max_clients_ + 1];
        IS_pfds_list_ = new struct pollfd *[max_clients_ + 1];
        IS_sockets_ = new int[max_clients_];
        IS_ip_list_ = new char *[max_clients_];

        IS_total_socket_count_ = 0;
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
            delete [] IC_ip_list_[i];
        }
        delete [] IC_ip_list_;

        // Delete Internal Server members
        delete internalServer_;
        delete [] IS_pfds_;
        delete [] IS_sockets_;
        for (int i = 0; i < IS_total_socket_count_; i++) {
            delete [] IS_ip_list_[i];
            // Other clients will be at index i + 1 because the first index is the listener.
            delete IS_pfds_list_[i + 1];
        }
        if (IS_listener_added_) {
            // Delete the listener if one was added.
            delete IS_pfds_list_[0];
        }
        delete [] IS_ip_list_;
        delete [] IS_pfds_list_;
    }

    /**
     * Function for running the node implementation.
     * @param kill_switch Int for being able to kill the program gracefully.
     */
    void run(char* kill_switch) {

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
        if ((IS_sockets_[IS_total_socket_count_] = internalServer_->socket_accept()) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read in the message that should be the ip of the client.
        internalServer_->socket_read(IS_sockets_[IS_total_socket_count_], buffer, 1024);

        // Send an acknowledge message back
        char ack[13] = {0};
        strcpy(ack, "-acknowledge");
        internalServer_->socket_send(IS_sockets_[IS_total_socket_count_], ack);

        // Create a new string for this ip and store it.
        char* new_ip_from_message = get_ip_from_client_message(buffer);
        char *new_ip = store_client_ip(new_ip_from_message, IS_ip_list_, IS_total_socket_count_);
        delete new_ip_from_message;
        printf("Other client accepted: %s\n", new_ip);
        printf("%s\n", buffer);
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

        // Reset buffer so that new messages don't overlap.
        memset(buffer, 0, 1024);
    }


    /**
     * Function for running the internal server implementation so that this node can
     * receive messages from other nodes.
     * @param kill_switch
     */
    void run_internal_server(char* kill_switch) {

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
     * Function for when the list of other clients known to the rendezvous server is received.
     * @param buffer Char buffer holding the list message.
     */
    void received_ip_list_message(char *buffer) {
        // Build the list of ips from this message.
        IC_other_total_client_count_ = build_ip_list(buffer, IC_ip_list_);

        // Send a message to the last client in the list.
        send_client_to_client_message(IC_ip_list_[IC_other_total_client_count_ - 1]);
    }

    /**
     * Function for sending a message from a client to another client.
     * @param receiver_ip The ip of the other client that should receive the message.
     */
    void send_client_to_client_message(char *receiver_ip) {
        // Create a new client object to connect to the receiver client.
        Client *new_client_connection = new Client(client_ip_, port_, receiver_ip);
        // Add this client to the internal list of clients
        IC_other_client_connections_[IC_other_client_connections_count_] = new_client_connection;
        IC_other_client_connections_count_++;

        // Create the message to send to the new_client_connection.
        char buffer[1024] = {0};
        strcpy(buffer, "-ip ");
        strcat(buffer, client_ip_);
        strcat(buffer, " Hello from fellow client");

        // Send the message, clear the buffer, and then read hoping for an acknowledge message back.
        new_client_connection->socket_send(buffer);
        memset(buffer, 0, 1024);
        new_client_connection->socket_read(buffer, 1024);
        printf("%s\n", buffer);
    }

    /**
     * Function for running the implementation for the node communicating with the
     * Rendezvous Server.
     * @param kill_switch Int value for knowing when to gracefully kill the program.
     */
    void run_rendezvous_communication(char* kill_switch) {
        // Create buffer for incoming and outgoing messages.
        char buffer[1024] = {0};

        // Create the message to send to the server that is this ip with an -ip tag.
        strcpy(buffer, "-ip ");
        strcat(buffer, client_ip_);
        internalClient_->socket_send(buffer);

        // Loop continuously while connected to the server.
        while (kill_switch[0] != '1') {
            // Reset buffer in the while loop so new messages dont have parts of old ones in them.
            memset(buffer, 0, 1024);

            // Read from server, if no response than close because that means the server went down.
            if (internalClient_->socket_read(buffer, 1024) == 0) {
                // End when the server stops running.
                kill_switch[0] = '1';
                return;
            }
            // If message is the First Client message then print this and do nothing.
            if (strcmp(buffer, "First Client") == 0) {
                printf("First client registered, no other IPs known\n");
            }
            // List message received
            else if ((buffer[0] == '-' && buffer[1] == 'l' && buffer[2] == 'i' &&
                      buffer[3] == 's' && buffer[4] == 't')) {
                // Handle receiving a list message from the rendezvous server.
                received_ip_list_message(buffer);
            }
            // Getting one singular new ip
            else if (buffer[0] == '-' && buffer[1] == 'i' && buffer[2] == 'p') {
                // run the function for handling the one new client added to the network
                new_ip_received_by_client(IC_ip_list_, IC_other_total_client_count_, buffer);
                IC_other_total_client_count_++;
            }
        }
    }
};
