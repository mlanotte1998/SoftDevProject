#pragma once
#include "message.h"

class Ack : public Message {
	public:

		Ack(size_t sender, size_t target, size_t id) {
			kind_ = MsgKind::Ack;
			sender_ = sender;
			target_ = target;
			id_ = id;
		}

		Ack(char* ser) : Message(ser){};
		
};
