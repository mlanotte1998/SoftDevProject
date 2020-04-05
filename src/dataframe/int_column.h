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

    /***
    * Constructor for passing any number of arguments
    * @param n number of arguments
    * @param ... int values to be added in the array.
    */
    IntColumn(int n, ...) {
        int i;
        int val;
        va_list vl;
        // Make sure n is greater than 0
        if (n > 0) {
            // Then create an outer array that is the ceiling log of the n value + 1;
            // The best way to explain this is with an example. If n was 7, then there
            // will be 7 elements in the array. With our double list structure,
            // each inner list increases by * 2 , so they have 1 then 2 then 4 then 8 elements
            // and so on. For 7 you would have 1 + 2 + 4 = 7, so we need an upper array of size 3.
            // With the current function below you would get 3 for that value. but say n was 8
            // then you would need another sub array so the upper array needs to be size 4,
            // but the log2(8) is still 3, which is why it is ceil(log2(n + 1).
            binary_column_array_ = new int *[int(ceil(log2(n + 1)))];
            int *cur_array = new int[1];

            int cur_hor = 0;
            int cur_ver = 0;
            int cur_power = int(pow(2, cur_hor));
            va_start(vl, n);
            // Iterate through each element in the constructor.
            for (i = 0; i < n; i++) {
                // If the current vertical index is smaller than the current power, which is the
                // size of the current inner array, then add this value in and increase the vertical index.
                val = va_arg(vl, int);
                if (cur_ver < cur_power) {
                    cur_array[cur_ver] = val;
                    cur_ver++;
                }
                // Else, the current inner array is at capacity, so increase the horizontal counter
                // reset the vertical counter, and get the size of the newly needed array by
                // doing 2 to the power of the horizontal index we are at. This makes sure that
                // every time a new array is needed, it is twice the size of the old array.
                // Finally set the value into the new array and increase the vertical index.
                else {
                    binary_column_array_[cur_hor] = cur_array;
                    cur_hor++;
                    cur_ver = 0;
                    cur_power = int(pow(2, cur_hor));
                    cur_array = new int[cur_power];
                    cur_array[cur_ver] = val;
                    cur_ver++;
                }
            }
            // Incase the last sub array was not added  by hitting the else case on the
            // last iteration of the for loop, add it here.
            // Set the array length to be the number of indices plus 1.
            // the size is n, and end the va.
            binary_column_array_[cur_hor] = cur_array;
            array_length_ = cur_hor + 1;
            size_ = n;
            va_end(vl);
        }
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
