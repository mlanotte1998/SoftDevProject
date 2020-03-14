//
// Created by Jiawen Liu on 1/24/20.
//

#ifndef MAP_H
#define MAP_H

#pragma once
#include "object.h"
#include "map_node.h"

/**
 * Map
 * author: Jiawen Liu
 */
class Map : public Object {
public:
    int bucket_size_ = 16;
    float load_factor = .75;
    int pair_size;
    Map_Node** map_;
    size_t hash_;

    /**
     * Default constructor that constructs an empty Map with
     * the default initial capacity 16
    */
    Map() {
        pair_size = 0;
        map_ = new Map_Node*[bucket_size_];
        hash_ = 0;
        for (int i = 0; i < bucket_size_; i++) {
            map_[i] = nullptr;
        }
    }

    /**
     * Destructor that delete Map object
     */
    ~Map() {
        pair_size = 0;
        hash_ = 0;
        // for each index in bucket delete entire linked list
        for (int i = 0; i < bucket_size_; i++) {
            // delete all pairs at index
            if (map_[i] != nullptr) {
                // iterate through linked list deleting each node
                Map_Node* list_head = map_[i];
                Map_Node* temp = nullptr;
                while(list_head != nullptr) {
                    temp = list_head;
                    list_head = list_head->next_;
                    delete temp;
                }
                map_[i] = nullptr;
            }
        }
        delete[] map_;
        map_ = nullptr;
    }

    /**
     * Returns the number of key-value pairs in this map.
     */
    int get_size() {
        return pair_size;
    }

    /**
     * Put the given key value pair into the map
     * If the key is already associated with a value, the new value should overwrite the previous one
     * @return  val
     */
    void put(Object* key, Object* val) {
        // hash the key object to add to hash pair
        size_t key_hash = key->hash();
        // add key value pair to the map using helper method
        put_hash(key_hash, val);
        // resize the map if adding this pair achieves the pair size bound
        if (pair_size >= bucket_size_ * load_factor) {
            resize();
        }
    }


    /**
     * Returns the value of which the specified key is mapped to, or nullptr if this map does not contain the given key
     * @param key   the key whose associated value is to be returned
     * @return  the value mapped to the given key, or nullptr if the key is not found
     */
    Object* get(Object* key) {
        // find the index where this key would be stored based on its hash
        size_t key_hash = key->hash();
        int key_idx = key_hash % bucket_size_;
        // if data exists at this index search for key hash in linked list
        Map_Node* bucket_value = map_[key_idx];
        if (bucket_value) {
            Map_Node* iterator = bucket_value;
            while (iterator != nullptr) {
                // if hash is found return associated value
                if (iterator->hash_ == key_hash) return iterator->get_value();
                iterator = iterator->next_;
            }
        }
        return nullptr;
    }


    /**
     * Returns true if this map contains the given key
     * @param key   The key whose presence in this map is to be tested
     * @return  true if this map contains a mapping for the specified key, otherwise false
     */
    bool contains_key(Object* key) {
        // find the index where this key would be stored based on its hash
        size_t key_hash = key->hash();
        int key_idx = key_hash % bucket_size_;
        // if data exists at this index search for key hash in linked list
        Map_Node* bucket_value = map_[key_idx];
        if (bucket_value) {
            Map_Node* iterator = bucket_value;
            while (iterator != nullptr) {
                // if hash is found return true
                if (iterator->hash_ == key_hash) return true;
                iterator = iterator->next_;
            }
        }
        return false;
    }


    /**
     * Removes the mapping for the specified key from this map if present.
     * @param key
     * @return   value associated with the key, or nullptr if the key is not found
     */
    Object* remove(Object* key) {
        // variables to store the node and object to be removed if found
        Map_Node* target = nullptr;
        Object* return_val = nullptr;
        // find the index where this key would be stored based on its hash
        size_t key_hash = key->hash();
        int key_idx = key_hash % bucket_size_;
        // if data exists at this index search for key hash in linked list
        Map_Node* bucket_value = map_[key_idx];
        if (bucket_value) {
            // head of the list contains the target value
            // next element in list becomes new head (likely null)
            if (bucket_value->hash_ == key_hash) {
                target = bucket_value;
                map_[key_idx] = bucket_value->next_;
            } 
            // search the rest of the list for the target and remove it from list
            else {
                Map_Node* iterator = bucket_value;
                Map_Node* prev = nullptr;
                while (iterator != nullptr && iterator->hash_ != key_hash) {
                    prev = iterator;
                    iterator = iterator->next_;
                }
                if (iterator) {
                    prev->next_ = iterator->next_;
                    target = iterator;
                }
            }
        }
        // if the pair with given key is found return the object value and delete the node
        if (target) {
            return_val = target->get_value();
            delete target;
            pair_size--;
        }
        return return_val;
    }

