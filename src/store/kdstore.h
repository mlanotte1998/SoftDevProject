#pragma once

#include "key.h"
#include "value.h"
#include "../utility-classes/map.h"

class DataFrame;
class Node;

/***
* KDStore::
*
* A KDStore is a key-value store designed to hold DataFrames.
* authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
*/
class KDStore {
public:
    Map* map_;
    int idx_;
    Node* n_;

    /** KDStore constructor */
    KDStore(size_t idx);

    /** KDStore destructor */
    ~KDStore();

    /** Get Dataframe from KDStore by Kev object */
    DataFrame* get(Key key);

    /** Wait for unblock then get Dataframe from KDStore by Kev object */
    DataFrame* waitAndGet(Key key);

    /** Put key-dataframe pair into KDStore */
    void put(Key key, DataFrame* value);

    void run_network(char* kill_switch);
};

#include "../dataframe/dataframe.h"
#include "../utility-classes/map.h"
#include "../network/network.h"

/** KDStore constructor */
KDStore::KDStore(size_t idx) {
    map_ = new Map();
    idx_ = idx;
    n_ = nullptr;
}

/** KDStore destructor */
KDStore::~KDStore() {
    delete map_;
    if (n_ != nullptr) {
      delete n_;
    }
}

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
      return n_->waitAndGet(key);
      //strcpy(node_ip, "127.0.0.2");
      //n_->ask_other_node_for_dataframe(node_ip, "hello");
    }
}

void KDStore::put(Key key, DataFrame* value) {
    size_t key_idx = key.get_idx();
    if (key_idx == idx_) {
        map_->put(&key, value);
        map_->contains_key(&key);
    } else {
      n_->put(key, value);
    }
}

/** Runs the network */
void KDStore::run_network(char* kill_switch) {
    char rendezvous_ip[10] = {0};
    strcpy(rendezvous_ip, "127.0.0.1");
    if (idx_ == 0) {
      char node_ip[10] = {0};
      strcpy(node_ip, "127.0.0.2");
      n_ = new Node(node_ip, 8081, rendezvous_ip, 5, map_, idx_);
      n_->run(kill_switch);
    } else if (idx_ == 1) {
        char node_ip[10] = {0};
        strcpy(node_ip, "127.0.0.3");
        n_ = new Node(node_ip, 8081, rendezvous_ip, 5, map_, idx_);
        n_->run(kill_switch);
    } else if (idx_ == 2) {
        char node_ip[10] = {0};
        strcpy(node_ip, "127.0.0.4");
        n_ = new Node(node_ip, 8081, rendezvous_ip,  5, map_, idx_);
        n_->run(kill_switch);
    }
}
