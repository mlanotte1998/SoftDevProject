#pragma once

#include "message.h"

/**
 * Ack::
 *
 * Serializable Ack Message class to be used by network
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Ack : public Message {
public:

    /** Ack constructor */
    Ack(size_t sender, size_t target, size_t id) {
        kind_ = MsgKind::Ack;
        sender_ = sender;
        target_ = target;
        id_ = id;
    }

    /** Constructs Ack from serialized Ack string */
    Ack(char *ser) : Message(ser) {};

};
