//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "constants.h"

/*************************************************************************
 * BoolColumn::
 * Holds bool values.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class BoolColumn : public Column {
public:

    bool **binary_column_array_; // double array used so that inner objects are not copied

    /***
     * Basic constructor initializes the array to be able to hold one value to start.
     */
    BoolColumn() {
        // Create a first array of size 1 by 1.
        bool *first_array = new bool[1];
        binary_column_array_ = new bool *[1];
        binary_column_array_[0] = first_array;
        size_ = 0;
        array_length_ = 0;
    }

    /**
     * Constructor for building a column from a serialized string.
     * @param ser The serialized string for a Bool Column
     */
    BoolColumn(char *ser) {
        // Set initial column members.
        bool *first_array = new bool[1];
        binary_column_array_ = new bool *[1];
        binary_column_array_[0] = first_array;
        array_length_ = 0;
        size_ = 0;

        // Start tokenizing on spaces.
        char *ser_token = strtok(ser, " ");
        // Loop through the tokens.
        while (ser_token != NULL) {
            // Get to where the bool values are.
            // No need to read other items since we can just push back the list to build
            // up the entire array.
            if (strncmp("-p1_val::", ser_token, strlen("-p1_val::")) == 0) {
                int key_len = strlen("-p1_val::");
                int arr_len = strlen("arr(");
                // Create a char* for copying over the array values.
                char arr_value[MAX_ARRAY_SIZE_BYTES_LARGE];
                memset(arr_value, 0, MAX_ARRAY_SIZE_BYTES_LARGE);
                strncpy(
                        arr_value,
                        ser_token + key_len + arr_len,
                        strlen(ser_token) - key_len - arr_len - 1
                );
                // Go through tokens of the values that make up the array that are
                // separated by commas.
                char *arr_token = strtok(arr_value, ",");
                while (arr_token != NULL) {
                    // If the value is a 0 then add a false, else add true.
                    if (strncmp(arr_token, "0", 1) == 0) push_back(false);
                    else push_back(true);
                    arr_token = strtok(NULL, ",");
                }
            }
            ser_token = strtok(NULL, " ");
        }
    }

    /***
     * Destructor for the BoolColumn
     */
    ~BoolColumn() {
        // Delete each sub array then the upper array.
        for (unsigned int i = 0; i < array_length_; i++) {
            delete[] binary_column_array_[i];
        }
        if (size_ == 0) {
            delete[] binary_column_array_[0];
        }
        delete[] binary_column_array_;
    }

    /**
     * Gets the element at the given index.
     * @param idx the size_t representation of the desired index.
     * @return returns the boolean at the index or errors.
     */
    bool get(size_t idx) {
        // Use math to get the index of the upper array first, then use that value to
        // subtract from the index to get the overflow from the binary to get the inner array index.
        // Example: index 9 is 1001, so binary_array_index would be 4 because that is the size
        // of 9 in binary (where the leading 1 is), and then 9 - (2 to the power of (4 - 1)) gets 1
        // which is the remaining amount of the binary without the leading 1.
        // Also return an error if the idx is greater than the size.
        exit_if_not(idx < size_, bool_col_index_out_of_bounds);
        int binary_array_index = int(ceil(log2(idx + 2)));
        int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
        return binary_column_array_[binary_array_index - 1][inner_array_index];
    }


    /***
     * Overrides virtual method.
     * @return this
     */
    BoolColumn *as_bool() {
        return this;
    }

    /***
     * Adds an element to the end
     * @param val
     */
    void push_back(bool val) {
        // If size is 0, then add this as the first element.
        if (size_ == 0) {
            binary_column_array_[0][0] = val;
            size_++;
            array_length_++;
        }
            // Else if the array is at capacity meaning that the floor and log of the
            // size + 1 is the same (for example size 3 array has an array of 1 and 2 that are full,
            // 3 + 1 is 4, the log of 4 for both ceiling and floor is the same meaning this
            // is a power of 2.
        else if (int(ceil(log2(size_ + 1))) == int(floor(log2(size_ + 1)))) {
            bool **new_binary_array = new bool *[int(ceil(log2((size_ + 1) + 1)))];
            // Create a new outer array and add the sub arrays to it
            for (unsigned int i = 0; i < array_length_; i++) {
                new_binary_array[i] = binary_column_array_[i];
            }
            // Create the new sub array and add the new element to it.
            bool *new_inner_array = new bool[int(pow(2, array_length_ + 1))];
            new_inner_array[0] = val;
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
            binary_column_array_[binary_array_index - 1][inner_array_index] = val;
            size_++;
        }

    }

    /** Set value at idx. An out of bound idx is undefined.  */
    void set(size_t idx, bool val) {
        // Set the value at the index with a new value by using the same
        // math to find the index of the correct inner array.
        if (idx < size_) {
            int binary_array_index = int(ceil(log2(idx + 2)));
            int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
            binary_column_array_[binary_array_index - 1][inner_array_index] = val;
        }
        // out of bounds we throw an error.
        exit_if_not(idx < size_, bool_col_index_out_of_bounds);
    }

};
