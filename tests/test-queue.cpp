// Lang::cpp

#include <stdio.h>
#include "../src/utility-classes/object.h"
#include "../src/utility-classes/string.h"
#include "../src/utility-classes/queue.h"


void FAIL() { exit(1); }
void OK(const char* m) { printf("Test %s passed\n", m); }
void t_true(bool p) { if (!p) FAIL(); }
void t_false(bool p) { if(p) FAIL(); }

// Size when empty.
void test_size_empty() {
  StringQueue * q = new StringQueue();
  t_true(q->size() == 0);
  delete q;
  OK("1");
}

// Size when not empty AND Size modified when pushed.
void test_not_empty_push() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  q->push(s);
  t_true(q->size() == 1);
  delete s;
  delete q;
  OK("2");
}

// Size modified when popped.
void test_size_pop() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  String * s2 = new String("GOODBYE");
  t_true(q->size() == 0);
  q->push(s);
  t_true(q->size() == 1);
  q->push(s2);
  t_true(q->size() == 2);
  delete(q->pop());
  t_true(q->size() == 1);
  delete s2;
  delete q;
  OK("3");
}

// Size remains when peeked.
void test_size_peek() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  String * s2 = new String("GOODBYE");
  t_true(q->size() == 0);
  q->push(s);
  t_true(q->size() == 1);
  q->push(s2);
  t_true(q->size() == 2);
  q->peek();
  t_true(q->size() == 2);
  delete s;
  delete s2;
  delete q;
  OK("4");
}

// Size remains when pushed and popped.
void test_size_push_pop() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  t_true(q->size() == 0);
  q->push(s);
  delete(q->pop());
  t_true(q->size() == 0);
  delete q;
  OK("5");
}

// Pop returns correct element.
void test_pop_element() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  String * s2 = new String("GOODBYE");
  q->push(s);
  q->push(s2);
  String * s3 = q->pop();
  String * s4 = q->pop();
  t_true(s3->equals(s));
  t_true(s4->equals(s2));
  delete s;
  delete s2;
  delete q;
  OK("6");
}

// Peek returns correct element AND Peek doesn't remove element.
void test_peek_element() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  String * s2 = new String("GOODBYE");
  q->push(s);
  q->push(s2);
  String * s3 = q->peek();
  String * s4 = q->peek();
  t_true(s3->equals(s));
  t_true(s4->equals(s));
  delete s;
  delete s2;
  delete q;
  OK("7");
}

// isEmpty returns true when elements are not there.
void test_isEmpty_true() {
  StringQueue * q = new StringQueue();
  t_true(q->isEmpty());
  delete q;
  OK("8");
}

// isEmpty returns false when elements are there.
void test_isEmpty_false() {
  StringQueue * q = new StringQueue();
  String * s = new String("HELLO");
  String * s2 = new String("GOODBYE");
  q->push(s);
  q->push(s2);
  t_false(q->isEmpty());
  delete(q->pop());
  t_false(q->isEmpty());
  delete s2;
  delete q;
  OK("9");
}

// tests large volume
void test_large() {
  StringQueue* q = new StringQueue();
  String* s = new String("Hello");
  String* s2 = new String("Goodbye");
  for (size_t i = 0; i < 500; ++i) {
    q->push(s);
    q->push(s2);
  }
  t_true(q->size() == 1000);
  for (size_t i = 0; i < 1000; ++i) {
    String* peek = q->peek();
    String* pop = q->pop();
    t_true(peek->equals(pop));
  }
  t_true(q->size() == 0);
  delete s;
  delete s2;
  delete q;
  OK("10");
}

/**
  * Runs the tests on Queue operations.
  * authors: horn.s@husky.neu.edu, armani.a@husky.neu.edu
  */
int main() {
  test_size_empty();
  test_not_empty_push();
  test_size_pop();
  test_size_peek();
  test_size_push_pop();
  test_pop_element();
  test_peek_element();
  test_isEmpty_true();
  test_isEmpty_false();
  test_large();
}
