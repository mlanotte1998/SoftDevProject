//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

//Language::CwC

#pragma once

#include <iostream>
#include <math.h>

#include "schema.h"
#include "row.h"
#include "rower.h"

/***
* DataFrame::
*
* A DataFrame is table composed of columns of equal length. Each column
* holds values of the same type (I, S, B, F). A dataframe has a schema that
* describes it.
* authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
*/
class ModifiedDataFrame : public Object {
public:
    size_t length_;
    Schema schema_;
    Column **cols_;

    /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
    ModifiedDataFrame(ModifiedDataFrame &df) {
        length_ = 0;
        Schema other_schema = df.get_schema();
        cols_ = new Column *[other_schema.width()];
        schema_ = Schema();
        for (size_t i = 0; i < other_schema.width(); i++) {
            if (other_schema.types_[i] == 'B') {
                add_column(new BoolColumn(), other_schema.col_name(i));
            } else if (other_schema.types_[i] == 'I') {
                add_column(new IntColumn(), other_schema.col_name(i));
            } else if (other_schema.types_[i] == 'F') {
                add_column(new FloatColumn(), other_schema.col_name(i));
            } else if (other_schema.types_[i] == 'D') {
                add_column(new DoubleColumn(), other_schema.col_name(i));
            } else if (other_schema.types_[i] == 'S') {
                add_column(new StringColumn(), other_schema.col_name(i));
            } else {
                pln("Given Malformed Schema");
                exit(-1);
            }
        }
    }

    /** Create a data frame from a schema and columns. All columns are created
      * empty. */
    ModifiedDataFrame(Schema &schema) {
        length_ = 0;
        schema_ = schema;
        cols_ = new Column *[schema.width()];
        for (size_t i = 0; i < schema.width(); i++) {
            if (schema.types_[i] == 'B') {
                cols_[i] = new BoolColumn();
            } else if (schema.types_[i] == 'I') {
                cols_[i] = new IntColumn();
            } else if (schema.types_[i] == 'F') {
                cols_[i] = new FloatColumn();
            } else if (schema.types_[i] == 'D') {
                cols_[i] = new DoubleColumn();
            } else if (schema.types_[i] == 'S') {
                cols_[i] = new StringColumn();
            } else {
                pln("Given Malformed Schema");
                exit(-1);
            }
        }
    }

    /** ModifiedDataFrame destructor */
    ~ModifiedDataFrame() {

    }

    /** Returns the dataframe's schema. Modifying the schema after a dataframe
      * has been created in undefined. */
    Schema &get_schema() {
        return schema_;
    }

    /** Adds a column this dataframe, updates the schema, the new column
      * is external, and appears as the last column of the dataframe, the
      * name is optional and external. A nullptr colum is undefined. */
    void add_column(Column *col, String *name) {
        bool valid = col->size() == nrows() || nrows() == 0;
        exit_if_not(valid, duplicate("Invalid Column Size"));
        if (nrows() == 0) length_ = col->size();
        // add column data to dataframe
        Column **grow_cols_ = new Column *[schema_.width() + 1];
        for (size_t col_idx = 0; col_idx < schema_.width(); col_idx++) {
            grow_cols_[col_idx] = cols_[col_idx];
        }
        grow_cols_[schema_.width()] = col;
        delete[] cols_;
        cols_ = grow_cols_;
        // add column + name to schema
        schema_.add_column(col->get_type(), name);
    }

    /** Return the int value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    int get_int(size_t col, size_t row) {
        exit_if_not(col < ncols(), duplicate("Col Index out of Bounds"));
        IntColumn *column = cols_[col]->as_int();
        exit_if_not(column != nullptr, duplicate("Non-Integer Column"));
        exit_if_not(row < column->size(), duplicate("Row Index out of Bounds"));
        return column->get(row);
    }

    /** Return the bool value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    bool get_bool(size_t col, size_t row) {
        exit_if_not(col < ncols(), duplicate("Col Index out of Bounds"));
        BoolColumn *column = cols_[col]->as_bool();
        exit_if_not(column != nullptr, duplicate("Non-Boolean Column"));
        exit_if_not(row < column->size(), duplicate("Row Index out of Bounds"));
        return column->get(row);
    }

    /** Return the double value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    double get_double(size_t col, size_t row) {
        exit_if_not(col < ncols(), duplicate("Col Index out of Bounds"));
        DoubleColumn *column = cols_[col]->as_double();
        exit_if_not(column != nullptr, duplicate("Non-Double Column"));
        exit_if_not(row < column->size(), duplicate("Row Index out of Bounds"));
        return column->get(row);
    }

    /** Return the float value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    float get_float(size_t col, size_t row) {
        exit_if_not(col < ncols(), duplicate("Col Index out of Bounds"));
        FloatColumn *column = cols_[col]->as_float();
        exit_if_not(column != nullptr, duplicate("Non-Float Column"));
        exit_if_not(row < column->size(), duplicate("Row Index out of Bounds"));
        return column->get(row);
    }

    /** Return the string value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    String *get_string(size_t col, size_t row) {
        exit_if_not(col < ncols(), duplicate("Col Index out of Bounds"));
        StringColumn *column = cols_[col]->as_string();
        exit_if_not(column != nullptr, duplicate("Non-String Column"));
        exit_if_not(row < column->size(), duplicate("Row Index out of Bounds"));
        return column->get(row);
    }

    /** Return the offset of the given column name or -1 if no such col. */
    int get_col(String &col) {
        return schema_.col_idx(col.c_str());
    }

