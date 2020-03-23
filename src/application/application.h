#include <stddef.h>
#include "../store/kdstore.h"

class Application {
	public:
		size_t idx_;
		KDStore kv;

		Application(size_t idx) {
			idx_ = idx;
		}

		~Application() {
			
		}

		virtual void run_() {}
};
