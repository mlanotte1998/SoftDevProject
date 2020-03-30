#pragma once

#include "../utility-classes/object.h"

/***
* Key::
*
* A Key is an object designed to be used in a key-value store (KVStore, KDStore).
* Every key object has a name and index.
* authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
*/
class Key : public Object {
	public:
		const char* name_;
		size_t idx_;

		/** Key constructor */
		Key(const char* name, size_t idx) {
			name_ = name;
			idx_ = idx;
		}

		/** Method for calculating Key equality */
		bool equals(Key other) {
			return name_ == other.name_ && idx_ == other.idx_;
		}

		/** Returns the index of this Key */
		size_t get_idx() {
			return idx_; 
		}

		/** Method for hashing a Key */
		size_t hash_me() {
			size_t hash = 0;
			for (size_t i = 0; i < strlen(name_); ++i) {
				hash = name_[i] + (hash << 6) + (hash << 16) - hash;
			}
			hash += idx_;
			return hash;
		}
};
