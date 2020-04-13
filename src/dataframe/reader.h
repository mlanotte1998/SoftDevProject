#pragma once

#include "dataframe.h"
#include "row.h"

/***
 * Reader::
 * Reads from Dataframe.
 * Designed to be subclassed
 * 
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Reader : public Object {
public:
	// virtual visit method
	virtual bool visit(Row & r) {
		return false;
	};

	// virtual done method
	virtual bool done() {
		return false;
	};

};
