// Lang: CwC

#pragma once
#include <stdlib.h>
#include "object.h"
#include "string.h"
#include "queue_node.h"

/**
  * A Dynamic First-In-First-Out data structure
  * that can hold an Object or a String.
  * This queue does not have a maximum capacity.
  * authors: horn.s@husky.neu.edu, armani.a@husky.neu.edu
  */
class ObjectQueue {
public:
  size_t size_;
  QueueNode* head;
  QueueNode* tail;

  // Constructor for the queue.
  ObjectQueue() {
    size_ = 0;
    head = nullptr;
    tail = nullptr;
  }

  // Destructor for the queue.
  ~ObjectQueue() {
    // start at head node of queue
    QueueNode* queue_head = head;
    QueueNode* temp;
    // iterates through all nodes in queue to delete all
    while(queue_head != nullptr) {
      temp = queue_head;
      queue_head = queue_head->next;
      delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size_ = 0;
  }

  // Add element toAdd to queue.
  void push(Object* toAdd) {
    QueueNode* element = new QueueNode(toAdd);
    // if queue is currently empty add object as head
    if (isEmpty()) {
      head = element;
    }
    // if queue has elements add object to tail
    else {
      tail->next = element;
      element->prev = tail;
    }
    // newly added object will always be the new tail
    tail = element;
    size_++;
  }

  // Removes element from front of Queue, returning the element.
  Object* pop() {
    // cannot pop from an empty queue
    if (isEmpty()) return nullptr;
    // get the head element's value
    QueueNode* element = head;
    Object* value = element->data;
    // reassign head to next value in queue
    head = element->next;
    // if popped node wasn't only node new head no longer has prev
    if (head != nullptr) head->prev = nullptr;
    // decrement queue size, delete previous head node and return value
    size_--;
    delete element;
    return value;
  }

  // Returns the size of this queue at the moment.
  size_t size() {
    return size_;
  }

  // Returns whether the queue has any elements.
  bool isEmpty() {
    return size_ == 0;
  }

  // Returns the element at the front of the queue without removal.
  Object* peek() {
    if (isEmpty()) return nullptr;
    Object* value = head->data;
    return value;
  }
};

/**
  * A Dynamic First-In-First-Out data structure
  * that can hold a String.
  * This queue does not have a maximum capacity.
  * authors: horn.s@husky.neu.edu, armani.a@husky.neu.edu
  */
class StringQueue {
public:
  // StringQueue implemented by composition using ObjectQueue
  ObjectQueue* queue;

  // Constructor for the queue.
  StringQueue() {
    queue = new ObjectQueue();
  }

  // Destructor for the queue.
  ~StringQueue() {
    delete queue;
  }

  // Add element toAdd to queue.
  void push(String* toAdd) {
    queue->push(toAdd);
  }

  // Removes element from front of Queue, returning the element.
  String* pop() {
    String* str_value = dynamic_cast<String*>(queue->pop());
    return str_value;
  }

  // Returns the size of this queue at the moment.
  size_t size() {
    return queue->size();
  }

  // Returns whether the queue has any elements.
  bool isEmpty() {
    return queue->isEmpty();
  }

  // Returns the element at the front of the queue without removal.
  String* peek() {
    String* str_value = dynamic_cast<String*>(queue->peek());
    return str_value;
  }
};
