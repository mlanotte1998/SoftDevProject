#include <stdio.h>
#include "array.h"
#include "string.h"

void t_true(bool result) {
    if (result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
}

void t_false(bool result) {
    if (!result) {
        printf("PASSED\n");
    } else {
        printf("FAILED\n");
    }
}

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
    printf("Test append for string: ");
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    t_true(l1->get(0)->equals(s) && l1->get(1)->equals(t) && l1->size() == 2);

    printf("Test append for int: ");
    IntArray *l2 = new IntArray();
    l2->append(3);
    l2->append(4);
    t_true(l2->get(0) == 3 && l2->get(1) == 4 && l2->size() == 2);

    printf("Test append for float: ");
    float u = -3.3;
    float v = 4.4;
    FloatArray *l3 = new FloatArray();
    l3->append(u);
    l3->append(v);
    t_true(float_equal(l3->get(0), -3.3) && float_equal(l3->get(1), 4.4) && l3->size() == 2);

    printf("Test append for bool: ");
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
}

void test_append_array() {
    printf("Test append_array for string (empty): ");
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    Array *l2 = new Array();
    l2->append_array(l1);
    t_true(l2->equals(l1));

    printf("Test append_array for int (empty): ");
    IntArray *l3 = new IntArray();
    l3->append(1);
    l3->append(2);
    IntArray *l4 = new IntArray();
    l4->append_array(l3);
    t_true(l4->equals(l3));

    printf("Test append_array for float (empty): ");
    float u = -2.2;
    float v = 1.0;
    FloatArray *l5 = new FloatArray();
    l5->append(u);
    l5->append(v);
    FloatArray *l6 = new FloatArray();
    l6->append_array(l5);
    t_true(l6->equals(l5));

    printf("Test append_array for bool (empty): ");
    BoolArray *l7 = new BoolArray();
    l7->append(true);
    l7->append(false);
    BoolArray *l8 = new BoolArray();
    l8->append_array(l7);
    t_true(l8->equals(l7));

    printf("Test append_array for string (not empty): ");
    Array *l10 = new Array();
    String *s2 = new String("Hi");
    String *t2 = new String("Mom");
    l10->append(s2);
    l10->append(t2);
    l1->append_array(l10);
    t_true(l1->get(2)->equals(s2) && l1->get(3)->equals(t2) && l1->size() == 4);

    printf("Test append_array for int (not empty): ");
    IntArray *l11 = new IntArray();
    l11->append(4);
    l11->append(5);
    l3->append_array(l11);
    t_true(l3->get(2) == 4 && l3->get(3) == 5 && l3->size() == 4);

    printf("Test append_array for float (not empty): ");
    FloatArray *l12 = new FloatArray();
    float f1 = 3.7;
    float f2 = 10.5;
    l12->append(f1);
    l12->append(f2);
    l5->append_array(l12);
    t_true(float_equal(l5->get(2), 3.7) && (l5->get(3), 10.5) && l5->size() == 4);

    printf("Test append_array for bool (not empty): ");
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
}

void test_add() {
    printf("Test add for string: ");
    Array *l1 = new Array();
    String *s = new String("Hello");
    String *t = new String("World");
    String *u = new String("Hi");
    l1->append(s);
    l1->add(0, t);
    t_true(l1->get(0)->equals(t) && l1->get(1)->equals(s) && l1->size() == 2);

    printf("Test add for int: ");
    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);
    t_true(l2->get(0) == 13 && l2->get(1) == 6 && l2->size() == 2);

    printf("Test add for float: ");
    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);
    t_true(float_equal(l3->get(0), 7.8) && float_equal(l3->get(1), 5.6) && l3->size() == 2);

    printf("Test add for bool: ");
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
}

