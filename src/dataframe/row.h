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
#include "helper.h"

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
    Schema schema_;
    Column** cols_;

    /** Build a row following a schema. */
    Row(Schema& scm) {
      name = nullptr;
      row_idx_ = 0;
      schema_ = scm;
      size_t cols_count_ = scm.width();
      cols_ = new Column*[cols_count_];
      for (size_t col_idx = 0; col_idx < cols_count_; col_idx++) {
        if (scm.types_[col_idx] == 'B') cols_[col_idx] = new BoolColumn();
        else if (scm.types_[col_idx] == 'I') cols_[col_idx] = new IntColumn();
        else if (scm.types_[col_idx] == 'F') cols_[col_idx] = new FloatColumn();
        else if (scm.types_[col_idx] == 'S') cols_[col_idx] = new StringColumn();
        else {
          pln("Given Malformed Schema");
          exit(-1);
        }
      }
    }

    ~Row() {
      delete name;
      row_idx_ = 0;
      schema_ = nullptr;
      delete[] cols_;
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
      exit_if_not(col < width(), duplicate("Int Col Index out of Bounds"));
      IntColumn* column = cols_[col]->as_int();
      exit_if_not(column != nullptr, duplicate("Non-Integer Column"));
      exit_if_not(column->size() != 0, duplicate("Int Col Index out of Bounds"));
      return column->get(0);
    }

    bool get_bool(size_t col) {
      exit_if_not(col < width(), duplicate("Bool Col Index out of Bounds"));
      BoolColumn* column = cols_[col]->as_bool();
      exit_if_not(column != nullptr, duplicate("Non-Boolean Column"));
      exit_if_not(column->size() != 0, duplicate("Bool Col Index out of Bounds"));
      return column->get(0);
    }

    float get_float(size_t col) {
      exit_if_not(col < width(), duplicate("Float Col Index out of Bounds"));
      FloatColumn* column = cols_[col]->as_float();
      exit_if_not(column != nullptr, duplicate("Non-Float Column"));
      exit_if_not(column->size() != 0, duplicate("Float Col Is Empty"));
      return column->get(0);
    }

    String* get_string(size_t col) {
      exit_if_not(col < width(), duplicate("String Col Index out of Bounds"));
      StringColumn* column = cols_[col]->as_string();
      exit_if_not(column != nullptr, duplicate("Non-String Column"));
      exit_if_not(column->size() != 0, duplicate("String Col Index out of Bounds"));
      return column->get(0);
    }

    /** Number of fields in the row. */
    size_t width() {
      return schema_.width();
    }

    /** Type of the field at the given position. An idx >= width is  undefined. */
    char col_type(size_t idx) {
      exit_if_not(idx < width(), duplicate("Col Index out of Bounds"));
      return schema_.col_type(idx);
    }

    /** Given a Fielder, visit every field of this row. The first argument is
      * index of the row in the dataframe.
      * Calling this method before the row's fields have been set is undefined. */
    void visit(size_t idx, Fielder& f) {
      f.start(idx);
      for (size_t index = 0; index < schema_.width(); index++) {
        if (col_type(index) == 'B') f.accept(get_bool(index));
        else if (col_type(index) == 'I') f.accept(get_int(index));
        else if (col_type(index) == 'F') f.accept(get_float(index));
        else if (col_type(index) == 'S') f.accept(get_string(index));
        else {
          pln("Given Malformed Schema");
          exit(-1);
        }
      }
      f.done();
    }
};
