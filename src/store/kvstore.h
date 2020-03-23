#include "key.h"
#include "value.h"
#include "../utility-classes/map.h"

/***
* KVStore::
*
* A KVStore is a basic key-value store.
*/
class KVStore {
public:
    Map* map_;

    KVStore() {
        map_ = new Map();
    }

    ~KVStore() {
        delete map_;
    }

    Value* get(Key key) {
        return dynamic_cast<Value*>(map_->get(&key));
    }

    Value* waitAndGet(Key key) {
        return dynamic_cast<Value*>(map_->get(&key));
    }

    void put(Key key, Value* value) {
        map_->put(&key, value);
    }
};
