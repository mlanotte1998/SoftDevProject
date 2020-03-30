#pragma once

#include "key.h"
#include "value.h"
#include "../utility-classes/map.h"
#include "../network/network.h"

class DataFrame;

/***
* KDStore::
*
* A KDStore is a key-value store designed to hold DataFrames.
*/
class KDStore {
public:
    Map* map_;
    int idx_;


    KDStore() {
        map_ = new Map();
    }

    KDStore(size_t idx) {
        map_ = new Map();
        idx_ = idx;
    }

    ~KDStore() {
        delete map_;
    }

    void run_network(char* kill_switch) {
        char rendezvous_ip[10] = {0};
        strcpy(rendezvous_ip, "127.0.0.1");
        if (idx_ == 0) {
            RendezvousServer r(rendezvous_ip, 8080, 5);
            r.run(kill_switch);
        } else if (idx_ == 1) {
            char node_ip[10] = {0};
            strcpy(node_ip, "127.0.0.2");
            Node n(node_ip, 8080, rendezvous_ip, 5);
            n.run(kill_switch);
        } else if (idx_ == 2) {
            char node_ip[10] = {0};
            strcpy(node_ip, "127.0.0.3");
            Node n(node_ip, 8080, rendezvous_ip,  5);
            n.run(kill_switch);
        }

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
