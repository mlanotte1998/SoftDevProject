//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include "../utility-classes/string.h"
#include "../utility-classes/helper.h"
#include "constants.h"

/***
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Schema : public Object {
public:
    size_t cols_;
    size_t rows_;
    char* types_;
    size_t cols_capacity_;
    size_t rows_capacity_;

    /** Copying constructor */
    Schema(Schema& from) {
      cols_ = from.cols_;
      rows_ = from.rows_;
      // iterate through other types and assign to types_
      types_ = new char[cols_];
      for (size_t type_idx = 0; type_idx < cols_; type_idx++) {
        types_[type_idx] = from.types_[type_idx];
      }

      rows_capacity_ = rows_;
      cols_capacity_ = cols_;
    }

    /** Create an empty schema **/
    Schema() {
      cols_ = 0;
      rows_ = 0;
      types_ = new char[1];
      cols_capacity_ = 1;
      rows_capacity_ = 1;
    }

    /** Create a schema from a string of types. A string that contains
      * characters other than those identifying the four type results in
      * undefined behavior. The argument is external, a nullptr argument is
      * undefined. **/
    Schema(const char* types) {
      // null schema descriptor is invalid
      if (types == nullptr) return;
      // store schema description as array of Strings
      size_t schema_length = strlen(types);
      types_ = new char[schema_length];
      for (int i = 0; i < schema_length; i++) {
        if (types[i] == 'B' || types[i] == 'I' || types[i] == 'F' || types[i] == 'D' || types[i] == 'S') {
          types_[i] = types[i];
        } else {
          delete[] types_;
          pln(given_malformed_schema);
          exit(-1);
        }
      }
      cols_ = schema_length;
      rows_ = 0;
      cols_capacity_ = schema_length;
      rows_capacity_ = 1;
    }

    ~Schema() {
      delete [] types_;
    }

    /** Add a column of the given type */
    void add_column(char typ) {
      // return if invalid type
      if (typ != 'B' && typ != 'I' && typ != 'F' && typ != 'D' && typ != 'S') {
        return;
      }
      // add type to types_
      if (cols_ >= cols_capacity_) {
        if (cols_capacity_ == 0) {
          cols_capacity_ = 1; 
        }
        char* grow_types_ = new char[cols_capacity_ * 2];
        for (size_t col_idx = 0; col_idx < cols_; col_idx++) {
          grow_types_[col_idx] = types_[col_idx];
        }
        grow_types_[cols_] = typ;
        delete [] types_;
        types_ = grow_types_;
        cols_capacity_ *= 2;
      } else {
          types_[cols_] = typ;
      }

      // increment cols count
      cols_++;
    }

    /** Return type of column at idx. An idx >= width is undefined. */
    char col_type(size_t idx) {
      if (idx >= cols_) return 'U';
      return types_[idx];
    }

    /** The number of columns */
    size_t width() {
      return cols_;
    }

    /** The number of rows */
    size_t length() {
      return rows_;
    }
};
