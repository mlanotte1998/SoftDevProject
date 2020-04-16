#pragma once

#include "../utility-classes/object.h"
#include "../utility-classes/array.h"

/*****************************************************************************
 * A Key is represented by a string value and the index of a home node.
 *************************************************   author: vitekj@me.com */
class Key : public String {
public:
  size_t home_;  // index of the home node

  /** Create a key from a c-string; use the hash value to pick the node */
  Key(char const* cstr) : String(cstr, strlen(cstr)) {
    home_ = hash() % arg.num_nodes;
  }

  Key(Key & from):
    String(from),
    home_(from.home_) {
  }

  /** Create a key from a c-string; explicitly checks the home node */
  Key(char const* cstr, size_t home) : String(cstr, strlen(cstr)) {
    home_ = home;
  }

  /** Create a key from a String; use its hash value to pick the node */    
  Key(String& from) : String(from) { home_ = hash() % arg.num_nodes; }
  
  /** Create a key from a String; explicitly choose the node*/
  Key(String& from, size_t home) : String(from) { home_ = home; }

	Key(String * from):
		String(true, from->steal()) {
		home_ = hash() % arg.num_nodes;
	}
    
  /** Create a key from a String; consumes and deletes the String */
  Key(bool steal, String* from, size_t home) :
    String(steal, from->steal(), from->size()) {
    home_ = home;
    delete from;
  }

  Key(char const * start, size_t length, size_t home):
    String(start, length) {
    home_ = home;
  }

  /** Copy the key */
  Key *clone() override { return new Key(*this, home_); }
  
  /** Return the index of the node where the key is homed. */
  size_t home() { return home_; }

  bool equals(Object * other) override {
    Key * k2 = dynamic_cast<Key *>(other);
    if (k2 == nullptr || k2->home_ != home_) return false;
    return String::equals(other);  
  }
}; // Key
