#pragma once

#include "key.h"
#include "value.h"
#include "../utility-classes/map.h"

class DataFrame;

/***
* KDStore::
*
* A KDStore is a key-value store designed to hold DataFrames.
*/
class KDStore {
public:
    Map* map_;

    KDStore() {
        map_ = new Map();
    }

    ~KDStore() {
        delete map_;
    }

    DataFrame* get(Key key);

    DataFrame* waitAndGet(Key key);

    void put(Key key, DataFrame* value);
};

#include "../dataframe/dataframe.h"

DataFrame* KDStore::get(Key key) {
    return dynamic_cast<DataFrame*>(map_->get(&key));
}

DataFrame* KDStore::waitAndGet(Key key) {
    return dynamic_cast<DataFrame*>(map_->get(&key));
}

void KDStore::put(Key key, DataFrame* value) {
    map_->put(&key, value);
}
