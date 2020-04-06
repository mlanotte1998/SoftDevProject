//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include "column.h"
#include "../utility-classes/string.h"
#include "constants.h"

/*************************************************************************
 * StringColumn::
 * Holds string pointers. The strings are external.  Nullptr is a valid
 * value.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class StringColumn : public Column {
public:

    String ***binary_column_array_; // double array used so that inner objects are not copied

    /***
     * Basic constructor initializes the array to be able to hold one value to start.
     */
    StringColumn() {
        // Create a first array of size 1 by 1.
        String **first_array = new String *[1];
        binary_column_array_ = new String **[1];
        binary_column_array_[0] = first_array;
        size_ = 0;
        array_length_ = 0;
    }

    /**
     * Constructor for building a string column from a serialized string.
     * @param ser The serialized string for a string Column
     */
    StringColumn(char *ser) {
        // Set initial members
        String **first_array = new String *[1];
        binary_column_array_ = new String **[1];
        binary_column_array_[0] = first_array;
        array_length_ = 0;
        size_ = 0;

        // get start of array
        char *ser_array = strstr(ser, "-p1_val");
        char *ser_array_end = strstr(ser_array, "\"])");
        int ser_array_length = ser_array_end - ser_array;

        // first token is entire array
        char *ser_token = ser_array;
        char *ser_token_end;
        int ser_token_length;

        // buffer used to add char* to a string
        int buffer_size = ser_array_length;
        char buffer[buffer_size];
        memset(buffer, 0, buffer_size);

        // Loop through tokens
        while (ser_token != NULL) {
            // get beginning of token
            ser_token = strstr(ser_token, "[\"");

            // get end of token
            ser_token_end = strstr(ser_token, "\"],");
            if (ser_token_end == NULL) ser_token_end = strstr(ser_token, "\"])");

            // calculate token length
            ser_token_length = ser_token_end - ser_token - strlen("[\"");

            // add token to buffer
            memset(buffer, 0, buffer_size);
            for (int i = 0; i < ser_token_length; i++) {
                buffer[i] = ser_token[i + 2];
            }

            // create string using token buffer and add to column
            String *add_str = new String(buffer);
            push_back(add_str);

            delete add_str;
            // move to next token
            ser_token++;
            ser_token = strstr(ser_token, "\"],[\"");
        }
    }

    /***
     * Destructor for the BoolColumn
     */
    ~StringColumn() {
        size_t cur_index = 0;
        size_t cur_power = 1;

        // Delete each sub array then the upper array.
        // Also need to delete the strings inside since they were cloned
        for (unsigned int i = 0; i < array_length_; i++) {
            for (unsigned int j = 0; j < cur_power; j++) {
                if (cur_index < size_) {
                    if (binary_column_array_ != nullptr) {
                        delete binary_column_array_[i][j];
                    }
                }
                cur_index++;
            }
            cur_power *= 2;
            delete[] binary_column_array_[i];

        }
        if (size_ == 0) {
            delete[] binary_column_array_[0];
        }
        delete[] binary_column_array_;
    }

    /** Return column as string.*/
    StringColumn *as_string() {
        return this;
    }

    /** Returns the string at idx; undefined on invalid idx.*/
    /**
     * Gets the element at the given index.
     * @param idx the size_t representation of the desired index.
     * @return returns the string at the index or errors.
     */
    String *get(size_t idx) {
        // Use math to get the index of the upper array first, then use that value to
        // subtract from the index to get the overflow from the binary to get the inner array index.
        // Example: index 9 is 1001, so binary_array_index would be 4 because that is the size
        // of 9 in binary (where the leading 1 is), and then 9 - (2 to the power of (4 - 1)) gets 1
        // which is the remaining amount of the binary without the leading 1.
        // Also return an error if the idx is greater than the size.
        exit_if_not(idx < size_, string_col_index_out_of_bounds);
        int binary_array_index = int(ceil(log2(idx + 2)));
        int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
        return binary_column_array_[binary_array_index - 1][inner_array_index];
    }

    /***
     * Adds an element to the end
     * @param val
     */
    void push_back(String *val) {
        // If size is 0, then add this as the first element.
        if (size_ == 0) {
            binary_column_array_[0][0] = val->clone();
            size_++;
            array_length_++;
        }
            // Else if the array is at capacity meaning that the floor and log of the
            // size + 1 is the same (for example size 3 array has an array of 1 and 2 that are full,
            // 3 + 1 is 4, the log of 4 for both ceiling and floor is the same meaning this
            // is a power of 2.
        else if (int(ceil(log2(size_ + 1))) == int(floor(log2(size_ + 1)))) {
            String ***new_binary_array = new String **[int(ceil(log2((size_ + 1) + 1)))];
            // Create a new outer array and add the sub arrays to it
            for (unsigned int i = 0; i < array_length_; i++) {
                new_binary_array[i] = binary_column_array_[i];
            }
            // Create the new sub array and add the new element to it.
            String **new_inner_array = new String *[int(pow(2, array_length_ + 1))];
            new_inner_array[0] = val->clone();
            new_binary_array[array_length_] = new_inner_array;
            size_++;
            array_length_++;
            // delete the old outer array and set the new one as a member.
            delete[] binary_column_array_;
            binary_column_array_ = new_binary_array;
        } else {
            // Else the new element can be added without needing to grow,
            // so find the index that it should go in the same way
            // the the get method works.
            int binary_array_index = int(ceil(log2(size_ + 2)));
            int inner_array_index = size_ + 1 - int(pow(2, binary_array_index - 1));
            binary_column_array_[binary_array_index - 1][inner_array_index] = val->clone();
            size_++;
        }
    }

    /** Set value at idx. Out of bound idx is undefined. */
    void set(size_t idx, String *val) {
        // Set the value at the index with a new value by using the same
        // math to find the index of the correct inner array.
        if (idx < size_) {
            int binary_array_index = int(ceil(log2(idx + 2)));
            int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
            delete binary_column_array_[binary_array_index - 1][inner_array_index];
            binary_column_array_[binary_array_index - 1][inner_array_index] = val->clone();
        }
        // out of bounds we throw an error.
        exit_if_not(idx < size_, string_col_index_out_of_bounds);
    }
};
