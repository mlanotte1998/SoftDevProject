#include "key.h"
#include "value.h"
#include "../utility-classes/map.h"

/***
* KVStore::
*
* A KVStore is a basic key-value store.
* authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
*/
class KVStore {
public:
    Map* map_;

    /** KVStore constructor */
    KVStore() {
        map_ = new Map();
    }

    /** KVStore destructor */
    ~KVStore() {
        delete map_;
    }

    /** Get a Value object from the KVStore by a Key object */
    Value* get(Key key) {
        return dynamic_cast<Value*>(map_->get(&key));
    }

    /** Wait for unblock then get a Value object from the KVStore by a Key object */
    Value* waitAndGet(Key key) {
        return dynamic_cast<Value*>(map_->get(&key));
    }

    /** Put key-value pair into KVStore */
    void put(Key key, Value* value) {
        map_->put(&key, value);
    }
};
