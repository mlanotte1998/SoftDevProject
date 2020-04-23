#pragma once
// LANGUAGE: CwC
#include <cstring>
#include <string>
#include <cassert>
#include "object.h"

/** An immutable string class that wraps a character array.
 * The character array is zero terminated. The size() of the
 * String does count the terminator character. Most operations
 * work by copy, but there are exceptions (this is mostly to support
 * large strings and avoid them being copied).
 *  author: vitekj@me.com */
class String : public Object {
public:
  size_t size_; // number of characters excluding terminate (\0)
  char *cstr_;  // owned; char array
  
  /** Build a string from a string constant */
  String(char const* cstr, size_t len) {
    assert(cstr[len] == 0);
    size_ = len;
    // Cannot use duplicate here because String is allowed to contain 0
    // characters as we use it as a buffer regularly.
    cstr_ = new char[size_ + 1];
    memcpy(cstr_, cstr, size_ + 1);
  }
  
  /** Copy constructor and poor man's move constructor.   */
  String(String& from, bool move = false) {
    size_ = from.size_;
    if (move) {
      cstr_ = from.cstr_;
      from.cstr_ = nullptr;
      from.size_ = 0;
    } else {
      cstr_ = new char[size_ + 1];
      memcpy(cstr_, from.cstr_, size_ + 1);
    }
    assert(cstr_[size_] == 0);
  }

  // TODO is the steal argument there to distinguish between the two
  // constructors?  if yeas, it should be last optional argument and not
  // first. If only stealing should be allowed I would go for static
  // function in C++, not sure if they are allowed in CwC:)
  String(bool steal, char* cstr, size_t len) {
    assert(steal && cstr[len] == 0);
    size_ = len;
    cstr_ = cstr;
  }
  
  String(bool steal, char* cstr) : String(steal, cstr, strlen(cstr)) {}

  String(char const* cstr) : String(cstr, strlen(cstr)) {}

  /** Delete the string */
  ~String() { delete[] cstr_; }
  
  /** Return the number characters in the string (doesn't count terminator) */
  size_t size() { return size_; }
  
  /** Return the raw char*. The result should not be modified or freed. */
  char* c_str() {  return cstr_; }
  
  /** Returns the character at index */
  char at(size_t index) {
    assert(index < size_);
    return cstr_[index];
  }
    
  /** Compare two strings. */
  bool equals(Object* other) {
    if (other == this) return true;
    String* x = dynamic_cast<String *>(other);
    if (x == nullptr) return false;
    if (size_ != x->size_) return false;
    return strncmp(cstr_, x->cstr_, size_) == 0;
  }
  
  /** Deep copy of this string */
  String* clone() { return new String(*this); }
  
  /** This consumes cstr_, the String must be deleted next */
  char * steal() {
    char *res = cstr_;
    cstr_ = nullptr;
    return res;
  }
  
  /** Compute a hash for this string. */
  size_t hash_me() {
    size_t hash = 0;
    for (size_t i = 0; i < size_; ++i)
      hash = cstr_[i] + (hash << 6) + (hash << 16) - hash;
    return hash;
  }
};


/** A string buffer builds a string from various pieces.
 *  author: jv */
class StrBuff : public Object {
public:
  size_t capacity_ = 10;
  char *val_ = new char[capacity_]; // owned; consumed by get()
  size_t size_ = 0;
  
  StrBuff() {}

  StrBuff(String& s) { c(s);  }

  StrBuff(const char* s) { c(s);  }
  
  void grow_by_(size_t step) {
    if (step + size_ < capacity_)
        return;
    capacity_ = step + 2*capacity_;
    char* oldV = val_;
    val_ = new char[capacity_];
    strncpy(val_, oldV, size_);
    delete[] oldV;
  }

  StrBuff& c(const char* str) {
    size_t step = strlen(str);
    grow_by_(step + 1);
    memcpy(val_+size_, str, step + 1);
    size_ += step;
    return *this;
  }

  StrBuff& c(String &s) { return c(s.c_str());  }
  StrBuff& c(size_t v) { return c(std::to_string(v).c_str());  } // Cpp
  
  String* get() {
    assert(val_); // can be called only once
    grow_by_(1);  // ensure space for terminator
    val_[size_] = 0; // terminate
    String *res = new String(true, val_, size_);
    val_ = new char[capacity_ = 10]; // val_ was consumed above
    size_ = 0;
    return res;
  }
};  // StrBuff
