#include "object.h"

/**
 * QueueNode: A single node used to store data in a Queue.
 * This node is part of a doubly linked list within a Queue.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class QueueNode {
	public:
		Object* data;
		QueueNode* next;
		QueueNode* prev;


		// constructor for QueueNode
		// stores both next and previous to satisfy doubly linked functionality
		QueueNode(Object* val) {
			data = val;
			next = nullptr;
			prev = nullptr;
		}

		// destructor for QueueNode
		// removes all pointers to potential other nodes in the list
		~QueueNode() {
			data = nullptr;
			next = nullptr;
			prev = nullptr;
		}
};
