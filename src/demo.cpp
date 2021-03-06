// Lang::CwC

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <cstdlib>

#include "application/demo.h"


char kill_switch[2] = {0};

void handle_sigint(int sig)
{
    kill_switch[0] = '1';
}

/**
 * The main function.
*/
int main(int argc, char* argv[]) {

    int x = atoi(argv[2]);

    signal(SIGINT, handle_sigint);
    kill_switch[0] = '0';
    kill_switch[1] = '\0';


    KDStore* k = new KDStore(x);

    Demo d (x, k);

    std::thread network_thread(&KDStore::run_network, k, kill_switch);

    std::thread demo_thread(&Demo::run_, d);

    network_thread.join();
    demo_thread.join();

    delete k;

    return 0;
}
