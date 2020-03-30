#include <stddef.h>
#include "../store/kdstore.h"


class Application : public Object {
	public:
		size_t idx_;
		KDStore* kv;

		Application(size_t idx, KDStore* kd) {
			idx_ = idx;
            kv = kd;
		}

		~Application() {
    }

		size_t this_node() {
		    return idx_;
		}

		virtual void run_() {}
};
