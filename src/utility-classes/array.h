#pragma once

#include <stdio.h>
#include "object.h"

using namespace std;

/**
 * Array: Represents a resizeable array.
 * Author: gomes.chri, modi.an
 */
class Array : public Object {
public:

    Object **array_; // array of objects held in this object.
    size_t size_; // size of the array
    size_t capacity_; // capacity of the array in memory as a member

    /**
     * Creates an empty array.
     * @return the array
     */
    Array() : Object() {
        // Starting capacity at 2 and then double when full
        array_ = new Object *[2];
        size_ = 0;
        capacity_ = 2;
    }

    /**
     * Deconstructs an instance of array.
     */
    ~Array() {
        delete[] array_;
    }

    /**
     * Adds an element to the end the array.
     * @arg e  element to add
     */
    void append(Object *e) {
        // If size is at capacity then double the capacity of the array.
        if (size_ >= capacity_) {
            Object **new_array = new Object *[capacity_ * 2];
            size_t i = 0;
            for (; i < capacity_; i++) {
                new_array[i] = array_[i];
            }
            // New element goes at index of the size since that is the next element in
            // the array due to indexes starting at 0.
            // Also multiply capacity.
            new_array[i] = e;
            // make sure to delete the old array aftet copying the elements over to the new one.
            delete [] array_;
            array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            array_[size_] = e;
            size_++;
        }
    }

    /**
     * Concats another array onto the end of this array.
     * @arg c  array to concat
     */
    void append_array(Array *c) {

        size_t inputListSize = c->size();
        size_t combinedSize = inputListSize + size_;
        size_t combinedCapacity;
        // Go with two times the highest size known for the two arrays
        // so that there is an array large enough to hold both sets of elements
        if (inputListSize > size_) {
            combinedCapacity = inputListSize * 2;
        } else {
            combinedCapacity = capacity_ * 2;
        }

        // Append objects from the other array on to the end of this one.
        Object **list = new Object *[combinedCapacity];
        for (unsigned int i = 0; i < combinedSize; i++) {
            // if combined size is less than the current size, then copying current array
            if (i < size_) {
                list[i] = get(i);
            }
            // else copy the elements from the other array
            else {
                list[i] = c->get(i - size_);
            }

        }
        // Deleting the array old array is important so that memory is not lost.
        delete[] array_;
        array_ = list;
        capacity_ = combinedCapacity;
        size_ = combinedSize;
    }

