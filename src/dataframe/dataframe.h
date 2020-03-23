//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

//Language::CwC

#pragma once

#include "constants.h"
#include "schema.h"
#include "row.h"
#include "rower.h"
#include "../store/kdstore.h"


/***
* DataFrame::
*
* A DataFrame is table composed of columns of equal length. Each column
* holds values of the same type (I, S, B, F). A dataframe has a schema that
* describes it.
*/
class DataFrame : public Object {
public:
    size_t length_;
    Schema *schema_;
    Column **cols_;

    /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
    DataFrame(DataFrame &df) {
        length_ = 0;
        Schema other_schema = df.get_schema();
        cols_ = new Column *[other_schema.width()];
        schema_ = new Schema(other_schema);
        for (size_t i = 0; i < schema_->width(); i++) {
            if (other_schema.types_[i] == 'B') {
                cols_[i] = new BoolColumn();
            } else if (other_schema.types_[i] == 'I') {
                cols_[i] = new IntColumn();
            } else if (other_schema.types_[i] == 'F') {
                cols_[i] = new FloatColumn();
            } else if (other_schema.types_[i] == 'D') {
                cols_[i] = new DoubleColumn();
            } else if (other_schema.types_[i] == 'S') {
                cols_[i] = new StringColumn();
            } else {
                pln(given_malformed_schema);
                exit(-1);
            }
        }
    }

    /** Create a data frame from a schema and columns. All columns are created
      * empty. */
    DataFrame(Schema &schema) {
        // modifying this dataframe modifies its internal schema, but not the external schema
        length_ = 0;
        schema_ = new Schema(schema);
        cols_ = new Column *[schema_->width()];
        for (size_t i = 0; i < schema_->width(); i++) {
            if (schema_->types_[i] == 'B') {
                cols_[i] = new BoolColumn();
            } else if (schema_->types_[i] == 'I') {
                cols_[i] = new IntColumn();
            } else if (schema_->types_[i] == 'F') {
                cols_[i] = new FloatColumn();
            } else if (schema_->types_[i] == 'D') {
                cols_[i] = new DoubleColumn();
            } else if (schema_->types_[i] == 'S') {
                cols_[i] = new StringColumn();
            } else {
                pln(given_malformed_schema);
                exit(-1);
            }
        }
    }

    /** Create a data frame from an array of doubles and adds the one column dataframe to the
     * given KDStore
    */
    static DataFrame* fromArray(Key* key, KDStore* kdstore, size_t size, double* vals) {
        Schema schm("D");
        DataFrame* df = new DataFrame(schm);
        for(size_t i = 0; i < size; i++) {
            Row* row = new Row(schm);
            row->set(0, vals[i]);
            df->add_row(*row);
            delete row;
        }
        kdstore->put(*key, df);
        return df;
    }

    /** DataFrame destructor */
    ~DataFrame() {

        for (int i = 0; i < ncols(); i++) {
            delete cols_[i];
        }
        delete[] cols_;
        delete schema_;

    }

    /** Returns the dataframe's schema. Modifying the schema after a dataframe
      * has been created in undefined. */
    Schema &get_schema() {
        return *schema_;
    }

    /** Adds a column this dataframe, updates the schema, the new column
      * is external, and appears as the last column of the dataframe.
      * A nullptr colum is undefined. */
    void add_column(Column *col) {
        bool valid = col->size() == nrows() || nrows() == 0;
        exit_if_not(valid, invalid_col_size);
        if (nrows() == 0) length_ = col->size();
        // add column data to dataframe
        Column **grow_cols_ = new Column *[schema_->width() + 1];
        for (size_t col_idx = 0; col_idx < schema_->width(); col_idx++) {
            grow_cols_[col_idx] = cols_[col_idx];
        }
        grow_cols_[schema_->width()] = col;
        delete[] cols_;
        cols_ = grow_cols_;
        // add column  to schema
        schema_->add_column(col->get_type());
    }

    /** Return the value at the given column and row. Accessing rows or
     *  columns out of bounds, or request the wrong type is undefined.*/
    int get_int(size_t col, size_t row) {
        exit_if_not(col < ncols(), col_index_out_of_bounds);
        IntColumn *column = cols_[col]->as_int();
        exit_if_not(column != nullptr, non_int_col);
        exit_if_not(row < column->size(), row_index_out_of_bounds);
        return column->get(row);
    }

