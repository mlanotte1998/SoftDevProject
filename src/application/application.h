#include <stddef.h>
#include "../store/kdstore.h"

/**
 * Application::
 * Runnable application using a KDStore to store dataframes
 * and a node index
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
*/
class Application : public Object {
	public:
		size_t idx_;
		KDStore* kv;

		/** Application constructor */
		Application(size_t idx, KDStore* kd) {
			idx_ = idx;
      kv = kd;
		}

		/** Application destructor */
		~Application() {}

		/** Returns the node index of this Application */
		size_t this_node() {
		    return idx_;
		}

		/** Run method to be overridden by child class */
		virtual void run_() {}
};
