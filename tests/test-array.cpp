#include <stdio.h>
#include "../src/utility-classes/array.h"
#include "../src/utility-classes/string.h"

void t_true(bool result) {
    if (result) {

    } else {
        printf("FAILED\n");
    }
}

void t_false(bool result) {
    if (!result) {
    } else {
        printf("FAILED\n");
    }
}

void OK(const char* m) { std:: cout << "All test cases in test " << m << " are passed" << std:: endl;}

/**
* Determine if these two floats are equal with respect to eps.
* @param f1 the first float to compare.
* @param f2 the second float to compare.
*/
bool float_equal(float f1, float f2) {
    float eps = 0.0000001;
    if (f1 > f2) {
        return f1 - f2 < eps;
    } else {
        return f2 - f1 < eps;
    }
}

void test_append() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    t_true(l1->get(0)->equals(s) && l1->get(1)->equals(t) && l1->size() == 2);

    IntArray *l2 = new IntArray();
    l2->append(3);
    l2->append(4);
    t_true(l2->get(0) == 3 && l2->get(1) == 4 && l2->size() == 2);

    float u = -3.3;
    float v = 4.4;
    FloatArray *l3 = new FloatArray();
    l3->append(u);
    l3->append(v);
    t_true(float_equal(l3->get(0), -3.3) && float_equal(l3->get(1), 4.4) && l3->size() == 2);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->append(false);
    t_true(l4->get(0) && !l4->get(1) && l4->size() == 2);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Append");
}

void test_append_array() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    Array *l2 = new Array();
    l2->append_array(l1);
    t_true(l2->equals(l1));

    IntArray *l3 = new IntArray();
    l3->append(1);
    l3->append(2);
    IntArray *l4 = new IntArray();
    l4->append_array(l3);
    t_true(l4->equals(l3));

    float u = -2.2;
    float v = 1.0;
    FloatArray *l5 = new FloatArray();
    l5->append(u);
    l5->append(v);
    FloatArray *l6 = new FloatArray();
    l6->append_array(l5);
    t_true(l6->equals(l5));

    BoolArray *l7 = new BoolArray();
    l7->append(true);
    l7->append(false);
    BoolArray *l8 = new BoolArray();
    l8->append_array(l7);
    t_true(l8->equals(l7));

    Array *l10 = new Array();
    String *s2 = new String("Hi");
    String *t2 = new String("Mom");
    l10->append(s2);
    l10->append(t2);
    l1->append_array(l10);
    t_true(l1->get(2)->equals(s2) && l1->get(3)->equals(t2) && l1->size() == 4);

    IntArray *l11 = new IntArray();
    l11->append(4);
    l11->append(5);
    l3->append_array(l11);
    t_true(l3->get(2) == 4 && l3->get(3) == 5 && l3->size() == 4);

    FloatArray *l12 = new FloatArray();
    float f1 = 3.7;
    float f2 = 10.5;
    l12->append(f1);
    l12->append(f2);
    l5->append_array(l12);
    t_true(float_equal(l5->get(2), 3.7) && (l5->get(3), 10.5) && l5->size() == 4);

    BoolArray *l13 = new BoolArray();
    l13->append(true);
    l13->append(true);
    l7->append_array(l13);
    t_true(l7->get(2) && l7->get(3) && l7->size() == 4);

    delete s;
    delete t;
    delete s2;
    delete t2;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;
    delete l6;
    delete l7;
    delete l8;
    delete l10;
    delete l11;
    delete l12;
    delete l13;
    OK("Test AppendArray");
}

void test_add() {
    Array *l1 = new Array();
    String *s = new String("Hello");
    String *t = new String("World");
    String *u = new String("Hi");
    l1->append(s);
    l1->add(0, t);
    t_true(l1->get(0)->equals(t) && l1->get(1)->equals(s) && l1->size() == 2);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);
    t_true(l2->get(0) == 13 && l2->get(1) == 6 && l2->size() == 2);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);
    t_true(float_equal(l3->get(0), 7.8) && float_equal(l3->get(1), 5.6) && l3->size() == 2);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->add(0, false);
    t_true(!l4->get(0) && l4->get(1) && l4->size() == 2);

    delete s;
    delete t;
    delete u;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Add");
}

void test_clear() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    l1->clear();
    t_true(l1->size() == 0);

    l1->clear();
    t_true(l1->size() == 0);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);
    l2->clear();
    t_true(l2->size() == 0);

    l2->clear();
    t_true(l2->size() == 0);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);
    l3->clear();
    t_true(l3->size() == 0);

    l3->clear();
    t_true(l3->size() == 0);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->add(0, false);
    l4->clear();
    t_true(l4->size() == 0);

    l4->clear();
    t_true(l4->size() == 0);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Clear");
}

