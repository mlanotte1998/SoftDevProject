// Lang::CwC

#include <stdio.h>
#include <stdlib.h>

#include "../src/application/trivial.h"


/**
 * The main function.
 */
int main(int argc, char* argv[]) {
	KDStore* k = new KDStore(0);
	Trivial trivial(0, k);
	trivial.run_();
	return 0;
}
