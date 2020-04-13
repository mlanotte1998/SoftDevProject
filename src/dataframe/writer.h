#include "dataframe.h"

/***
 * Writer::
 * Writes to Dataframe.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Writer : public Object {
public:
	virtual void visit(Row & r) {};

	virtual bool done() {};

};
