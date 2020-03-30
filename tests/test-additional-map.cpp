#include <iostream>
#include "../src/utility-classes/map.h"
#include "../src/utility-classes/string.h"


void FAIL() {   exit(1);    }
void OK(const char* m) { std:: cout << "All test cases in test " << m << " are passed" << std:: endl;}
void t_true(bool p) { if (!p) FAIL(); }
void t_false(bool p) { if (p) FAIL(); }

// test for hashing method of Map object using equality
void test_hash1() {
    Map* h1 = new Map();
    Map* h2 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    String * key_3 = new String("C");
    String * val_3 = new String("3");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    h1->put(key_3, val_3);
    h2->put(key_2, val_2);
    h2->put(key_3, val_3);
    h2->put(key_1, val_1);
    t_true(h1->equals(h2));
    delete key_1;
    delete key_2;
    delete key_3;
    delete val_1;
    delete val_2;
    delete val_3;
    delete h1;
    delete h2;
    OK("Hash");
}

// test for inequality of two Maps containing same objects in different pairings
void test_hash2() {
    Map* h1 = new Map();
    Map* h2 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    String * key_3 = new String("C");
    String * val_3 = new String("3");
    h1->put(key_1, val_1);
    h1->put(key_2, val_3);
    h1->put(key_3, val_2);
    h2->put(key_2, val_2);
    h2->put(key_3, val_3);
    h2->put(key_1, val_1);
    t_false(h1->equals(h2));
    delete key_1;
    delete key_2;
    delete key_3;
    delete val_1;
    delete val_2;
    delete val_3;
    delete h1;
    delete h2;
    OK("Hash 2");
}

// test for explicitly resizing map
// Map will resize automatically, but explicitly calling resize
// should maintain same object consistency
void test_resize() {
    Map* h1 = new Map();
    Map* h2 = new Map();
    String * key_1 = new String("Hello");
    String * val_1 = new String("1");
    String * key_2 = new String("World");
    String * val_2 = new String("2");
    String * key_3 = new String("C");
    String * val_3 = new String("3");
    h1->put(key_1, val_1);
    h1->put(key_2, val_2);
    h1->put(key_3, val_3);
    h2->put(key_2, val_2);
    h2->put(key_3, val_3);
    h2->put(key_1, val_1);
    t_true(h1->equals(h2));
    h1->resize();
    t_false(h1->equals(h2));
    t_true(h1->get(key_1) == val_1);
    t_true(h1->get(key_2) == val_2);
    t_true(h1->get(key_3) == val_3);
    t_true(h1->get_size() == h2->get_size());
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
    delete h2;
    OK("Resize");
}