    /**
     * Adds an item at a specific index in the Array
     * @arg i  position to add the new item
     * @arg o  the new item
     */
    void add(size_t i, Object *o) {
        // If size is at capacity then need to double the capacity by creating a new array of double the size.
        if (size_ >= capacity_) {
            Object **new_array = new Object *[capacity_ * 2];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = array_[j - 1];
                }
            }
            // delete old array , update size and capacity
            delete[] array_;
            array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            // else create a new array of the known capacity to copy the elements over.
            Object **new_array = new Object *[capacity_];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = array_[j - 1];
                }
            }
            // delete old array , update size
            delete[] array_;
            array_ = new_array;
            size_++;
        }
    }

    /**
     * Removes all elements from the array.
     */
    void clear() {
        // clear the array and size, start out with a new array of size 2 again
        delete[] array_;
        size_ = 0;
        capacity_ = 2;
        array_ = new Object *[2];
    }

    /**
     * Checks if this array is equal to object given.
     * @arg o  object to compare
     * @return whether the two are equal
     */
    bool equals(Object *o) {
        // check if the object o is an array
        if (dynamic_cast<Array *>(o) != nullptr) {
            // if it is then cast it and see if they are the same size.
            // if they are the same size then check if the elements are the same
            // only if all elements are the same then this returns true.
            Array *other = dynamic_cast<Array *>(o);
            if (other->size() == size()) {
                for (unsigned int i = 0; i < size(); i++) {
                    if (!other->get(i)->equals(get(i))) {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    /**
     * Gets the element at a given index.
     * @arg index  index of the element to get
     * @return element at the index
     */
    Object *get(size_t index) {
        // return nullpointer if given bad index, otherwise return the item at that index.
        if (index > size() || index < 0) {
            return nullptr;
        } else {
            return array_[index];
        }
    }

    /**
     * Gets the index of the element given. If not in list,
     * returns size + 1.
     * @arg o  element to find in the list
     * @return index of the element if found
     */
    size_t index_of(Object *o) {
        // look for object by iterating through items in the list.
        // if not found then return the size + 1 as specified by the spec.
        for (unsigned int i = 0; i < size(); i++) {
            if (array_[i]->equals(o)) {
                return i;
            }
        }
        return size_ + 1;
    }

    /**
     * Removes the element at the given index. If invalid index,
     * does nothing and returns a null pointer.
     * @arg i  index to remove from
     * @return removed element
     */
    Object *remove(size_t i) {
        // create a nullptr to be the default return
        Object *removedObject = nullptr;

        // if i too big then return the nullptr
        if (i >= size_) {
            return removedObject;
        } else {
            // if it is good then iterate until you reach that index and then set the returned object
            // to the object at that index.
            // need to create a new array to pull the element out.
            Object **new_list = new Object *[size_ - 1];
            for (unsigned int j = 0; j < size_; j++) {
                if (j < i) {
                    new_list[j] = get(j);
                } else if (j == i) {
                    removedObject = get(i);
                } else {
                    new_list[j - 1] = get(j);
                }

            }
            delete [] array_; 
            array_ = new_list;
            size_ = size_ - 1;
        }
        return removedObject;
    }

    /**
     * Replaces the element at the index with given one. If
     * invalid index, does nothing and returns given element.
     * @arg i  index to replace the element at
     * @arg e  element to replace with
     * @return replaced element
     */
    Object *set(size_t i, Object *e) {
        // make sure index is is range then set pull the old object and set the new one before returning the old object.
        if (i < size() && i >= 0) {
            Object *old_object = array_[i];
            array_[i] = e;
            return old_object;

        }
        // if bad index then return given object.
        return e;
    }

    /**
     * Gets the size of the array.
     * @return the size
     */
    size_t size() {
        return size_;
    }

    /**
     * Hashes the array.
     * @return the hashed value
     */
    size_t hash() {
        size_t hash = 11;
        for (unsigned int i = 0; i < size(); i++) {
            hash = hash * 37 + array_[i]->hash();
        }
        return hash;
    }
};

/**
 * Array: Represents a resizeable int array.
 * Author: gomes.chri, modi.an
 */
class IntArray : public Object {
public:

    int *int_array_; // array of ints held in this object.
    size_t size_; // size of the array
    size_t capacity_; // capacity of the array in memory as a member

    /**
     * Creates an empty array.
     * @return the array
     */
    IntArray() : Object() {
        // Starting capacity at 2 and then double when full
        int_array_ = new int[2];
        size_ = 0;
        capacity_ = 2;
    }

    /**
     * Deconstructs an instance of array.
     */
    ~IntArray() {
        delete[] int_array_;
    }

    /**
     * Adds an element to the end the array.
     * @arg e  element to add
     */
    void append(int e) {
        // If size is at capacity then double the capacity of the array.
        if (size_ >= capacity_) {
            int *new_array = new int[capacity_ * 2];
            size_t i = 0;
            for (; i < capacity_; i++) {
                new_array[i] = int_array_[i];
            }
            // New element goes at index of the size since that is the next element in
            // the array due to indexes starting at 0.
            // Also multiply capacity.
            new_array[i] = e;
            // make sure to delete the old array aftet copying the elements over to the new one.
            delete int_array_;
            int_array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            int_array_[size_] = e;
            size_++;
        }
    }

    /**
     * Concats another array onto the end of this array.
     * @arg c  array to concat
     */
    void append_array(IntArray *c) {
        size_t inputListSize = c->size();
        size_t combinedSize = inputListSize + size_;
        size_t combinedCapacity;
        // Go with two times the highest size known for the two arrays
        // so that there is an array large enough to hold both sets of elements
        if (inputListSize > size_) {
            combinedCapacity = inputListSize * 2;
        } else {
            combinedCapacity = capacity_ * 2;
        }

        // Append ints from the other array on to the end of this one.
        int *list = new int[combinedCapacity];
        for (unsigned int i = 0; i < combinedSize; i++) {
            // if combined size is less than the current size, then copying current array
            if (i < size_) {
                list[i] = get(i);
                // else copy the elements from the other array
            } else {
                list[i] = c->get(i - size_);
            }

        }
        // Deleting the array old array is important so that memory is not lost.
        delete[] int_array_;
        int_array_ = list;
        capacity_ = combinedCapacity;
        size_ = combinedSize;
    }

    /**
     * Adds an item at a specific index in the Array
     * @arg i  position to add the new item
     * @arg o  the new item
     */
    void add(size_t i, int o) {
        // If size is at capacity then need to double the capacity by creating a new array of double the size.
        if (size_ >= capacity_) {
            int *new_array = new int[capacity_ * 2];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = int_array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = int_array_[j - 1];
                }
            }
            // delete old array , update size and capacity
            delete[] int_array_;
            int_array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            // else create a new array of the known capacity to copy the elements over.
            int *new_array = new int[capacity_];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = int_array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = int_array_[j - 1];
                }
            }
            // delete old array , update size
            delete[] int_array_;
            int_array_ = new_array;
            size_++;
        }
    }

    /**
     * Removes all elements from the array.
     */
    void clear() {
        // clear the array and size, start out with a new array of size 2 again
        delete[] int_array_;
        size_ = 0;
        capacity_ = 2;
        int_array_ = new int[2];
    }

    /**
     * Checks if this array is equal to object given.
     * @arg o  object to compare
     * @return whether the two are equal
     */
    bool equals(Object *o) {
        // check if the object o is an array
        if (dynamic_cast<IntArray *>(o) != nullptr) {
            // if it is then cast it and see if they are the same size.
            // if they are the same size then check if the elements are the same
            // only if all elements are the same then this returns true.
            IntArray *other = dynamic_cast<IntArray *>(o);
            if (other->size() == size()) {
                for (unsigned int i = 0; i < size(); i++) {
                    if (other->get(i) != get(i)) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Gets the element at a given index.
     * @arg index  index of the element to get
     * @return element at the index
     */
    int get(size_t index) {
        if (index > size() || index < 0) {
            // Spec group said to return -1 maybe but it was ultimately up to us, and
            // since -1 is a valid int, we went with NULL instead.
            return NULL;
        } else {
            return int_array_[index];
        }
    }

    /**
     * Gets the index of the element given. If not in list,
     * returns size + 1.
     * @arg o  element to find in the list
     * @return index of the element if found
     */
    size_t index_of(int o) {
        // look for int by iterating through items in the list.
        // if not found then return the size + 1 as specified by the spec.
        for (unsigned int i = 0; i < size(); i++) {
            if (int_array_[i] == o) {
                return i;
            }
        }
        return size_ + 1;
    }

    /**
     * Removes the element at the given index. If invalid index,
     * does nothing and returns a null pointer.
     * @arg i  index to remove from
     * @return removed element
     */
    int remove(size_t i) {
        // Spec group said to return -1 maybe but it was ultimately up to us, and
        // since -1 is a valid int, we went with NULL instead.
        int removedInt;

        if (i >= size_ || i < 0) {
            return removedInt;
        } else {
            // if it is good then iterate until you reach that index and then set the returned int
            // to the int at that index.
            // need to create a new array to pull the element out.
            int *new_list = new int[size_];
            for (unsigned int j = 0; j < size_; j++) {
                if (j < i) {
                    new_list[j] = get(j);
                } else if (j == i) {
                    removedInt = get(i);
                } else {
                    new_list[j - 1] = get(j);
                }

            }
            delete [] int_array_;
            int_array_ = new_list;
            size_ = size_ - 1;
        }
        return removedInt;
    }

    /**
     * Replaces the element at the index with given one. If
     * invalid index, does nothing and returns given element.
     * @arg i  index to replace the element at
     * @arg e  element to replace with
     * @return replaced element
     */
    int set(size_t i, int e) {
        // make sure index is is range then set pull the old int and set the new one before returning the old int.
        if (i < size()) {
            int old_int = int_array_[i];
            int_array_[i] = e;
            return old_int;
        }
        // if bad index then return given int.
        return e;
    }

    /**
     * Gets the size of the array.
     * @return the size
     */
    size_t size() {
        return size_;
    }

    /**
     * Hashes the array.
     * @return the hashed value
     */
    size_t hash() {
        size_t hash = 11;
        for (unsigned int i = 0; i < size(); i++) {
            hash = hash * 37 + int_array_[i];
        }
        return hash;
    }
};

/**
 * Array: Represents a resizeable float array.
 * Author: gomes.chri, modi.an
 */
class FloatArray : public Object {
public:


    float *float_array_; // array of floats held in this object.
    size_t size_; // size of the array
    size_t capacity_; // capacity of the array in memory as a member

    /**
     * Creates an empty array.
     * @return the array
     */
    FloatArray() : Object() {
        // Starting capacity at 2 and then double when full
        float_array_ = new float[2];
        size_ = 0;
        capacity_ = 2;
    }

    /**
     * Deconstructs an instance of array.
     */
    ~FloatArray() {
        delete[] float_array_;
    }

    /**
     * Adds an element to the end the array.
     * @arg e  element to add
     */
    void append(float e) {
        // If size is at capacity then double the capacity of the array.
        if (size_ >= capacity_) {
            float *new_array = new float[capacity_ * 2];
            size_t i = 0;
            for (; i < capacity_; i++) {
                new_array[i] = float_array_[i];
            }
            // New element goes at index of the size since that is the next element in
            // the array due to indexes starting at 0.
            // Also multiply capacity.
            new_array[i] = e;
            // make sure to delete the old array aftet copying the elements over to the new one.
            delete float_array_;
            float_array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            float_array_[size_] = e;
            size_++;
        }
    }

    /**
     * Concats another array onto the end of this array.
     * @arg c  array to concat
     */
    void append_array(FloatArray *c) {
        size_t inputListSize = c->size();
        size_t combinedSize = inputListSize + size_;
        size_t combinedCapacity;
        // Go with two times the highest size known for the two arrays
        // so that there is an array large enough to hold both sets of elements
        if (inputListSize > size_) {
            combinedCapacity = inputListSize * 2;
        } else {
            combinedCapacity = capacity_ * 2;
        }

        // Append floats from the other array on to the end of this one.
        float *list = new float[combinedCapacity];
        for (unsigned int i = 0; i < combinedSize; i++) {
            // if combined size is less than the current size, then copying current array
            if (i < size_) {
                list[i] = get(i);
                // else copy the elements from the other array
            } else {
                list[i] = c->get(i - size_);
            }

        }
        // Deleting the array old array is important so that memory is not lost.
        delete[] float_array_;
        float_array_ = list;
        capacity_ = combinedCapacity;
        size_ = combinedSize;
    }

    /**
     * Adds an item at a specific index in the Array
     * @arg i  position to add the new item
     * @arg o  the new item
     */
    void add(size_t i, float o) {
        // If size is at capacity then need to double the capacity by creating a new array of double the size.
        if (size_ >= capacity_) {
            float *new_array = new float[capacity_ * 2];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = float_array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = float_array_[j - 1];
                }
            }
            // delete old array , update size and capacity
            delete[] float_array_;
            float_array_ = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            // else create a new array of the known capacity to copy the elements over.
            float *new_array = new float[capacity_];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = float_array_[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = float_array_[j - 1];
                }
            }
            // delete old array , update size
            delete[] float_array_;
            float_array_ = new_array;
            size_++;
        }
    }

    /**
     * Removes all elements from the array.
     */
    void clear() {
        // clear the array and size, start out with a new array of size 2 again
        delete[] float_array_;
        size_ = 0;
        capacity_ = 2;
        float_array_ = new float[2];
    }

    /**
     * Checks if this array is equal to object given.
     * @arg o  object to compare
     * @return whether the two are equal
     */
    bool equals(Object *o) {
        // check if the object o is a FloatArray
        if (dynamic_cast<FloatArray *>(o) != nullptr) {
            // if it is then cast it and see if they are the same size.
            // if they are the same size then check if the elements are the same
            // only if all elements are the same then this returns true.
            FloatArray *other = dynamic_cast<FloatArray *>(o);
            if (other->size() == size()) {
                for (unsigned int i = 0; i < size(); i++) {
                    if (!float_equal(other->get(i), get(i))) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Gets the element at a given index.
     * @arg index  index of the element to get
     * @return element at the index
     */
    float get(size_t index) {
        if (index > size() || index < 0) {
            // Spec group said to return -1 maybe but it was ultimately up to us, and
            // since -1 is a valid int, we went with NULL instead.
            return NULL;
        } else {
            return float_array_[index];
        }
    }

    /**
     * Gets the index of the element given. If not in list,
     * returns size + 1.
     * @arg o  element to find in the list
     * @return index of the element if found
     */
    size_t index_of(float o) {
        // look for float by iterating through items in the list.
        // if not found then return the size + 1 as specified by the spec.
        for (unsigned int i = 0; i < size(); i++) {
            if (float_equal(float_array_[i], o)) {
                return i;
            }
        }
        return size_ + 1;
    }

    /**
     * Removes the element at the given index. If invalid index,
     * does nothing and returns a null pointer.
     * @arg i  index to remove from
     * @return removed element
     */
    float remove(size_t i) {
        // Spec group said to return -1 maybe but it was ultimately up to us, and
        // since -1 is a valid float, we went with NULL instead.
        float removedFloat;

        if (i >= size_ || i < 0) {
            return removedFloat;
        } else {
            float *new_list = new float[size_];
            for (unsigned int j = 0; j < size_; j++) {
                if (j < i) {
                    new_list[j] = get(j);
                } else if (j == i) {
                    removedFloat = get(i);
                } else {
                    new_list[j - 1] = get(j);
                }

            }

            delete [] float_array_;
            float_array_ = new_list;
            size_ = size_ - 1;
        }
        return removedFloat;
    }

    /**
     * Replaces the element at the index with given one. If
     * invalid index, does nothing and returns given element.
     * @arg i  index to replace the element at
     * @arg e  element to replace with
     * @return replaced element
     */
    float set(size_t i, float e) {
        // make sure index is is range then set pull the old float and set the new one before returning the old float.
        if (i < size()) {
            float old_float = float_array_[i];
            float_array_[i] = e;
            return old_float;
        }
        // if bad index then return given float.
        return e;
    }

    /**
     * Gets the size of the array.
     * @return the size
     */
    size_t size() {
        return size_;
    }

    /**
     * Hashes the array.
     * @return the hashed value
     */
    size_t hash() {
        size_t hash = 11;
        for (unsigned int i = 0; i < size(); i++) {
            hash = hash * 37 + float_array_[i];
        }
        return hash;
    }

    bool float_equal(float f1, float f2) {
        float eps = 0.0000001;
        if (f1 > f2) {
            return f1 - f2 < eps;
        } else {
            return f2 - f1 < eps;
        }
    }
};

/**
 * Array: Represents a resizeable bool array.
 * Author: gomes.chri, modi.an
 */
class BoolArray : public Object {
public:

    bool *bool_array; // array of bools held in this object.
    size_t size_; // size of the array
    size_t capacity_; // capacity of the array in memory as a member

    /**
     * Creates an empty array.
     * @return the array
     */
    BoolArray() : Object() {
        // Starting capacity at 2 and then double when full
        bool_array = new bool[2];
        size_ = 0;
        capacity_ = 2;
    }

    /**
     * Deconstructs an instance of array.
     */
    ~BoolArray() {
        delete[] bool_array;
    }

    /**
     * Adds an element to the end the array.
     * @arg e  element to add
     */
    void append(bool e) {
        // If size is at capacity then double the capacity of the array.
        if (size_ >= capacity_) {
            bool *new_array = new bool[capacity_ * 2];
            size_t i = 0;
            for (; i < capacity_; i++) {
                new_array[i] = bool_array[i];
            }
            // New element goes at index of the size since that is the next element in
            // the array due to indexes starting at 0.
            // Also multiply capacity.
            new_array[i] = e;
            // make sure to delete the old array aftet copying the elements over to the new one.
            delete [] bool_array;
            bool_array = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            bool_array[size_] = e;
            size_++;
        }
    }

    /**
     * Concats another array onto the end of this array.
     * @arg c  array to concat
     */
    void append_array(BoolArray *c) {
        size_t inputListSize = c->size();
        size_t combinedSize = inputListSize + size_;
        size_t combinedCapacity;
        // Go with two times the highest size known for the two arrays
        // so that there is an array large enough to hold both sets of elements
        if (inputListSize > size_) {
            combinedCapacity = inputListSize * 2;
        } else {
            combinedCapacity = capacity_ * 2;
        }

        // Append bools from the other array on to the end of this one.
        bool *list = new bool[combinedCapacity];
        // if combined size is less than the current size, then copying current array
        for (unsigned int i = 0; i < combinedSize; i++) {
            if (i < size_) {
                list[i] = get(i);
            }
            // else copy the elements from the other array
            else {
                list[i] = c->get(i - size_);
            }

        }
        // Deleting the array old array is important so that memory is not lost.
        delete[] bool_array;
        bool_array = list;
        capacity_ = combinedCapacity;
        size_ = combinedSize;
    }

    /**
     * Adds an item at a specific index in the Array
     * @arg i  position to add the new item
     * @arg o  the new item
     */
    void add(size_t i, bool o) {
        // If size is at capacity then need to double the capacity by creating a new array of double the size.
        if (size_ >= capacity_) {
            bool *new_array = new bool[capacity_ * 2];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = bool_array[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = bool_array[j - 1];
                }
            }
            // delete old array , update size and capacity
            delete[] bool_array;
            bool_array = new_array;
            size_++;
            capacity_ *= 2;
        } else {
            // else create a new array of the known capacity to copy the elements over.
            bool *new_array = new bool[capacity_];
            // if j is less than the i index then add that element from the old array to the new one
            // if j = i then add the element to be added
            // else add the elements from the old array with j - 1 to account for the element added at i
            for (unsigned int j = 0; j < size_ + 1; j++) {
                if (j < i) {
                    new_array[j] = bool_array[j];
                } else if (j == i) {
                    new_array[j] = o;
                } else {
                    new_array[j] = bool_array[j - 1];
                }
            }
            // delete old array , update size
            delete[] bool_array;
            bool_array = new_array;
            size_++;
        }
    }

    /**
     * Removes all elements from the array.
     */
    void clear() {
        // clear the array and size, start out with a new array of size 2 again
        delete[] bool_array;
        size_ = 0;
        capacity_ = 2;
        bool_array = new bool[2];
    }

    /**
     * Checks if this array is equal to object given.
     * @arg o  object to compare
     * @return whether the two are equal
     */
    bool equals(Object *o) {
        // check if the object o is an BoolArray
        if (dynamic_cast<BoolArray *>(o) != nullptr) {
            // if it is then cast it and see if they are the same size.
            // if they are the same size then check if the elements are the same
            // only if all elements are the same then this returns true.
            BoolArray *other = dynamic_cast<BoolArray *>(o);
            if (other->size() == size()) {
                for (unsigned int i = 0; i < size(); i++) {
                    if (!other->get(i) == (get(i))) {
                        return false;
                    }
                }
                return true;
            }
        }
        return false;
    }

    /**
     * Gets the element at a given index.
     * @arg index  index of the element to get
     * @return element at the index
     */
    bool get(size_t index) {
        if (index > size() || index < 0) {
            // Spec group said to return false maybe but it was ultimately up to us, and
            // since false is a valid int, we went with NULL instead.
            return NULL;
        } else {
            return bool_array[index];
        }
    }

    /**
     * Gets the index of the element given. If not in list,
     * returns size + 1.
     * @arg o  element to find in the list
     * @return index of the element if found
     */
    size_t index_of(bool o) {
        // look for bool by iterating through items in the list.
        // if not found then return the size + 1 as specified by the spec.
        for (unsigned int i = 0; i < size(); i++) {
            if (bool_array[i] == o) {
                return i;
            }
        }
        return size_ + 1;
    }

    /**
     * Removes the element at the given index. If invalid index,
     * does nothing and returns a null pointer.
     * @arg i  index to remove from
     * @return removed element
     */
    bool remove(size_t i) {
        // Spec group said to return false maybe but it was ultimately up to us, and
        // since false is a valid int, we went with NULL instead.
        bool removedBool;

        if (i >= size_ || i < 0) {
            return removedBool;
        } else {
            bool *new_list = new bool[size_ - 1];
            for (unsigned int j = 0; j < size_; j++) {
                if (j < i) {
                    new_list[j] = get(j);
                } else if (j == i) {
                    removedBool = get(i);
                } else {
                    new_list[j - 1] = get(j);
                }

            }
            delete [] bool_array;
            bool_array = new_list;
            size_ = size_ - 1;
        }
        return removedBool;
    }

    /**
     * Replaces the element at the index with given one. If
     * invalid index, does nothing and returns given element.
     * @arg i  index to replace the element at
     * @arg e  element to replace with
     * @return replaced element
     */
    bool set(size_t i, bool e) {
        // make sure index is is range then set pull the old bool and set the new one before returning the old bool.
        if (i < size() && i >= 0) {
            bool old_bool = bool_array[i];
            bool_array[i] = e;
            return old_bool;

        }
        return e;
    }

    /**
     * Gets the size of the array.
     * @return the size
     */
    size_t size() {
        return size_;
    }

    /**
     * Hashes the array.
     * @return the hashed value
     */
    size_t hash() {
        size_t hash = 11;
        for (unsigned int i = 0; i < size(); i++) {
            int one_or_zero = 0;
            if (bool_array[i]) { one_or_zero = 1; }
            else { one_or_zero = 0; }
            hash = hash * 37 + one_or_zero;
        }
        return hash;
    }
};
