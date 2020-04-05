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
		size_t client_; // client that this directory is known by.
		size_t ports_count_; // count of ports.
		size_t* ports_; // list of ports.
		size_t addresses_count_; // count of addresses.
		String** addresses_; // list of addresses.
		size_t nodes_count_; // count of nodes.
		size_t* nodes_; // list of node numbers.

		/** Directory constructor */
		Directory(size_t sender, size_t target, size_t id, size_t client,
			 size_t ports_count, size_t* ports, size_t addresses_count,
			 String** addresses, size_t nodes_count , size_t* nodes) {
			kind_ = MsgKind::Directory;
			sender_ = sender;
			target_ = target;
			id_ = id;
			client_ = client;
			ports_count_ = ports_count;
			ports_ = new size_t[ports_count_];
			for (size_t i = 0; i < ports_count; i++) {
				ports_[i] = ports[i];
			}
			addresses_count_ = addresses_count;
			addresses_ = new String*[addresses_count_];
			for (size_t j = 0; j < addresses_count; j++) {
				addresses_[j] = new String(*addresses[j]);
			}
			nodes_count_ = nodes_count;
			nodes_ = new size_t[nodes_count_];
			for (size_t i = 0; i < ports_count; i++) {
				nodes_[i] = nodes[i];
			}
		}

		/** Constructs Directory from serialized Directory string */
		Directory(char* ser) {
			kind_ = MsgKind::Directory;
			// ports_count_ and addresses_count_ initially 0 as they haven't been read yet
			ports_count_ = 0;
			addresses_count_ = 0;

			// create copy of serialized string to tokenize
			char ser_copy[MAX_BUFFER_SIZE_STANDARD];
			memset(ser_copy, 0, MAX_BUFFER_SIZE_STANDARD);
			strcpy(ser_copy, ser);

			// create copy of serialized string to grab the ports from
			char ser_copy2[MAX_BUFFER_SIZE_STANDARD];
			memset(ser_copy2, 0, MAX_BUFFER_SIZE_STANDARD);
			strcpy(ser_copy2, ser);

			// create copy of serialized string to grab the addresses from
			char ser_copy3[MAX_BUFFER_SIZE_STANDARD];
			memset(ser_copy3, 0, MAX_BUFFER_SIZE_STANDARD);
			strcpy(ser_copy3, ser);

			// create copy of serialized string to grab the nodes from
			char ser_copy4[MAX_BUFFER_SIZE_STANDARD];
			memset(ser_copy4, 0, MAX_BUFFER_SIZE_STANDARD);
			strcpy(ser_copy4, ser);

			// Create a normal message to extract shared fields.
			Message* temp = new Message(ser);
			sender_ = temp->sender_;
			target_ = temp->target_;
			id_ = temp->id_;

			delete temp;

			bool empty_directory = false;

			// loop through tokens split by spaces
			char *ser_token = strtok(ser_copy, " ");

			while (ser_token != NULL) {

				if (!empty_directory) {
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

						if (ports_count_ == 0) {
							empty_directory = true;
						}
					}

					// if p7 value is found add it as ports_ member
					if (strncmp("-p7_val::", ser_token, strlen("-p7_val::")) == 0) {
						char* p7_value = strstr(ser_copy2, "-p7_val::");
						// ports_count_ will be defined by the time this is reached
						ports_ = new size_t[ports_count_];

						char* p7_end = strstr(p7_value, ")");
						int p7_array_length = p7_end - p7_value;

						// first token is entire array
						char* p7_token = p7_value;
						char* p7_token_end;
						int p7_token_length;

						// buffer used to add char* to a string
						int p7_buffer_size = p7_array_length;
						char p7_buffer[p7_buffer_size];
        		memset(p7_buffer, 0, p7_buffer_size);

						size_t p7_index = 0;
						bool end_of_p7_array = false;
						p7_token = strstr(p7_token, "(");
						while (p7_token != NULL && p7_index < ports_count_ && !end_of_p7_array && p7_token - p7_value <= p7_array_length) {
							// token starts after delimiter
							p7_token++;

							// get end of token
							p7_token_end = strstr(p7_token, ",");
							if (p7_token_end == NULL || p7_token_end - p7_value > p7_array_length) {
								p7_token_end = strstr(p7_token, ")");
								end_of_p7_array = true;
							}

							// calculate token length
							p7_token_length = p7_token_end - p7_token;

							// add token to p7_buffer
							memset(p7_buffer, 0, p7_buffer_size);
							for (int i = 0; i < p7_token_length; i++) {
								p7_buffer[i] = p7_token[i];
							}

							// read in value and add to ports_
							size_t p7_index_value = 0;
							sscanf(p7_buffer, "%zu", &p7_index_value);
							ports_[p7_index] = p7_index_value;

							// move to next token
							p7_token++;
							p7_index++;
							p7_token = strstr(p7_token, ",");
						}
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
						char* p9_value = strstr(ser_copy3, "-p9_val::");
						// addresses_count_ will be defined by the time this is reached
						addresses_ = new String*[addresses_count_];

						char* p9_end = strstr(p9_value, "\"])");
						int p9_array_length = p9_end - p9_value;

						// first token is entire array
						char* p9_token = p9_value;
						char* p9_token_end;
						int p9_token_length;

						// buffer used to add char* to a string
						int p9_buffer_size = p9_array_length;
						char p9_buffer[p9_buffer_size];
        		memset(p9_buffer, 0, p9_buffer_size);

						size_t p9_index = 0;
						while (p9_token != NULL) {
							// get beginning of token
							p9_token = strstr(p9_token, "[\"");

							// get end of token
							p9_token_end = strstr(p9_token, "\"],");
							if (p9_token_end == NULL) p9_token_end = strstr(p9_token, "\"])");

							// calculate token length
							p9_token_length = p9_token_end - p9_token - strlen("[\"");

							// add token to buffer
							memset(p9_buffer, 0, p9_buffer_size);
							for (int i = 0; i < p9_token_length; i++) {
								p9_buffer[i] = p9_token[i + 2];
							}

							// create string using token buffer and add to addresses
							String* add_str = new String(p9_buffer);
							addresses_[p9_index] = add_str;

							// move to next token
							p9_token++;
							p9_index++;
							p9_token = strstr(p9_token, "\"],[\"");
						}
					}

					// if p10 value is found add it as nodes_count_ member
					if (strncmp("-p10_val::", ser_token, strlen("-p10_val::")) == 0) {
						int p10_key_len = strlen("-p10_val::");
						char nodes_count_value[MAX_SIZET_BYTES];
						memset(nodes_count_value, 0, MAX_SIZET_BYTES);
						strncpy(
							nodes_count_value,
							ser_token + p10_key_len,
							strlen(ser_token) - p10_key_len
						);
						sscanf(nodes_count_value, "%zu", &nodes_count_);
					}

					// if p11 value is found add it as nodes_ member
					if (strncmp("-p11_val::", ser_token, strlen("-p11_val::")) == 0) {
						char* p11_value = strstr(ser_copy4, "-p11_val::");
						// ports_count_ will be defined by the time this is reached
						nodes_ = new size_t[nodes_count_];

						char* p11_end = strstr(p11_value, ")");
						int p11_array_length = p11_end - p11_value;

						// first token is entire array
						char* p11_token = p11_value;
						char* p11_token_end;
						int p11_token_length;

						// buffer used to add char* to a string
						int p11_buffer_size = p11_array_length;
						char p11_buffer[p11_buffer_size];
        		memset(p11_buffer, 0, p11_buffer_size);

						size_t p11_index = 0;
						bool end_of_p11_array = false;
						p11_token = strstr(p11_token, "(");
						while (p11_token != NULL && p11_index < nodes_count_ && !end_of_p11_array && p11_token - p11_value <= p11_array_length) {
							// token starts after delimiter
							p11_token++;

							// get end of token
							p11_token_end = strstr(p11_token, ",");
							if (p11_token_end == NULL || p11_token_end - p11_value > p11_array_length) {
								p11_token_end = strstr(p11_token, ")");
								end_of_p11_array = true;
							}

							// calculate token length
							p11_token_length = p11_token_end - p11_token;

							// add token to p11_buffer
							memset(p11_buffer, 0, p11_buffer_size);
							for (int i = 0; i < p11_token_length; i++) {
								p11_buffer[i] = p11_token[i];
							}

							// read in value and add to nodes_
							size_t p11_index_value = 0;
							sscanf(p11_buffer, "%zu", &p11_index_value);
							nodes_[p11_index] = p11_index_value;

							// move to next token
							p11_token++;
							p11_index++;
							p11_token = strstr(p11_token, ",");
						}
					}
				}

					// continue to next token
					ser_token = strtok(NULL, " ");
			}

			if (empty_directory) {
				addresses_count_ = 0;
				nodes_count_ = 0;
				ports_ = new size_t[1];
				nodes_ = new size_t[1];
				addresses_ = new String*[1];
			}
		}

		/** Directory destructor */
		~Directory() {
			delete [] ports_;
			delete [] nodes_;
			for(size_t i = 0; i < addresses_count_; i++) {
				delete addresses_[i];
			}
			delete [] addresses_;
		}

};
