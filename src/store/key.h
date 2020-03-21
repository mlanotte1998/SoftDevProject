#pragma once

#include "../utility-classes/object.h"

class Key : public Object {
	public:
		const char* name_;
		size_t idx_;

		Key(const char* name, size_t idx) {
			name_ = name;
			idx_ = idx;
		}
};
