#include <stdio.h>

//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
public:

    /** Called before visiting a row, the argument is the row offset in the
      dataframe. */
    virtual void start(size_t r) {
      return;
    };

    /** Called for fields of the argument's type with the value of the field. */
    virtual void accept(bool b) {
      return;
    };

    virtual void accept(float f) {
      return;
    };

    virtual void accept(int i) {
      return;
    };

    virtual void accept(String* s) {
      return;
    };

    /** Called when all fields have been seen. */
    virtual void done() {
      return;
    };
};

class PrintFielder : public Fielder {
public:

  PrintFielder() {}

  void start(size_t r) {
    return;
  }

  void accept(bool b) {
    if (b) {
      printf("<1>");
    } else {
      printf("<0>");
    }
  }

  void accept(float f){
    printf("<%f>", f);
  }

  void accept(int i) {
    printf("<%d>", i);
  }

  void accept(String* s) {
    printf("<");
    for(unsigned int i = 0; i < s->size(); i++) {
      printf("%c", s->at(i));
    }
    printf(">");
  }

  void done() {
    printf("\n");
  }
};
