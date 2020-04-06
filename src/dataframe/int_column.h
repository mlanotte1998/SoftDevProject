//
// Created by Michael Lanotte and Daniel Welch on 2/11/20.
//

#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include "constants.h"

/***
 * IntColumn::
 * Holds int values.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class IntColumn : public Column {
public:

    int **binary_column_array_; // double array used so that inner objects are not copied

    /***
     * Basic constructor initializes the array to be able to hold one value to start.
     */
    IntColumn() {
        // Create a first array of size 1 by 1.
        int *first_array = new int[1];
        binary_column_array_ = new int *[1];
        binary_column_array_[0] = first_array;
        size_ = 0;
        array_length_ = 0;
    }

    /**
     * Constructor for building a int column from a serialized string.
     * @param ser The serialized string for a Int Column
     */
    IntColumn(char* ser) {
        // Set initial members
        int *first_array = new int[1];
        binary_column_array_ = new int *[1];
        binary_column_array_[0] = first_array;
        array_length_ = 0;
        size_ = 0;

        // Split up the message by spaces.
        char* ser_token = strtok(ser, " ");
        // Loop through until reaching the p1 that represents the inner array.
        while(ser_token != NULL) {
            if (strncmp("-p1_val::", ser_token, strlen("-p1_val::")) == 0) {
                int key_len = strlen("-p1_val::");
                int arr_len = strlen("arr(");
                char arr_value[MAX_ARRAY_SIZE_BYTES_LARGE];
                memset(arr_value, 0, MAX_ARRAY_SIZE_BYTES_LARGE);
                // Copy the array elements to a new string.
                strncpy(
                    arr_value,
                    ser_token + key_len + arr_len,
                    strlen(ser_token) - key_len - arr_len - 1
                );
                // Tokenize the string representing the inner array by the commas
                // that separate the values.
                char* arr_token = strtok(arr_value, ",");
                while(arr_token != NULL) {
                    int index_value = 0;
                    sscanf(arr_token, "%i", &index_value);
                    // Push back each value in the string to build up the entire Int Column.
                    // No need to parse out the other information from the message because the
                    // values like the length of the array will be build by the push backs.
                    push_back(index_value);
                    arr_token = strtok(NULL, ",");
                }
            }
            ser_token = strtok(NULL, " ");
        }
    }

    /***
     * Destructor for the IntColumn
     */
    ~IntColumn() {
        // Delete each sub array then the upper array.
        for (unsigned int i = 0; i < array_length_; i++) {
            delete[] binary_column_array_[i];
        }
        if (size_ == 0) {
          delete [] binary_column_array_[0];
        }
        delete[] binary_column_array_;
    }

    /**
     * Gets the element at the given index.
     * @param idx the size_t representation of the desired index.
     * @return returns the int at the index or errors.
     */
    int get(size_t idx) {
        // Use math to get the index of the upper array first, then use that value to
        // subtract from the index to get the overflow from the binary to get the inner array index.
        // Example: index 9 is 1001, so binary_array_index would be 4 because that is the size
        // of 9 in binary (where the leading 1 is), and then 9 - (2 to the power of (4 - 1)) gets 1
        // which is the remaining amount of the binary without the leading 1.
        // Also return an error if the idx is greater than the size.
        exit_if_not(idx < size_, int_col_index_out_of_bounds);
        int binary_array_index = int(ceil(log2(idx + 2)));
        int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
        return binary_column_array_[binary_array_index - 1][inner_array_index];
    }

    /***
     * Overrides virtual method.
     * @return this
     */
    IntColumn *as_int() {
        return this;
    }

    /***
    * Adds an element to the end
    * @param val
    */
    void push_back(int val) {
        // If size is 0, then add this as the first element.
        if (size_ == 0) {
            binary_column_array_[0][0] = val;
            size_++;
            array_length_++;
        } else if (int(ceil(log2(size_ + 1))) == int(floor(log2(size_ + 1)))) {
            // Else if the array is at capacity meaning that the floor and log of the
            // size + 1 is the same (for example size 3 array has an array of 1 and 2 that are full,
            // 3 + 1 is 4, the log of 4 for both ceiling and floor is the same meaning this
            // is a power of 2.
            int **new_binary_array = new int *[int(ceil(log2((size_ + 1) + 1)))];
            // Create a new outer array and add the sub arrays to it
            for (unsigned int i = 0; i < array_length_; i++) {
                new_binary_array[i] = binary_column_array_[i];
            }
            // Create the new sub array and add the new element to it.
            int *new_inner_array = new int[int(pow(2, array_length_ + 1))];
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
    void set(size_t idx, int val) {
        // Set the value at the index with a new value by using the same
        // math to find the index of the correct inner array.
        if (idx < size_) {
            int binary_array_index = int(ceil(log2(idx + 2)));
            int inner_array_index = idx + 1 - int(pow(2, binary_array_index - 1));
            binary_column_array_[binary_array_index - 1][inner_array_index] = val;
        }
        // out of bounds we throw an error.
        exit_if_not(idx < size_, int_col_index_out_of_bounds );
    }

};