void test_clear() {
    printf("Test clear for string (not empty): ");
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    l1->clear();
    t_true(l1->size() == 0);

    printf("Test clear for string (empty): ");
    l1->clear();
    t_true(l1->size() == 0);

    printf("Test clear for int (not empty): ");
    IntArray *l2 = new IntArray();
    l2->append(6);
    l2->add(0, 13);
    l2->clear();
    t_true(l2->size() == 0);

    printf("Test clear for int (empty): ");
    l2->clear();
    t_true(l2->size() == 0);

    printf("Test clear for float (not empty): ");
    FloatArray *l3 = new FloatArray();
    float f = 5.6;
    float g = 7.8;
    l3->append(f);
    l3->add(0, g);
    l3->clear();
    t_true(l3->size() == 0);

    printf("Test clear for float (empty): ");
    l3->clear();
    t_true(l3->size() == 0);

    printf("Test clear for bool (not empty): ");
    BoolArray *l4 = new BoolArray();
    l4->append(true);
    l4->add(0, false);
    l4->clear();
    t_true(l4->size() == 0);

    printf("Test clear for bool (empty): ");
    l4->clear();
    t_true(l4->size() == 0);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
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

    printf("Test get for string: ");
    t_true(l1->get(1)->equals(t));

    printf("Test get for int: ");
    t_true(l2->get(1) == 6);

    printf("Test get for float: ");
    t_true(float_equal(l3->get(1), 5.6));

    printf("Test get for bool: ");
    t_false(l4->get(0));

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
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
    printf("Test index_of for string (in array): ");
    t_true(l1->index_of(hello) == 0);

    printf("Test index_of for string (not in array): ");
    t_true(l1->index_of(notIn) == 3);

    printf("Test index_of for int (in array): ");
    t_true(l2->index_of(13) == 0);

    printf("Test index_of for int (not in array): ");
    t_true(l2->index_of(7) == 3);

    printf("Test index_of for float (in array): ");
    t_true(l3->index_of(7.8) == 0);

    printf("Test index_of for float (not in array): ");
    t_true(l3->index_of(7) == 3);

    printf("Test index_of for bool (in array): ");
    t_true(l4->index_of(true) == 0);

    printf("Test index_of for bool (not in array): ");
    t_true(l4->index_of(false) == 2);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
    delete hello;
    delete notIn;
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

    printf("Test remove for string (valid index): ");
    t_true(l1->remove(0)->equals(s) && l1->size() == 1);

    printf("Test remove for string (negative index): ");
    l1->remove(-1);
    t_true(l1->size() == 1);

    printf("Test remove for string (too big index): ");
    l1->remove(7);
    t_true(l1->size() == 1);

    printf("Test remove for int (valid index): ");
    t_true(l2->remove(0) == 13 && l2->size() == 1);

    printf("Test remove for int (negative index): ");
    l2->remove(-1);
    t_true(l2->size() == 1);

    printf("Test remove for int (too big index): ");
    l2->remove(7);
    t_true(l2->size() == 1);

    printf("Test remove for float (valid index): ");
    t_true(float_equal(l3->remove(0), 7.8) && l3->size() == 1);

    printf("Test remove for float (negative index): ");
    l3->remove(-1);
    t_true(l3->size() == 1);

    printf("Test remove for float (too big index): ");
    l3->remove(7);
    t_true(l3->size() == 1);

    printf("Test remove for bool (valid index): ");
    t_true(l4->remove(0) && l4->size() == 1);

    printf("Test remove for bool (negative index): ");
    l4->remove(-1);
    t_true(l4->size() == 1);

    printf("Test remove for bool (too big index): ");
    l4->remove(7);
    t_true(l4->size() == 1);

    delete s;
    delete t;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
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

    printf("Test set for string (negative index): ");
    t_true(l1->set(-1, u)->equals(u));

    printf("Test set for string (too big index): ");
    t_true(l1->set(5, u)->equals(u));

    printf("Test set for string (valid index): ");
    t_true(l1->set(1, u)->equals(t) && l1->get(1)->equals(u));

    printf("Test set for int (negative index): ");
    t_true(l2->set(-1, 3) == 3);

    printf("Test set for int (too big index): ");
    t_true(l2->set(5, 9) == 9);

    printf("Test set for int (valid index): ");
    t_true(l2->set(1, 9) == 6 && l2->get(1) == 9);

    printf("Test set for float (negative index): ");
    t_true(l3->set(-1, h) == h);

    printf("Test set for float (too big index): ");
    t_true(l3->set(5, h) == h);

    printf("Test set for float (valid index): ");
    t_true(float_equal(l3->set(1, h), 5.6) && l3->get(1) == h);

    printf("Test set for bool (negative index): ");
    t_true(l4->set(-1, true));

    printf("Test set for bool (too big index): ");
    t_true(l4->set(5, true));

    printf("Test set for bool (valid index): ");
    t_true(!l4->set(1, true) && l4->get(1));

    delete s;
    delete t;
    delete u;
    delete l1;
    delete l2;
    delete l3;
    delete l4;
}

void test_size() {
    String *s = new String("Hello");
    float f = 5.6;
    Array *l1 = new Array();
    IntArray *l2 = new IntArray();
    FloatArray *l3 = new FloatArray();
    BoolArray *l4 = new BoolArray();

    printf("Test size for string (empty): ");
    t_true(l1->size() == 0);

    printf("Test size for string (not empty): ");
    l1->append(s);
    t_true(l1->size() == 1);

    printf("Test size for int (empty): ");
    t_true(l2->size() == 0);

    printf("Test size for int (not empty): ");
    l2->append(5);
    t_true(l2->size() == 1);

    printf("Test size for float (empty): ");
    t_true(l3->size() == 0);

    printf("Test size for float (not empty): ");
    l3->append(f);
    t_true(l3->size() == 1);

    printf("Test size for bool (empty): ");
    t_true(l4->size() == 0);

    printf("Test size for bool (not empty): ");
    l4->append(true);
    t_true(l4->size() == 1);

    delete s;
    delete l1;
    delete l2;
    delete l3;
    delete l4;

}

void test_hash() {
    printf("Test hash for string: ");
    String *s = new String("Hello");
    String *t = new String("World");
    Array *l1 = new Array();
    l1->append(s);
    l1->append(t);
    Array *l2 = new Array();
    l2->append_array(l1);
    t_true(l1->hash() == l2->hash());

    printf("Test hash for int: ");
    IntArray *l3 = new IntArray();
    l3->append(1);
    l3->append(2);
    IntArray *l4 = new IntArray();
    l4->append_array(l3);
    t_true(l3->hash() == l4->hash());

    printf("Test hash for float: ");
    float u = -2.2;
    float v = 1.0;
    FloatArray *l5 = new FloatArray();
    l5->append(u);
    l5->append(v);
    FloatArray *l6 = new FloatArray();
    l6->append_array(l5);
    t_true(l5->hash() == l6->hash());

    printf("Test hash for bool: ");
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
