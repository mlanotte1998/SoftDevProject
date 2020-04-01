#pragma once
#include "message.h"
#include "../utility-classes/string.h"

/**
 * Directory::
 *
 * Serializable Directory class to be used by network
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Directory : public Message {
	public:
		size_t client_;
		size_t ports_count_;
		size_t* ports_;  // owned
		size_t addresses_count_;
		String** addresses_;  // owned; strings owned

		Directory(size_t sender, size_t target, size_t id, size_t client, size_t ports_count, size_t* ports, size_t addresses_count, String** addresses) {
			kind_ = MsgKind::Directory;
			sender_ = sender;
			target_ = target;
			id_ = id;
			client_ = client;
			ports_ = ports;
			ports_count_ = ports_count;
			addresses_ = addresses;
			addresses_count_ = addresses_count;
		}

};
