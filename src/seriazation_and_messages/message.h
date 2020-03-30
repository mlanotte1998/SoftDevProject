#pragma once

#include "object.h"
#include "constants.h"

enum class MsgKind {
    Ack, Nack, Put, Reply, Get, WaitAndGet, Status, Kill, Register, Directory
};

class Message : public Object {
public:
    MsgKind kind_;  // the message kind
    size_t sender_; // the index of the sender node
    size_t target_; // the index of the receiver node
    size_t id_;     // an id t unique within the node

    Message() {

    }

    Message(MsgKind kind, size_t sender, size_t target, size_t id) {
        kind_ = kind;
        sender_ = sender;
        target_ = target;
        id_ = id;
    }

    // Deserialize constructor
    Message(char *ser) {
        // Iterate through tokens
        char *ser_token = strtok(ser, " ");
        while (ser_token != NULL) {
            // If the p1 val is found
            if (strncmp("-p1_val::", ser_token, strlen("-p1_val::")) == 0) {
                // extract the value that represents the kind of message.
                int key_len = strlen("-p1_val::");
                char kind_value[MAX_MSGKIND_BYTES];
                memset(kind_value, 0, MAX_MSGKIND_BYTES);
                strncpy(
                        kind_value,
                        ser_token + key_len,
                        strlen(ser_token) - key_len
                );
                // Check which kind the message is and set the kind_ member.
                if (strncmp(kind_value, "Ack", 3) == 0) kind_ = MsgKind::Ack;
                else if (strncmp(kind_value, "Nack", 4) == 0) kind_ = MsgKind::Nack;
                else if (strncmp(kind_value, "Put", 3) == 0) kind_ = MsgKind::Put;
                else if (strncmp(kind_value, "Reply", 5) == 0) kind_ = MsgKind::Reply;
                else if (strncmp(kind_value, "Get", 3) == 0) kind_ = MsgKind::Get;
                else if (strncmp(kind_value, "WaitAndGet", 10) == 0) kind_ = MsgKind::WaitAndGet;
                else if (strncmp(kind_value, "Status", 6) == 0) kind_ = MsgKind::Status;
                else if (strncmp(kind_value, "Kill", 4) == 0) kind_ = MsgKind::Kill;
                else if (strncmp(kind_value, "Register", 8) == 0) kind_ = MsgKind::Register;
                else if (strncmp(kind_value, "Directory", 9) == 0) kind_ = MsgKind::Directory;
            } else if (strncmp("-p2_val::", ser_token, strlen("-p2_val::")) == 0) {
                // When p2 is found
                // Get the sender and set it.
                int key_len = strlen("-p2_val::");
                char sender_value[MAX_SIZET_BYTES];
                memset(sender_value, 0, MAX_SIZET_BYTES);
                strncpy(
                        sender_value,
                        ser_token + key_len,
                        strlen(ser_token) - key_len
                );
                sscanf(sender_value, "%zu", &sender_);
            } else if (strncmp("-p3_val::", ser_token, strlen("-p3_val::")) == 0) {
                // When p3 is found,
                // Get the receiver/target and send it,
                int key_len = strlen("-p3_val::");
                char target_value[MAX_SIZET_BYTES];
                memset(target_value, 0, MAX_SIZET_BYTES);
                strncpy(
                        target_value,
                        ser_token + key_len,
                        strlen(ser_token) - key_len
                );
                sscanf(target_value, "%zu", &target_);
            } else if (strncmp("-p4_val::", ser_token, strlen("-p4_val::")) == 0) {
                // When p4 is found
                // Get the id and set it.
                int key_len = strlen("-p4_val::");
                char id_value[MAX_SIZET_BYTES];
                memset(id_value, 0, MAX_SIZET_BYTES);
                strncpy(
                        id_value,
                        ser_token + key_len,
                        strlen(ser_token) - key_len
                );
                sscanf(id_value, "%zu", &id_);
            }
            ser_token = strtok(NULL, " ");
        }
    }

};