    bool get_bool(size_t col, size_t row) {
        exit_if_not(col < ncols(), col_index_out_of_bounds);
        BoolColumn *column = cols_[col]->as_bool();
        exit_if_not(column != nullptr, non_bool_col);
        exit_if_not(row < column->size(), row_index_out_of_bounds);
        return column->get(row);
    }

    double get_double(size_t col, size_t row) {
        exit_if_not(col < ncols(), col_index_out_of_bounds);
        DoubleColumn *column = cols_[col]->as_double();
        exit_if_not(column != nullptr, non_double_col);
        exit_if_not(row < column->size(), row_index_out_of_bounds);
        return column->get(row);
    }

    float get_float(size_t col, size_t row) {
        exit_if_not(col < ncols(), col_index_out_of_bounds);
        FloatColumn *column = cols_[col]->as_float();
        exit_if_not(column != nullptr, non_float_col);
        exit_if_not(row < column->size(), row_index_out_of_bounds);
        return column->get(row);
    }

    String *get_string(size_t col, size_t row) {
        exit_if_not(col < ncols(), col_index_out_of_bounds);
        StringColumn *column = cols_[col]->as_string();
        exit_if_not(column != nullptr, non_string_col);
        exit_if_not(row < column->size(), row_index_out_of_bounds);
        return column->get(row);
    }

    /** Set the value at the given column and row to the given value.
      * If the column is not  of the right type or the indices are out of
      * bound, the result is undefined. */
    void set(size_t col, size_t row, int val) {
        if (col >= ncols() || row >= nrows()) return;
        IntColumn *column = cols_[col]->as_int();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    void set(size_t col, size_t row, bool val) {
        if (col >= ncols() || row >= nrows()) return;
        BoolColumn *column = cols_[col]->as_bool();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    void set(size_t col, size_t row, double val) {
        if (col >= ncols() || row >= nrows()) return;
        DoubleColumn *column = cols_[col]->as_double();
        if (column == nullptr) return;
        else column->set(row, val);
    }

    void set(size_t col, size_t row, float val) {
        if (col >= ncols() || row >= nrows()) return;
        FloatColumn *column = cols_[col]->as_float();
        if (column == nullptr) return;
        else column->set(row, val);
    }

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
            if (schema_->col_type(col) == 'B') row.set(col, get_bool(col, idx));
            else if (schema_->col_type(col) == 'I') row.set(col, get_int(col, idx));
            else if (schema_->col_type(col) == 'D') row.set(col, get_double(col, idx));
            else if (schema_->col_type(col) == 'F') row.set(col, get_float(col, idx));
            else if (schema_->col_type(col) == 'S') row.set(col, get_string(col, idx));
        }
    }

    /** Add a row at the end of this dataframe. The row is expected to have
     *  the right schema and be filled with values, otherwise undedined.  */
    void add_row(Row &row) {
        for (size_t col = 0; col < ncols(); col++) {
            if (schema_->col_type(col) == 'B') cols_[col]->as_bool()->push_back(row.get_bool(col));
            else if (schema_->col_type(col) == 'I') cols_[col]->as_int()->push_back(row.get_int(col));
            else if (schema_->col_type(col) == 'D') cols_[col]->as_double()->push_back(row.get_double(col));
            else if (schema_->col_type(col) == 'F') cols_[col]->as_float()->push_back(row.get_float(col));
            else if (schema_->col_type(col) == 'S') cols_[col]->as_string()->push_back(row.get_string(col));
        }
        length_++;
    }

    /** The number of rows in the dataframe. */
    size_t nrows() {
        return length_;
    }

    /** The number of columns in the dataframe.*/
    size_t ncols() {
        return schema_->width();
    }

    /** Visit rows in order */
    void map(Rower &r) {
        Row *iterator_row = new Row(*schema_);
        for (size_t i = 0; i < nrows(); i++) {
            fill_row(i, *iterator_row);
            r.accept(*iterator_row);
        }
        delete iterator_row;
    }

    /** Create a new dataframe, constructed from rows for which the given Rower
      * returned true from its accept method. */
    DataFrame *filter(Rower &r) {
        DataFrame *nd = new DataFrame(*this);
        Row *iterator_row = new Row(*schema_);
        for (unsigned int i = 0; i < nrows(); i++) {
            fill_row(i, *iterator_row);
            if (r.accept(*iterator_row)) {
                nd->add_row(*iterator_row);
            }
        }
        delete iterator_row;
        return nd;
    }
};