void test_get() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->add(0, false);

    t_true(l1->get(1)->equals(t));

    t_true(l2->get(1) == 6);

    t_true(float_equal(l3->get(1), 5.6));

    t_false(l4->get(0));

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Get");
}

void test_index_of() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);

    BoolArray *l4 = new BoolArray();
    l4->append(true);

    String *hello = new String("Hello");
    String *notIn = new String("NotIn");
    t_true(l1->index_of(hello) == 0);

    t_true(l1->index_of(notIn) == 3);

    t_true(l2->index_of(13) == 0);

    t_true(l2->index_of(7) == 3);

    t_true(l3->index_of(7.8) == 0);

    t_true(l3->index_of(7) == 3);

    t_true(l4->index_of(true) == 0);

    t_true(l4->index_of(false) == 2);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete hello;
    delete notIn;
    OK("Test Index of");
}

void test_remove() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->append(false);

    t_true(l1->remove(0)->equals(s) && l1->size() == 1);

    l1->remove(-1);
    t_true(l1->size() == 1);

    l1->remove(7);
    t_true(l1->size() == 1);

    t_true(l2->remove(0) == 13 && l2->size() == 1);

    l2->remove(-1);
    t_true(l2->size() == 1);

    l2->remove(7);
    t_true(l2->size() == 1);

    t_true(float_equal(l3->remove(0), 7.8) && l3->size() == 1);

    l3->remove(-1);
    t_true(l3->size() == 1);

    l3->remove(7);
    t_true(l3->size() == 1);

    t_true(l4->remove(0) && l4->size() == 1);

    l4->remove(-1);
    t_true(l4->size() == 1);

    l4->remove(7);
    t_true(l4->size() == 1);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Remove");
}

void test_set() {
    String *s = new String("Hello");
    String *t = new String("World");
    String *u = new String("Hi");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);

    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);

    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    float h = 3.4;
    l3->append(f);
    l3->add(0, g);

    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->append(false);

    t_true(l1->set(-1, u)->equals(u));

    t_true(l1->set(5, u)->equals(u));

    t_true(l1->set(1, u)->equals(t) && l1->get(1)->equals(u));

    t_true(l2->set(-1, 3) == 3);

    t_true(l2->set(5, 9) == 9);

    t_true(l2->set(1, 9) == 6 && l2->get(1) == 9);

    t_true(l3->set(-1, h) == h);

    t_true(l3->set(5, h) == h);

    t_true(float_equal(l3->set(1, h), 5.6) && l3->get(1) == h);

    t_true(l4->set(-1, true));

    t_true(l4->set(5, true));

    t_true(!l4->set(1, true) && l4->get(1));

    delete s;
    delete t;
    delete u;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Set");
}

void test_size() {
    String *s = new String("Hello");
    float f = 5.6;
    Array *l1 = new Array();
    IntArray *l2 = new IntArray();
    FloatArray *l3 = new FloatArray();
    BoolArray *l4 = new BoolArray();

    t_true(l1->size() == 0);

    l1->append(s);
    t_true(l1->size() == 1);

    t_true(l2->size() == 0);

    l2->append(5);
    t_true(l2->size() == 1);

    t_true(l3->size() == 0);

    l3->append(f);
    t_true(l3->size() == 1);

    t_true(l4->size() == 0);

    l4->append(true);
    t_true(l4->size() == 1);

    delete s;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    OK("Test Size");
}

void test_hash() {
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    Array *l2 = new Array();
    l2->append_array(l1);
    t_true(l1->hash() == l2->hash());

    IntArray *l3 = new IntArray();
    l3->append(1);
    l3->append(2);
    IntArray *l4 = new IntArray();
    l4->append_array(l3);
    t_true(l3->hash() == l4->hash());

    float u = -2.2;
    float v = 1.0;
    FloatArray *l5 = new FloatArray();
    l5->append(u);
    l5->append(v);
    FloatArray *l6 = new FloatArray();
    l6->append_array(l5);
    t_true(l5->hash() == l6->hash());

    BoolArray *l7 = new BoolArray();
    l7->append(true);
    l7->append(false);
    BoolArray *l8 = new BoolArray();
    l8->append_array(l7);
    t_true(l7->hash() == l8->hash());

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete l5;
    delete l6;
    delete l7;
    delete l8;
    OK("Test Hash");
}

int main() {
    test_add();
    test_append();
    test_append_array();
    test_clear();
    test_get();
    test_hash();
    test_index_of();
    test_remove();
    test_set();
    test_size();
    return 0;
}
