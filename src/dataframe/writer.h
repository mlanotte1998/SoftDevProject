#pragma once

#include "dataframe.h"
#include "row.h"

/***
 * Writer::
 * Writes to Dataframe.
 * Designed to be subclassed.
 * 
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Writer : public Object {
public:
	// virtual visit method
	virtual void visit(Row & r) {};

	// virtual done method
	virtual bool done() {
		return false;
	};

};
