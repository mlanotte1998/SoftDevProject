// Lang::CwC

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "application/demo.h"

/*
char kill_switch[2] = {0};

void handle_sigint(int sig)
{
    kill_switch[0] = '1';
    std::cout << "hello" << std::endl;
}

/**
 * The main function.

int main(int argc, char* argv[]) {

    signal(SIGINT, handle_sigint);
    kill_switch[0] = '0';
    kill_switch[1] = '\0';

    KDStore* k = new KDStore(0);

    Demo d (0, *k);

    //std::thread network_thread(&KDStore::run_server, k, kill_switch);

    // std::thread demo_thread(&Demo::run_, d);

    //network_thread.join();
    //demo_thread.join();

    d.run_();

    return 0;
}
 */

/**
 * The main function.
 */
int main(int argc, char* argv[]) {
    KDStore k(0);
    Demo demo(0, &k);
    demo.run_();
    return 0;
}

