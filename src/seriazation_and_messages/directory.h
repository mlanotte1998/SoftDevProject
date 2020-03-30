#pragma once
#include "message.h"
#include "string.h"

class Directory : public Message {
	size_t client;
	size_t * ports;  // owned
	String ** addresses;  // owned; strings owned

};
