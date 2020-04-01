#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include "message.h"

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
	sockaddr_in client_; // sockaddr for the node
	size_t port_; // port of the node

	/** Register constructor */
	Register(char* ip, size_t port, size_t sender, size_t target,
	size_t id) {
		client_.sin_family = AF_INET;
		client_.sin_addr.s_addr = inet_addr(ip);
		client_.sin_port = htons(port);

		port_ = port;

		kind_ = MsgKind::Register;
		sender_ = sender;
		target_ = target;
		id_ = id;
	}

	/** Constructs Regiser from serialized Register string */
	Register(char *ser) {
			// set kind_ member to Register
			kind_ = MsgKind::Register;

			// create copy of serialized string
			char ser_copy[MAX_BUFFER_SIZE];
			memset(ser_copy, 0, MAX_BUFFER_SIZE);
			strcpy(ser_copy, ser);

			// loop through tokens split by spaces
			char *ser_token = strtok(ser, " ");
			while (ser_token != NULL) {
					// if p2 value is found add it as sender_ member
					if (strncmp("-p2_val::", ser_token, strlen("-p2_val::")) == 0) {
							int key_len = strlen("-p2_val::");
							char sender_value[MAX_SIZET_BYTES];
							memset(sender_value, 0, MAX_SIZET_BYTES);
							strncpy(
											sender_value,
											ser_token + key_len,
											strlen(ser_token) - key_len
							);
							sscanf(sender_value, "%zu", &sender_);
					}
							// if p3 value is found add it as target_ member
					else if (strncmp("-p3_val::", ser_token, strlen("-p3_val::")) == 0) {
							int key_len = strlen("-p3_val::");
							char target_value[MAX_SIZET_BYTES];
							memset(target_value, 0, MAX_SIZET_BYTES);
							strncpy(
											target_value,
											ser_token + key_len,
											strlen(ser_token) - key_len
							);
							sscanf(target_value, "%zu", &target_);
					}
							// if p4 value is found add it as id_ member
					else if (strncmp("-p4_val::", ser_token, strlen("-p4_val::")) == 0) {
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
							// if p5 value is found add it as msg_ member
					else if (strncmp("-p5_val::", ser_token, strlen("-p5_val::")) == 0) {
							char *p5_value = strstr(ser_copy, "-p5_val::");
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


};