    /**
     * @return a list of values contained in this map
     */
    Object** values() {
        int index = 0;
        // array of Object* to store all values
        Object** vals = new Object*[pair_size];
        // for each index in bucket add every value in linked list
        for (int i = 0; i < bucket_size_; i++) {
            Map_Node* node = map_[i];
            // iterate through linked list adding each node's value
            if (node != nullptr) {
                Map_Node* iterator = node;
                while (iterator != nullptr) {
                    vals[index] = iterator->get_value();
                    index++;
                    iterator = iterator->next_;
                }
            }
        }
        return vals;
    }

    /**
     * calculate hash value for this map and store it as hash_
     */
    size_t hash() {
        // variables for storing the hash of one index of map and entire map hash
        size_t total_hash = 0;
        size_t index_hash = 0;
        // iterate through each index in bucket
        for (size_t i = 0; i < bucket_size_; i++) {
            Map_Node* node = map_[i];
            // iterate through linked list
            if (node != nullptr) {
                Map_Node* iterator = node;
                while (iterator != nullptr) {
                    // build the index hash by adding the key hash and
                    // value hash at each node in the linked list
                    index_hash += iterator->get_hash();
                    index_hash += iterator->get_value()->hash();
                    iterator = iterator->next_;
                }
            }
            // add index hash multiplied by index to total hash
            total_hash += (index_hash * (i + 1));
            index_hash = 0;
        }
        // assign hash_ to hash built from function and return hash
        hash_ = total_hash;
        return hash_;
    }

    /**
     * determines whether two maps are equal based on their hashes
     */
    bool equals(Object* object) {
        // if other object is a map compare this hash with other's hash
        if (dynamic_cast<Map *>(object) != nullptr) {
            Map *other = dynamic_cast<Map *>(object);
            if (other->pair_size == pair_size) {
                return other->hash() == hash();
            }
        }
        return false;
    }

    //######## Helper Methods for API ########

    /**
     * helper method for put and resize putting an object in map using
     * hash value rather than key
     */
    void put_hash(size_t key_hash, Object* val) {
        // calculate the index where this key should be stored based on its hash
        int key_idx = key_hash % bucket_size_;
        Map_Node* bucket_value = map_[key_idx];
        // if values already exist at hash index add to values
        if (bucket_value) {
            Map_Node* iterator = bucket_value;
            // check if key_val already exists in list
            while (iterator->next_ != nullptr && iterator->hash_ != key_hash) {
                iterator = iterator->next_;
            }
            // if it does replace its value
            if (iterator->hash_ == key_hash) {
                iterator->replace_value(val);
            } 
            // if it does not add it to end of the list
            else {
                iterator->next_ = new Map_Node(key_hash, val);
                pair_size++;
            }
        }
        // if nothing exists at hash index at the object as head of list
        else {
            map_[key_idx] = new Map_Node(key_hash, val);
            pair_size++;
        }
        // resize the map if adding this pair achieves the pair size bound
        if (pair_size >= bucket_size_ * load_factor) {
            resize();
        }
    }

    /**
     * helper method for resizing map once pair size bound is reached
     */
    void resize() {
        // increase the bucket size twofold
        bucket_size_ = bucket_size_ * 2;
        // existing map of old size and new map of increased size
        Map_Node** old_map = map_;
        Map_Node** grow_map = new Map_Node*[bucket_size_];
        // zero out new map
        for (int i = 0; i < bucket_size_; i++) {
            grow_map[i] = nullptr;
        }
        // assign map_ to new map of larger size
        map_ = grow_map;
        pair_size = 0;
        // iterate through every pair in existing map
        for (size_t i = 0; i < bucket_size_ / 2; i++) {
            Map_Node* node = old_map[i];
            if (node != nullptr) {
                Map_Node* head = node;
                Map_Node* temp = nullptr;
                while (head != nullptr) {
                    // rehash each key value pair to fit in new bucket
                    temp = head;
                    put_hash(head->get_hash(), head->get_value());
                    head = head->next_;
                    // delete the old node
                    delete temp;
                }
            }
        }
        delete[] old_map;
    }

};
#endif
