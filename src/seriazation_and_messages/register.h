#pragma once

#include "message.h"
#include "../utility-classes/string.h"

/**
Class for a register of an node that would be sent as a message
to the registrar to then be relayed to other nodes.
*/
/**
 * Register::
 *
 * Class for a register of an node that would be sent as a message
 * to the registrar to then be relayed to other nodes.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Register : public Message {
public:
	String* client_ip_; // client_ip for the node, has ownership
	size_t port_; // port of the node

	/** Register constructor */
	Register(size_t sender, size_t target, size_t id,
	char* ip, size_t port) {
		client_ip_ = new String(ip);

		port_ = port;

		kind_ = MsgKind::Register;
		sender_ = sender;
		target_ = target;
		id_ = id;
	}


	/** Constructs Status from serialized Register string */
	Register(char *ser) {
			// set kind_ member to Register
			kind_ = MsgKind::Register;

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

							// if p5 value is found add it as client_ip_ member
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
							String *client_ip = new String(buffer);
							client_ip_ = client_ip;
					}

					// if p6 value is found add it as port_ member
					if (strncmp("-p6_val::", ser_token, strlen("-p6_val::")) == 0) {
						int p6_key_len = strlen("-p6_val::");
						char port_value[MAX_SIZET_BYTES];
						memset(port_value, 0, MAX_SIZET_BYTES);
						strncpy(
							port_value,
							ser_token + p6_key_len,
							strlen(ser_token) - p6_key_len
						);
						sscanf(port_value, "%zu", &port_);
					}



					// continue to next token
					ser_token = strtok(NULL, " ");
			}
	}

	/** Register destructor */
	~Register() {
		delete client_ip_;
	}
};
