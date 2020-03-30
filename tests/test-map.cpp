#include <iostream>
#include "../src/utility-classes/map.h"
#include "../src/utility-classes/string.h"


void FAIL() {   exit(1);    }
void OK(const char* m) { std:: cout << "All test cases in test " << m << " are passed" << std:: endl;}
void t_true(bool p) { if (!p) FAIL(); }
void t_false(bool p) { if (p) FAIL(); }


/**
 * test cases for put()
 * program won't error when objects are put into a map
 */
void test1() {
    Map* h1 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    delete key_1;
    delete key_2;
    delete val_1;
    delete val_2;
    delete h1;
    OK("1");
}

/**
 * test cases for put() and get_size() method when using the same key
 *  the map's size remains the same, the previous value should be overwritten
 */
void test2() {
    Map* h1 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * val_2 = new String("2");
    h1->put(key_1, val_1);
    t_true(h1->get_size() == 1);
    h1->put(key_1, val_2);
    t_true(h1->get_size() == 1);
    t_false(val_1 -> equals(h1->get(key_1)));
    delete key_1; 
    delete val_1;
    delete val_2;
    delete h1;
    OK("2");
}

/**
 * test cases for get() methods
 * comparing value extracting from Map with input value
 */
void test3() {
    Map* h1 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    t_true(h1->get(key_1)->equals(val_1));
    t_true(h1->get(key_2)->equals(val_2));
    delete key_1;
    delete key_2;
    delete val_1;
    delete val_2;
    delete h1;
    OK("3");
}

/**
 * get() returns nullptr if there's no mapping for the given key in the map
 */
void test4() {
    Map* h1 = new Map();
    t_true(h1 -> get(new String("1")) == nullptr);
    delete h1;
    OK("4");
}


/**
 * test cases for remove() method
 * remove returns the value mapped to the given key if the key exists in the map
 * remove returns nullptr if the key does not exist
 */
void test5() {
    Map* h1 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    t_true(h1->remove(key_1)->equals(val_1));
    t_false(h1->remove(key_2)->equals(val_1));
    t_true(h1->remove(key_1) == nullptr);
    delete key_1;
    delete key_2;
    delete val_1;
    delete val_2;
    delete h1;
    OK("5");
}

/**
 * test cases for contains_key() method
 * contains_key returns true/false if the key exist/not exist in the map
 */
void test6() {
    Map* h1 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    String * key_3 = new String("NEU");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    t_true(h1->contains_key(key_1));
    t_true(h1->contains_key(key_2));
    t_false(h1->contains_key(key_3));
    delete key_1;
    delete key_2;
    delete val_1;
    delete val_2;
    delete key_3;
    delete h1;
    OK("6");
}


/**
 * test cases values() function
 * testing on values() function that return all values that exist in the Map
 */
void test7() {
    Map* h1 = new Map();
    String * key_1 = new String("A");
    String * val_1 = new String("1");
    String * key_2 = new String("B");
    String * val_2 = new String("2");
    String * key_3 = new String("C");
    String * val_3 = new String("3");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    h1->put(key_3, val_3);
    Object** values_array;
    values_array = h1->values();
    t_false(values_array == nullptr);
    for (int i=0; i< 3; i++) {
        t_true(val_1 -> equals(values_array[i]) || val_2 -> equals(values_array[i]) || val_3 -> equals(values_array[i]));
    }
    delete key_1;
    delete key_2;
    delete key_3;
    delete val_1;
    delete val_2;
    delete val_3;
    delete h1;
    OK("7");
}

// main function
int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    return 0;
}