    /** Return the offset of the given row name or -1 if no such row. */
    int get_row(String &col) {
        return schema_.row_idx(col.c_str());
    }

    /** Set the int value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, int val) {
        if (col >= ncols() || row >= nrows()) return;
        IntColumn *column = cols_[col]->as_int();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    /** Set the bool value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, bool val) {
        if (col >= ncols() || row >= nrows()) return;
        BoolColumn *column = cols_[col]->as_bool();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    /** Set the float value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, float val) {
        if (col >= ncols() || row >= nrows()) return;
        FloatColumn *column = cols_[col]->as_float();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    /** Set the double value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, double val) {
        if (col >= ncols() || row >= nrows()) return;
        DoubleColumn *column = cols_[col]->as_double();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    /** Set the string value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, String *val) {
        if (col >= ncols() || row >= nrows()) return;
        StringColumn *column = cols_[col]->as_string();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    /** Set the fields of the given row object with values from the columns at
      * the given offset.  If the row is not form the same schema as the
      * dataframe, results are undefined.
      */
    void fill_row(size_t idx, Row &row) {
        if (idx >= nrows()) return;
        for (size_t col = 0; col < ncols(); col++) {
            if (schema_.col_type(col) == 'B') row.set(col, get_bool(col, idx));
            else if (schema_.col_type(col) == 'I') row.set(col, get_int(col, idx));
            else if (schema_.col_type(col) == 'F') row.set(col, get_float(col, idx));
            else if (schema_.col_type(col) == 'D') row.set(col, get_double(col, idx));
            else if (schema_.col_type(col) == 'S') row.set(col, get_string(col, idx));
        }
    }

    /** Add a row at the end of this dataframe. The row is expected to have
     *  the right schema and be filled with values, otherwise undedined.  */
    void add_row(Row &row) {
        for (size_t col = 0; col < ncols(); col++) {
            if (schema_.col_type(col) == 'B') cols_[col]->as_bool()->push_back(row.get_bool(col));
            else if (schema_.col_type(col) == 'I') cols_[col]->as_int()->push_back(row.get_int(col));
            else if (schema_.col_type(col) == 'F') cols_[col]->as_float()->push_back(row.get_float(col));
            else if (schema_.col_type(col) == 'D') cols_[col]->as_double()->push_back(row.get_double(col));
            else if (schema_.col_type(col) == 'S') cols_[col]->as_string()->push_back(row.get_string(col));
        }
        if (length_ >= schema_.length()) {
            schema_.add_row(nullptr);
        }
        length_++;
    }

    /** The number of rows in the dataframe. */
    size_t nrows() {
        return length_;
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return schema_.width();
    }

    /** Helper method for map and pmap */
    void indexed_map(Rower &r, size_t start, size_t end) {
        Row *iterator_row = new Row(schema_);
        for (size_t i = start; i < end; i++) {
            fill_row(i, *iterator_row);
            r.accept(*iterator_row);
        }
    }

    /** Visit rows in order */
    void map(Rower &r) {
        indexed_map(r, 0, nrows());
    }

    /** Visit rows concurrently */
    void pmap(Rower &r);

    /** Create a new dataframe, constructed from rows for which the given Rower
      * returned true from its accept method. */
    ModifiedDataFrame *filter(Rower &r) {
       ModifiedDataFrame* nd = new ModifiedDataFrame(*this);
        Row* iterator_row = new Row(schema_);
        for(unsigned int i = 0; i < nrows(); i++) {
            fill_row(i, *iterator_row);
            if (r.accept(*iterator_row)){
                nd->add_row(*iterator_row);
            }
        }
        return nd;
    }
};

#include "rower_thread.h"

inline void ModifiedDataFrame::pmap(Rower &r) {
    // Decide how many threads to use
    int max_threads = 8;
    int df_rows = nrows();
    int thread_count = floor(df_rows / 2);

    // Zero threads is invalid
    if (thread_count == 0) return;

    // Do not use more than the maximum number of threads
    if (thread_count > max_threads) thread_count = max_threads;

    // Create thread pool of thread count
    RowerThread** thread_pool = new RowerThread*[thread_count];
    int increment = df_rows / thread_count;

    // First thread contains the original rower
    thread_pool[0] = new RowerThread(this, &r, 0, increment);
    thread_pool[0]->start();

    // Create and start pool of thread count
    for (int i = 1; i < thread_count; i++) {
        if (i == thread_count - 1) {
            thread_pool[i] = new RowerThread(this, dynamic_cast<Rower*>(r.clone()), increment*i, df_rows);
        } else {
            thread_pool[i] = new RowerThread(this, dynamic_cast<Rower*>(r.clone()), increment*i, increment*(i+1));
        }
        thread_pool[i]->start();
    }

    // Wait for threads to finish and merge rowers into original rower
    thread_pool[0]->join();
    for (int i = 1; i < thread_count; i++) {
        thread_pool[i]->join();
        thread_pool[0]->rower_->join_delete(thread_pool[i]->rower_);
    }
};
