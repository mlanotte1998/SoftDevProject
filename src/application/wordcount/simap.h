#pragma once

#include "wc_map.h"
#include "num.h"

/* author: jv */
class SIMap : public WordCountMap {
public:
  SIMap(): WordCountMap() {}
  Num* get(String& key) { return dynamic_cast<Num*>(get_(key)); }
  void set(String& k, Num* v) { assert(v); WordCountMap::set(k, v); }
}; // KVMap
