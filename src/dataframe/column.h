//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include "../utility-classes/object.h"
#include <stdio.h>
#include <stdarg.h>

class IntColumn;
class BoolColumn;
class FloatColumn;
class DoubleColumn;
class StringColumn;

/***
 * Column ::
 * Represents one column of a data frame which holds values of a single type.
 * This abstract class defines methods overriden in subclasses. There is
 * one subclass per element type. Columns are mutable, equality is pointer
 * equality. */
class Column : public Object {
public:

    size_t array_length_;  // length of the upper array
    size_t size_; // number of elements.

    /** Type converters: Return same column under its actual type, or
     *  nullptr if of the wrong type.  */

    /**
     * Return nullptr here and then classes that would return themselves
     * for the specific functions override them
     */

    virtual IntColumn* as_int() {
        return nullptr;
    }

    virtual BoolColumn*  as_bool() {
        return nullptr;
    }

    virtual FloatColumn* as_float() {
        return nullptr;
    }

    virtual DoubleColumn* as_double() {
        return nullptr;
    }

    virtual StringColumn* as_string() {
        return nullptr;
    }

    /** Type appropriate push_back methods. Calling the wrong method is
      * undefined behavior. **/
    virtual void push_back(int val) {}
    virtual void push_back(bool val) {}
    virtual void push_back(float val) {}
    virtual void push_back(double val) {}
    virtual void push_back(String* val) {}

    /** Returns the number of elements in the column. */
    virtual size_t size() {
        return size_;
    }

    /** Return the type of this column as a char: 'S', 'B', 'I' and 'F'.**/
    char get_type() {
        /**
         * Run the virtual functions already made to determine which this is.
         */
        if (this->as_bool() != nullptr) {
            return 'B';
        } else if (this->as_float() != nullptr) {
            return 'F';
        } else if (this->as_int() != nullptr) {
            return 'I';
        } else if (this->as_double() != nullptr) {
            return 'D';
        } else if (this->as_string() != nullptr) {
            return 'S';
        } else {
            return '\0';
        }
    }

};
