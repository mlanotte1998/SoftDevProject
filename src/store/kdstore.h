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
    Node* n_;

    KDStore(size_t idx) {
        map_ = new Map();
        idx_ = idx;
        n_ = nullptr;
    }

    ~KDStore() {
        delete map_;
        if (n_ != nullptr) {
          delete n_;
        }
    }

    void run_network(char* kill_switch) {
        char rendezvous_ip[10] = {0};
        strcpy(rendezvous_ip, "127.0.0.1");
        if (idx_ == 0) {
          char node_ip[10] = {0};
          strcpy(node_ip, "127.0.0.2");
          n_ = new Node(node_ip, 8080, rendezvous_ip, 5, map_);
          n_->run(kill_switch);
        } else if (idx_ == 1) {
            char node_ip[10] = {0};
            strcpy(node_ip, "127.0.0.3");
            n_ = new Node(node_ip, 8080, rendezvous_ip, 5, map_);
            n_->run(kill_switch);
        } else if (idx_ == 2) {
            char node_ip[10] = {0};
            strcpy(node_ip, "127.0.0.4");
            n_ = new Node(node_ip, 8080, rendezvous_ip,  5, map_);
            n_->run(kill_switch);
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

    size_t key_idx = key.get_idx();
    char node_ip[10] = {0};

    if (key_idx == idx_) {
      return dynamic_cast<DataFrame*>(map_->get(&key));
    } else if (key_idx == 0){
      while(n_ == nullptr) {
        // do nothing but wait
      }
      std::cout << "cool" << std::endl; 
      strcpy(node_ip, "127.0.0.2");
      n_->ask_other_node_for_dataframe(node_ip, "hello");
      return nullptr;
    }
}


void KDStore::put(Key key, DataFrame* value) {
    map_->put(&key, value);
}
