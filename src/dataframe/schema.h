//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include "../utility-classes/string.h"
#include "../utility-classes/helper.h"

/***
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
public:
    size_t cols_;
    size_t rows_;
    char* types_;
    String** col_names_;
    size_t cols_capacity_;
    String** row_names_;
    size_t rows_capacity_;

    // TODO: Implement using String Buffer and Given Helper Methods

    /** Copying constructor */
    Schema(Schema& from) {
      cols_ = from.cols_;
      rows_ = from.rows_;
      // iterate through other types and assign to types_
      types_ = new char[cols_];
      for (size_t type_idx = 0; type_idx < cols_; type_idx++) {
        types_[type_idx] = from.types_[type_idx];
      }
      // iterate through other row_names and assign to row_names_
      row_names_ = new String*[rows_];
      for (size_t row_idx = 0; row_idx < rows_; row_idx++) {
        // add string copy of row_name to row_names_
        if (from.row_names_[row_idx] != nullptr) {
            row_names_[row_idx] = new String(*from.row_names_[row_idx]);
        } else {
            row_names_[row_idx] = nullptr;
        }
      }

      // iterate through other col_names and assign to col_names_
      col_names_ = new String*[cols_];
      for (size_t col_idx = 0; col_idx < cols_; col_idx++) {
        // add string copy of col_name to col_names_
        if (from.col_names_[col_idx] != nullptr) {
          col_names_[col_idx] = new String(*from.col_names_[col_idx]);
        } else {
          col_names_[col_idx] = nullptr;
        }
      }
      rows_capacity_ = rows_;
      cols_capacity_ = cols_;
    }

    /** Create an empty schema **/
    Schema() {
      cols_ = 0;
      rows_ = 0;
      types_ = new char[1];
      row_names_ = new String*[1];
      col_names_ = new String*[1];
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
        if (types[i] == 'B' || types[i] == 'I' || types[i] == 'F' || types[i] == 'S') {
          types_[i] = types[i];
        } else {
          delete[] types_;
          pln("Given Malformed Schema");
          exit(-1);
        }
      }
      cols_ = schema_length;
      rows_ = 0;
      row_names_ = new String*[1];
      col_names_ = new String*[schema_length];
      for (int idx = 0; idx < schema_length; idx++) {
        col_names_[idx] = nullptr;
      }
      cols_capacity_ = schema_length;
      rows_capacity_ = 1;
    }

    ~Schema() {
    }

    /** Add a column of the given type and name (can be nullptr), name
      * is external. Names are expectd to be unique, duplicates result
      * in undefined behavior. */
    void add_column(char typ, String* name) {
      // return if invalid type
      if (typ != 'B' && typ != 'I' && typ != 'F' && typ != 'S') {
        return;
      }
      // add type to types_
      if (cols_ >= cols_capacity_) {
        char* grow_types_ = new char[cols_capacity_ * 2];
        for (size_t col_idx = 0; col_idx < cols_; col_idx++) {
          grow_types_[col_idx] = types_[col_idx];
        }
        grow_types_[cols_] = typ;
        delete types_;
        types_ = grow_types_;
        // add name to col_names
        String** grow_names = new String*[cols_capacity_ * 2];
        for (size_t name_idx = 0; name_idx < cols_; name_idx++) {
          grow_names[name_idx] = col_names_[name_idx];
        }
        grow_names[cols_] = name;
        delete col_names_;
        col_names_ = grow_names;
        cols_capacity_ *= 2;
      } else {
          types_[cols_] = typ;
          col_names_[cols_] = name;
      }

      // increment cols count
      cols_++;
    }

    /** Add a row with a name (possibly nullptr), name is external.  Names are
     *  expectd to be unique, duplicates result in undefined behavior. */
    void add_row(String* name) {
      // add name to row_names
      if (rows_ >= rows_capacity_) {
        String** grow_names = new String*[rows_capacity_ * 2];
        for (size_t name_idx = 0; name_idx < rows_; name_idx++) {
          grow_names[name_idx] = row_names_[name_idx];
        }
        grow_names[rows_] = name;
        delete row_names_;
        row_names_ = grow_names;
        rows_capacity_ *= 2;
      } else {
        row_names_[rows_] = name;
      }

      // increment rows count
      rows_++;
    }

    /** Return name of row at idx; nullptr indicates no name. An idx >= width
      * is undefined. */
    String* row_name(size_t idx) {
      if (idx >= rows_) return new String("UNDEFINED");
      return row_names_[idx];
    }

    /** Return name of column at idx; nullptr indicates no name given.
      *  An idx >= width is undefined.*/
    String* col_name(size_t idx) {
      if (idx >= cols_) return new String("UNDEFINED");
      return col_names_[idx];
    }

    /** Return type of column at idx. An idx >= width is undefined. */
    char col_type(size_t idx) {
      if (idx >= cols_) return 'U';
      return types_[idx];
    }

    /** Given a column name return its index, or -1. */
    int col_idx(const char* name) {
      if (cols_ == 0) return -1;
      // iterate through col_names_ return index of first match
      String* name_str = nullptr;
      if (name != nullptr) {
        name_str = new String(name);
      }
      for (size_t name_idx = 0; name_idx < cols_; name_idx++) {
        if (name_str == nullptr && name_str == col_names_[name_idx]) {
          return name_idx;
        } else if (name_str != nullptr && name_str->equals(col_names_[name_idx])) {
          delete name_str;
          return name_idx;
        }
      }
      if (name_str != nullptr) delete name_str;
      return -1;
    }

    /** Given a row name return its index, or -1. */
    int row_idx(const char* name) {
      if (rows_ == 0) return -1;
      // if given nullptr search for nullptr in row_names else search for string
      String* name_str = nullptr;
      if (name != nullptr) {
        name_str = new String(name);
      }
      // iterate through row_names_ return index of first match
      for (size_t name_idx = 0; name_idx < rows_; name_idx++) {
        if (name_str == nullptr && name_str == row_names_[name_idx]) {
          return name_idx;
        } else if (name_str != nullptr && name_str->equals(row_names_[name_idx])) {
          delete name_str;
          return name_idx;
        }
      }
      // if no match found return -1
      if (name_str != nullptr) delete name_str;
      return -1;
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
