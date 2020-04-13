#include "map.h"
#include "num.h"

/* author: jv */
class SIMap : public Map {
public:
  SIMap () {}
  Num* get(String& key) { return dynamic_cast<Num*>(get_(key)); }
  void set(String& k, Num* v) { assert(v); Map::set(k, v); }
}; // KVMap
