#include "../../dataframe/reader.h"
#include "simap.h"

/***
* Adder::
*
* Adder is a visitor class that is used to visit rows in a dataframe,
* adding values to an SIMap.
*/
/* author: jv */
class Adder : public Reader {
public:
  SIMap& map_;  // String to Num map;  Num holds an int
 
  // Adder constructor
  Adder(SIMap& map) : map_(map)  {}
 
  // visits a row and modifies the map accordingly
  bool visit(Row& r) override {
    String* word = r.get_string(0);
    assert(word != nullptr);
    Num* num = map_.contains(*word) ? map_.get(*word) : new Num();
    assert(num != nullptr);
    num->v++;
    map_.set(*word, num);
    return false;
  }
};
