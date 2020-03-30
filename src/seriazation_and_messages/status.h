#include "message.h"
#include "string.h"

class Status : public Message {
public:
    String *msg_; // owned

    Status(size_t sender, size_t target, size_t id, String *msg) {
        kind_ = MsgKind::Status;
        sender_ = sender;
        target_ = target;
        id_ = id;
        msg_ = msg;
    }

    Status(char *ser) {
        // set kind_ member to Status
        kind_ = MsgKind::Status;

        // create copy of serialized string to tokenize
        char ser_copy[MAX_BUFFER_SIZE];
        memset(ser_copy, 0, MAX_BUFFER_SIZE);
        strcpy(ser_copy, ser);

        // create copy of serialized string to grab the message from
        char ser_copy2[MAX_BUFFER_SIZE];
        memset(ser_copy2, 0, MAX_BUFFER_SIZE);
        strcpy(ser_copy2, ser);

        // Create a normal message to extract shared fields.
        Message* temp = new Message(ser);
        sender_ = temp->sender_;
        target_ = temp->target_;
        id_ = temp->id_;

        delete temp; 

        // loop through tokens split by spaces
        char *ser_token = strtok(ser_copy, " ");
        while (ser_token != NULL) {

                // if p5 value is found add it as msg_ member
            if (strncmp("-p5_val::", ser_token, strlen("-p5_val::")) == 0) {
                char *p5_value = strstr(ser_copy2, "-p5_val::");

                // buffer used to add char* to a string
                int buffer_size = strlen(p5_value) - strlen("-p5_val::");
                char buffer[buffer_size];
                memset(buffer, 0, buffer_size);
                char *ser_token = strstr(p5_value, "[\"");

                // get end of token
                char *ser_token_end = strstr(p5_value, "\"]");

                // calculate token length
                int ser_token_length = ser_token_end - ser_token - strlen("[\"");

                // add token to buffer
                memset(buffer, 0, buffer_size);
                for (int i = 0; i < ser_token_length; i++) {
                    buffer[i] = ser_token[i + 2];
                }

                // create string using token buffer set msg_ to created string
                String *msg_str = new String(buffer);
                msg_ = msg_str;
            }

            // continue to next token
            ser_token = strtok(NULL, " ");
        }
    }

    ~Status() {
      delete msg_;
    }

};