// testing with two Maps of large sizes to test natural resizing
void test_large() {
    Map* h1 = new Map();
    Map* h2 = new Map();
    String* key_1 = new String("a");
    String* key_2 = new String("b");
    String* key_3 = new String("c");
    String* key_4 = new String("d");
    String* key_5 = new String("e");
    String* key_6 = new String("f");
    String* key_7 = new String("g");
    String* key_8 = new String("h");
    String* key_9 = new String("i");
    String* key_10 = new String("j");
    String* key_11 = new String("k");
    String* key_12 = new String("l");
    String* key_13 = new String("m");
    String* key_14 = new String("n");
    String* key_15 = new String("o");
    String* key_16 = new String("p");
    String* key_17 = new String("q");
    String* key_18 = new String("r");
    String* key_19 = new String("s");
    String* key_20 = new String("t");
    String* key_21 = new String("u");
    String* key_22 = new String("v");
    String* key_23 = new String("w");
    String* key_24 = new String("x");
    String* key_25 = new String("y");
    String* key_26 = new String("z");
    String* key_27 = new String("aa");
    String* key_28 = new String("ab");
    String* key_29 = new String("ac");
    String* key_30 = new String("ad");
    String* key_31 = new String("ae");
    String* key_32 = new String("af");
    String* key_33 = new String("ag");
    String* key_34 = new String("ah");
    String* key_35 = new String("ai");
    String* val_1 = new String("Hello World");

    h1->put(key_1, val_1);
    h1->put(key_2, val_1);
    h1->put(key_3, val_1);
    h1->put(key_4, val_1);
    h1->put(key_5, val_1);
    h1->put(key_6, val_1);
    h1->put(key_7, val_1);
    h1->put(key_8, val_1);
    h1->put(key_9, val_1);
    h1->put(key_10, val_1);
    h1->put(key_11, val_1);
    h1->put(key_12, val_1);
    h1->put(key_13, val_1);
    h1->put(key_14, val_1);
    h1->put(key_15, val_1);
    h1->put(key_16, val_1);
    h1->put(key_17, val_1);
    h1->put(key_18, val_1);
    h1->put(key_19, val_1);
    h1->put(key_20, val_1);
    h1->put(key_21, val_1);
    h1->put(key_22, val_1);
    h1->put(key_23, val_1);
    h1->put(key_24, val_1);
    h1->put(key_25, val_1);
    h1->put(key_26, val_1);
    h1->put(key_27, val_1);
    h1->put(key_28, val_1);
    h1->put(key_29, val_1);
    h1->put(key_30, val_1);
    h1->put(key_31, val_1);
    h1->put(key_32, val_1);
    h1->put(key_33, val_1);
    h1->put(key_34, val_1);
    h1->put(key_35, val_1);

    h2->put(key_1, val_1);
    h2->put(key_2, val_1);
    h2->put(key_3, val_1);
    h2->put(key_4, val_1);
    h2->put(key_5, val_1);
    h2->put(key_6, val_1);
    h2->put(key_7, val_1);
    h2->put(key_8, val_1);
    h2->put(key_9, val_1);
    h2->put(key_10, val_1);
    h2->put(key_11, val_1);
    h2->put(key_12, val_1);
    h2->put(key_13, val_1);
    h2->put(key_14, val_1);
    h2->put(key_15, val_1);
    h2->put(key_16, val_1);
    h2->put(key_17, val_1);
    h2->put(key_18, val_1);
    h2->put(key_19, val_1);
    h2->put(key_20, val_1);
    h2->put(key_21, val_1);
    h2->put(key_22, val_1);
    h2->put(key_23, val_1);
    h2->put(key_24, val_1);
    h2->put(key_25, val_1);
    h2->put(key_26, val_1);
    h2->put(key_27, val_1);
    h2->put(key_28, val_1);
    h2->put(key_29, val_1);
    h2->put(key_30, val_1);
    h2->put(key_31, val_1);
    h2->put(key_32, val_1);
    h2->put(key_33, val_1);
    h2->put(key_34, val_1);
    h2->put(key_35, val_1);

    t_true(h1->equals(h2));
    t_true(h1->get_size() == 35);
    t_true(h1->bucket_size_ == 64);
    t_true(h2->get_size() == 35);
    t_true(h2->bucket_size_ == 64);

    delete val_1;
    delete key_1;
    delete key_2;
    delete key_3;
    delete key_4;
    delete key_5;
    delete key_6;
    delete key_7;
    delete key_8;
    delete key_9;
    delete key_10;
    delete key_11;
    delete key_12;
    delete key_13;
    delete key_14;
    delete key_15;
    delete key_16;
    delete key_17;
    delete key_18;
    delete key_19;
    delete key_20;
    delete key_21;
    delete key_22;
    delete key_23;
    delete key_24;
    delete key_25;
    delete key_26;
    delete key_27;
    delete key_28;
    delete key_29;
    delete key_30;
    delete key_31;
    delete key_32;
    delete key_33;
    delete key_34;
    delete key_35;
    delete h1;
    delete h2;

    OK("Large");
}

// main function
int main() {
    test_hash1();
    test_hash2();
    test_resize();
    test_large();
    return 0;
}