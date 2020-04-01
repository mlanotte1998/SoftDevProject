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
		size_t* ports_;
		size_t addresses_count_;
		String** addresses_;

		/** Directory constructor */
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

		/** Constructs Directory from serialized Directory string */
		Directory(char* ser) {
			kind_ = MsgKind::Directory;

			// create copy of serialized string to tokenize
			char ser_copy[MAX_BUFFER_SIZE];
			memset(ser_copy, 0, MAX_BUFFER_SIZE);
			strcpy(ser_copy, ser);

			// create copy of serialized string to grab the values from
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
					// if p5 value is found add it as client_ member
					if (strncmp("-p5_val::", ser_token, strlen("-p5_val::")) == 0) {
						int p5_key_len = strlen("-p5_val::");
						char client_value[MAX_SIZET_BYTES];
						memset(client_value, 0, MAX_SIZET_BYTES);
						strncpy(
							client_value,
							ser_token + p5_key_len,
							strlen(ser_token) - p5_key_len
						);
						sscanf(client_value, "%zu", &client_);
					}

					// if p6 value is found add it as ports_count_ member
					if (strncmp("-p6_val::", ser_token, strlen("-p6_val::")) == 0) {
						int p6_key_len = strlen("-p6_val::");
						char ports_count_value[MAX_SIZET_BYTES];
						memset(ports_count_value, 0, MAX_SIZET_BYTES);
						strncpy(
							ports_count_value,
							ser_token + p6_key_len,
							strlen(ser_token) - p6_key_len
						);
						sscanf(ports_count_value, "%zu", &ports_count_);
					}

					// if p7 value is found add it as ports_ member
					if (strncmp("-p7_val::", ser_token, strlen("-p7_val::")) == 0) {
					
					}

					// if p8 value is found add it as addresses_count_ member
					if (strncmp("-p8_val::", ser_token, strlen("-p8_val::")) == 0) {
						int p8_key_len = strlen("-p8_val::");
						char addresses_count_value[MAX_SIZET_BYTES];
						memset(addresses_count_value, 0, MAX_SIZET_BYTES);
						strncpy(
							addresses_count_value,
							ser_token + p8_key_len,
							strlen(ser_token) - p8_key_len
						);
						sscanf(addresses_count_value, "%zu", &addresses_count_);
					}

					// if p9 value is found add it as addresses_ member
					if (strncmp("-p9_val::", ser_token, strlen("-p9_val::")) == 0) {

					}

					// continue to next token
					ser_token = strtok(NULL, " ");
			}
		}

		/** Directory destructor */
		~Directory() {

		}

};
