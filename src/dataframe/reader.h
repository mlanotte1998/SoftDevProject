#pragma once

#include "dataframe.h"
#include "row.h"

/***
 * Reader::
 * Reads from Dataframe.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Reader : public Object {
public:
	virtual bool visit(Row & r) {
		return false;
	};

	virtual bool done() {
		return false;
	};

};
