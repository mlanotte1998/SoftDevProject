//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include "schema.h"
#include "column.h"
#include "bool_column.h"
#include "int_column.h"
#include "float_column.h"
#include "string_column.h"
#include "../utility-classes/helper.h"


/**
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
public:
    String* name;
    size_t row_idx_;
    Schema* schema_;
    size_t cols_count_;
    Column** cols_;

    /** Build a row following a schema. */
    Row(Schema& scm) {
      name = nullptr;
      row_idx_ = 0;
      schema_ = new Schema(scm);
      cols_count_ = scm.width();
      cols_ = new Column*[cols_count_];
      for (size_t col_idx = 0; col_idx < cols_count_; col_idx++) {
        if (scm.types_[col_idx] == 'B') cols_[col_idx] = new BoolColumn();
        else if (scm.types_[col_idx] == 'I') cols_[col_idx] = new IntColumn();
        else if (scm.types_[col_idx] == 'F') cols_[col_idx] = new FloatColumn();
        else if (scm.types_[col_idx] == 'S') cols_[col_idx] = new StringColumn();
        else {
          pln(given_malformed_schema);
          exit(-1);
        }
      }
    }

    ~Row() {
      delete name;
      row_idx_ = 0;

      for (int i = 0; i < cols_count_; i++) {
          delete cols_[i];
      }
      delete[] cols_;
      delete schema_;
    }

    /** Setters: set the given column with the given value. Setting a column with
      * a value of the wrong type is undefined. */
    void set(size_t col, int val){
      if (col >= width()) return;
      IntColumn* column = cols_[col]->as_int();
      if (column == nullptr) return;
      if (column->size() == 0) column->push_back(val);
      else column->set(0, val);
    }

    void set(size_t col, float val) {
      if (col >= width()) return;
      FloatColumn* column = cols_[col]->as_float();
      if (column == nullptr) return;
      if (column->size() == 0) column->push_back(val);
      else column->set(0, val);
    }

    void set(size_t col, bool val) {
      if (col >= width()) return;
      BoolColumn* column = cols_[col]->as_bool();
      if (column == nullptr) return;
      if (column->size() == 0) column->push_back(val);
      else column->set(0, val);
    }

    /** The string is external. */
    void set(size_t col, String* val) {
      if (col >= width()) return;
      StringColumn* column = cols_[col]->as_string();
      if (column == nullptr) return;
      if (column->size() == 0) column->push_back(val);
      else column->set(0, val);
    }

    /** Set/get the index of this row (ie. its position in the dataframe. This is
     *  only used for informational purposes, unused otherwise */
    void set_idx(size_t idx) {
      row_idx_ = idx;
    }

    size_t get_idx() {
      return row_idx_;
    }

    /** Getters: get the value at the given column. If the column is not
      * of the requested type, the result is undefined. */
    int get_int(size_t col) {
      exit_if_not(col < width(), int_col_index_out_of_bounds);
      IntColumn* column = cols_[col]->as_int();
      exit_if_not(column != nullptr, non_int_col);
      exit_if_not(column->size() != 0, int_col_emtpy);
      return column->get(0);
    }

    bool get_bool(size_t col) {
      exit_if_not(col < width(), bool_col_index_out_of_bounds);
      BoolColumn* column = cols_[col]->as_bool();
      exit_if_not(column != nullptr, non_bool_col);
      exit_if_not(column->size() != 0, bool_col_emtpy);
      return column->get(0);
    }

    float get_float(size_t col) {
      exit_if_not(col < width(), float_col_index_out_of_bounds);
      FloatColumn* column = cols_[col]->as_float();
      exit_if_not(column != nullptr, non_float_col);
      exit_if_not(column->size() != 0, float_col_emtpy);
      return column->get(0);
    }

    String* get_string(size_t col) {

      exit_if_not(col < width(), string_col_index_out_of_bounds);
      StringColumn* column = cols_[col]->as_string();
      exit_if_not(column != nullptr, non_string_col);
      exit_if_not(column->size() != 0, string_col_emtpy);
      return column->get(0);
    }

    /** Number of fields in the row. */
    size_t width() {
      return cols_count_;
    }

    /** Type of the field at the given position. An idx >= width is  undefined. */
    char col_type(size_t idx) {
      exit_if_not(idx < width(), col_index_out_of_bounds);
      return schema_->col_type(idx);
    }
};
