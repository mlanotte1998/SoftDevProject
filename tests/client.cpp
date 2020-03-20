//
// Created by Michael Lanotte and Daniel Welch on 2/17/20.
//

#include "../src/network/network.h"
#include "../src/network/network-helper.h"

#define PORT 8080
// For prototype went with "nice" simple number of 10 for the max clients
#define FIXED_CLIENTS 10

char kill_switch[2] = {0};

/**
 * Signal handler for when someone tries to close the program.
 * @param sig_num signal number.
 */
void sigintHandler(int sig_num)
{
    kill_switch[0] = '1';
}

/***
 * Main function for running the client and server programs.
 * @param argc command line argument count
 * @param argv command line argument list
 * @return returns exit.
 */
int main(int argc, char **argv) {

    // Get the ip from the arguments and error if no ip given.
    char ip_buffer[1024] = {0};
    if (get_ip_from_arguments(argc, argv, ip_buffer) < 0) {
        printf("%s\n", "Error, no ip passed in!");
        exit(-1);
    }

    kill_switch[0] = '0';
    kill_switch[1] = '\0';

    char rendezvous_ip[10] = {0}; 
    strcpy(rendezvous_ip, "127.0.0.1");
    // Run the Client program.
    Node* n = new Node(ip_buffer, PORT, rendezvous_ip, FIXED_CLIENTS);
    n->run(kill_switch);

    delete n;

    return 0;
}
