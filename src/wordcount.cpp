// Lang::CwC

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <cstdlib>

#include "application/wordcount/wordcount.h"


char kill_switch[2] = {0};

void handle_sigint(int sig)
{
    kill_switch[0] = '1';
}

/**
 * The main function.
*/
int main(int argc, char* argv[]) {

	int node = atoi(argv[2]);
	char* filename = argv[4];

	signal(SIGINT, handle_sigint);
	kill_switch[0] = '0';
	kill_switch[1] = '\0';


	KDStore* store = new KDStore(node);

	WordCount w (node, store, filename, 0);

	std::thread network_thread(&KDStore::run_network, store, kill_switch);

	std::thread wordcount_thread(&WordCount::run_, w);

	network_thread.join();
	wordcount_thread.join();

	delete store;

	return 0;
}
