#pragma once

#include "../../utility-classes/array.h"

/**  Item_ are entries in a Map, they are not exposed, are immutable, own
 *   they key, but the value is external.  author: jv */
class Items_ {
public:
  Array keys_; 
  Array vals_; 

  Items_() : keys_(), vals_() {}
  
  Items_(Object *k, Object * v) : keys_(), vals_() {
    keys_.append(k);
    vals_.append(v);
  }

  bool contains_(Object& k) {
    for (int i = 0; i < keys_.size(); i++) 
      if (k.equals(keys_.get(i)))
	  		return true;
    return false;
  }

  Object* get_(Object& k) {
    for (int i = 0; i < keys_.size(); i++) 
			if (k.equals(keys_.get(i)))
				return vals_.get(i);
    return nullptr;
  }

  size_t set_(Object& k, Object* v) {
		for (int i = 0; i < keys_.size(); i++) 
			if (k.equals(keys_.get(i))) {
				vals_.add(i,v);
				return 0;
			}
		// The keys are owned, but the key is received as a reference, i.e. not owned so we must make a copy of it. 
		keys_.append(k.clone());
		vals_.append(v);
		return 1;
  }

  size_t erase_(Object& k) {
		for (int i = 0; i < keys_.size(); i++) 
			if (k.equals(keys_.get(i))) {
				keys_.remove(i);
				vals_.remove(i);
				return 1;
			}
		return 0;
  }
};
