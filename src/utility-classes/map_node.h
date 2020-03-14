#include "object.h"

/**
 * Map_Node: A single node used to store a pair in a Map.
 * This node may be part of a linked list if there are collisions in the map
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Map_Node : public Object {

public:
    size_t hash_;
    Object* value_;
    Map_Node* next_;

    // Map_Node constructor
    Map_Node(size_t hash, Object* value) {
        hash_ = hash;
        value_ = value;
        next_ = nullptr;
    }

    // Map_Node destructor
    // deleting an entire linked list is handled by Map
    ~Map_Node() {
        hash_ = 0;
        value_ = nullptr;
        next_ = nullptr;
    }

    // returns the hash value of the key of the pair stored in this node
    size_t get_hash() {
        return hash_;
    }

    // returns the value of the pair stored in this node
    Object* get_value() {
        return value_;
    }

    // replaces the value of the key value pair
    // used when a pair is overwritten by put
    void replace_value(Object* val) {
        value_ = val;
    }

};
