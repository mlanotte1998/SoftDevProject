#pragma once

#include <stdio.h>
#include <string.h>
#include "../dataframe/dataframe.h"
#include "message.h"
#include "message_kind.h"
#include "ack.h"
#include "directory.h"
#include "status.h"
#include "register.h"
#include "../dataframe/constants.h"

/*****************************************************************************
 * Serializer::
 * A serialization support class.
 * authors: welch.da@husky.neu.edu, lanotte.m@husky.neu.edu
 */
class Serializer : public Object {
	public:
    char* buffer_;

    // constructor for serialization
    Serializer() {
      buffer_ = new char[MAX_BUFFER_SIZE];
			strcpy(buffer_, "");
    }

    // constructor for deserialization
    Serializer(unsigned char* bytes) {
      buffer_ = reinterpret_cast<char*>(bytes);
    }

    // destructor for serialization
    ~Serializer() {
      delete[] buffer_;
    }

    // get the serialized string
    unsigned char* getBytes() {
      return reinterpret_cast<unsigned char*>(buffer_);
    }

    // method for serializing an Object
    char* serialize(Object* obj) {
      if (obj == nullptr) return buffer_;
      Status* status = dynamic_cast<Status*>(obj);
      if (status != nullptr) {
        serialize_message(status, "Status", 5);
        serialize_status(status);
        return buffer_;
      }
      Directory* dir = dynamic_cast<Directory*>(obj);
      if (dir != nullptr) {
        serialize_message(dir, "Directory", 9);
        serialize_directory(dir);
        return buffer_;
      }
			Register* reg = dynamic_cast<Register*>(obj);
      if (reg != nullptr) {
        serialize_message(reg, "Register", 6);
        serialize_register(reg);
        return buffer_;
      }
      Ack* ack = dynamic_cast<Ack*>(obj);
      if (ack != nullptr) {
        serialize_message(ack, "Ack", 5);
        return buffer_;
      }
      Message* message = dynamic_cast<Message*>(obj);
      if (message != nullptr) {
        serialize_message(message, "Message", 4);
        return buffer_;
      }
      BoolColumn* bool_col = dynamic_cast<BoolColumn*>(obj);
      if (bool_col != nullptr) {
        serialize_bool_col(bool_col);
        return buffer_;
      }
      IntColumn* int_col = dynamic_cast<IntColumn*>(obj);
      if (int_col != nullptr) {
        serialize_int_col(int_col);
        return buffer_;
      }
      FloatColumn* float_col = dynamic_cast<FloatColumn*>(obj);
      if (float_col != nullptr) {
        serialize_float_col(float_col);
        return buffer_;
      }
      DoubleColumn* double_col = dynamic_cast<DoubleColumn*>(obj);
      if (double_col != nullptr) {
        serialize_double_col(double_col);
        return buffer_;
      }
      StringColumn* string_col = dynamic_cast<StringColumn*>(obj);
      if (string_col != nullptr) {
        serialize_string_col(string_col);
        return buffer_;
      }
      return buffer_;
    }

    // deserialize the buffer
    Object* deserialize() {
      if (strncmp(buffer_, STATUS_STRING, STATUS_STRING_SIZE) == 0) {
        return new Status(buffer_);
      } else if (strncmp(buffer_, REGISTER_STRING, REGISTER_STRING_SIZE) == 0) {
        return new Register(buffer_);
      } else if (strncmp(buffer_, DIRECTORY_STRING, DIRECTORY_STRING_SIZE) == 0) {
        return new Directory(buffer_);
      } else if (strncmp(buffer_, ACK_STRING, ACK_STRING_SIZE) == 0) {
        return new Ack(buffer_);
      } else if (strncmp(buffer_, MESSAGE_STRING, MESSAGE_STRING_SIZE) == 0) {
        return new Message(buffer_);
      } else if (strncmp(buffer_, BOOL_COL_STRING, BOOL_COL_STRING_SIZE) == 0) {
        return new BoolColumn(buffer_);
      } else if (strncmp(buffer_, INT_COL_STRING, INT_COL_STRING_SIZE) == 0) {
        return new IntColumn(buffer_);
      } else if (strncmp(buffer_, FLOAT_COL_STRING, FLOAT_COL_STRING_SIZE) == 0) {
        return new FloatColumn(buffer_);
      } else if (strncmp(buffer_, DOUBLE_COL_STRING, DOUBLE_COL_STRING_SIZE) == 0) {
        return new DoubleColumn(buffer_);
      } else if (strncmp(buffer_, STRING_COL_STRING, STRING_COL_STRING_SIZE) == 0) {
        return new StringColumn(buffer_);
      }
      return nullptr;
    }

    // append constant string to buffer
    void append(const char* val) {
      strcat(buffer_, val);
      strcat(buffer_, " ");
    }

    // append constant key value pair to buffer
    void append(const char* key, const char* val) {
      strcat(buffer_, key);
      strcat(buffer_, "::");
      strcat(buffer_, val);
      strcat(buffer_, " ");
    }

    // append key value pair to buffer
    void append(const char* key, char* val) {
      strcat(buffer_, key);
      strcat(buffer_, "::");
      strcat(buffer_, val);
      strcat(buffer_, " ");
    }

    // append key bool value pair to buffer
    void append(const char* key, bool val) {
      strcat(buffer_, key);
      strcat(buffer_, "::");
      if (val) strcat(buffer_, "1");
      else strcat(buffer_, "0");
      strcat(buffer_, " ");
    }

    // append key size_t value pair to buffer
    void append(const char* key, size_t val) {
      strcat(buffer_, key);
      strcat(buffer_, "::");
      char sizet_string[MAX_SIZET_BYTES];
      sprintf(sizet_string, "%zu", val);
      strcat(buffer_, sizet_string);
      strcat(buffer_, " ");
    }

    // append key int value pair to buffer
    void append(const char* key, int val) {
      strcat(buffer_, key);
      strcat(buffer_, "::");
      char int_string[MAX_INT_BYTES];
      sprintf(int_string, "%i", val);
      strcat(buffer_, int_string);
      strcat(buffer_, " ");
    }

    // method for serializing common parameters of columns
    // helper method called by each column serialization method
    void serialize_col_class(const char* type) {
      const char* title;
      const char* arr_type;
      if (strncmp(type, "B", 1) == 0) {
        title = "Bool";
        arr_type = "bool**";
      } else if (strncmp(type, "I", 1) == 0) {
        title = "Int";
        arr_type = "int**";
      } else if (strncmp(type, "F", 1) == 0) {
        title = "Float";
        arr_type = "float**";
      } else if (strncmp(type, "D", 1) == 0) {
        title = "Double";
        arr_type = "double**";
      } else if (strncmp(type, "S", 1) == 0) {
        title = "String";
        arr_type = "String***";
      }

      // append classtype
      char class_type[14];
      memset(class_type, 0, 14);
      strcat(class_type, title);
      strcat(class_type, "Column");
      append("-o_type", class_type);

      // append count of class parameters
      append("-p_count", 3);

      // append column array key and description
      append("-p1_type", arr_type);
      append("-p1_name","binary_column_array_");

      // append array_length_ parameter
      append("-p2_type", "size_t");
      append("-p2_name","array_length_");

      // append size_ parameter
      append("-p3_type", "size_t");
      append("-p3_name","size_");
    }

    // method for converting a MsgKind to a string to be
    // appended to the serialized string
    const char* message_type_to_str(MsgKind msg_kind) {
      if (msg_kind == ACK) return "Ack";
      else if (msg_kind == NACK) return "Nack";
      else if (msg_kind == PUT) return "Put";
      else if (msg_kind == REPLY) return "Reply";
      else if (msg_kind == GET) return "Get";
      else if (msg_kind == WAITANDGET) return "WaitAndGet";
      else if (msg_kind == STATUS) return "Status";
      else if (msg_kind == KILL) return "Kill";
      else if (msg_kind == REGISTER) return "Register";
      else if (msg_kind == DIRECTORY) return "Directory";
      return "N/A";
    }

    // method for serializing common parameters of messages
    // helper method called by each message serialization method
    void serialize_message(Message* msg, const char* type, int p_count) {
      append("-o_type", type);

      // append count of class parameters
      append("-p_count", p_count);

      // append kind_ parameter
      append("-p1_type", "MsgKind");
      append("-p1_name","kind_");

      // append sender_ parameter
      append("-p2_type", "size_t");
      append("-p2_name","sender_");

      // append target_ parameter
      append("-p3_type", "size_t");
      append("-p3_name","target_");

      // append id_ parameter
      append("-p4_type", "size_t");
      append("-p4_name","id_");

      if (strncmp(type, "Status", 6) == 0) {
        // append msg_ parameter
        append("-p5_type", "String*");
        append("-p5_name","msg_");
      }

			if (strncmp(type, "Register", 6) == 0) {
        // append client_ip_ parameter
        append("-p5_type", "String*");
        append("-p5_name","client_ip_");

				// append port_ parameter
        append("-p6_type", "size_t");
        append("-p6_name","port_");
      }

      if (strncmp(type, "Directory", 9) == 0) {
        // append client_ parameter
        append("-p5_type", "size_t");
        append("-p5_name","client_");
        // append ports_count_ parameter
        append("-p6_type", "size_t");
        append("-p6_name","ports_count_");
        // append ports_ parameter
        append("-p7_type", "size_t*");
        append("-p7_name","ports_");
        // append addresses_count_ parameter
        append("-p8_type", "size_t");
        append("-p8_name","addresses_count_");
        // append addresses_ parameter
        append("-p9_type", "String**");
        append("-p9_name","addresses_");
      }

      // append object values
      append("-p1_val", message_type_to_str(msg->kind_));
      append("-p2_val", msg->sender_);
      append("-p3_val", msg->target_);
      append("-p4_val", msg->id_);
    }

    // method for adding status-specific parameters to serialized message
    void serialize_status(Status* status) {
      // append the status message to the end of the serialized
      // message object as the p5_val
      int msg_size = strlen(status->msg_->c_str()) + 4;
      char msg_str[msg_size];
      memset(msg_str, 0, msg_size);
      strcat(msg_str, "[\"");
      strcat(msg_str, status->msg_->c_str());
      strcat(msg_str, "\"]");
      append("-p5_val", msg_str);
    }


	   // method for adding register-specific parameters to serialized message
		 void serialize_register(Register* regis) {
		    // append the ip string to the end of the serialized
		    // message object as the p5_val
		    int ip_size = strlen(regis->client_ip_->c_str()) + 4;
		      char ip_str[ip_size];
		      memset(ip_str, 0, ip_size);
		      strcat(ip_str, "[\"");
		      strcat(ip_str, regis->client_ip_->c_str());
		      strcat(ip_str, "\"]");
		      append("-p5_val", ip_str);

					// append the port number to the end of the serialzed message
					// object as the p6 val.
					append("-p6_val", regis->port_);
		   }



    // method for adding directory-specific parameters to serialized message
    void serialize_directory(Directory* dir) {
      // append the client_, ports_count_, ports_, addresses_count_, and addresses_
      // to the end of the serialized message object as the
      // p5_val, p6_val, p7_val, p8_val, and p9_val
      append("-p5_val", dir->client_);
      append("-p6_val", dir->ports_count_);

      // append ports values
      char sizet_arr_str[MAX_ARRAY_SIZE_BYTES];
      memset(sizet_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
      strcat(sizet_arr_str, "arr(");

      // create serialized array of size_t
      int sizet_builder_size = MAX_SIZET_BYTES;
      char sizet_builder[sizet_builder_size];
      for (size_t i = 0; i < dir->ports_count_; i++) {
          sprintf(sizet_builder, "%zu", dir->ports_[i]);
          if (dir->ports_count_ - 1 != i) strcat(sizet_builder, ",");
          strcat(sizet_arr_str, sizet_builder);
          memset(sizet_builder, 0, sizet_builder_size);
      }
      strcat(sizet_arr_str, ")");

      append("-p7_val", sizet_arr_str);

      append("-p8_val", dir->addresses_count_);

      // append addresses values
      char string_arr_str[MAX_ARRAY_SIZE_BYTES];
      memset(string_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
      strcat(string_arr_str, "arr(");

      // create serialized array of strings
      int str_builder_size = MAX_STRING_BYTES;
      char str_builder[str_builder_size];
      for (size_t i = 0; i < dir->addresses_count_; i++) {
          sprintf(str_builder, "[\"%s\"]", dir->addresses_[i]->c_str());
          if (dir->addresses_count_- 1 != i) strcat(str_builder, ",");
          strcat(string_arr_str, str_builder);
          memset(str_builder, 0, str_builder_size);
      }
      strcat(string_arr_str, ")");

      append("-p9_val", string_arr_str);
    }

    // method for serializing a BoolColumn
    void serialize_bool_col(BoolColumn* bc) {
      // append classname and params
      serialize_col_class("B");

      // append column array values
      char bool_arr_str[MAX_ARRAY_SIZE_BYTES];
      memset(bool_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
      strcat(bool_arr_str, "arr(");

      // create serialized array of bools
      int builder_size = MAX_BOOL_BYTES;
      char builder[builder_size];
      for (size_t i = 0; i < bc->size_; i++) {
          sprintf(builder, "%d", bc->get(i));
          if (bc->size_ - 1 != i) strcat(builder, ",");
          strcat(bool_arr_str, builder);
          memset(builder, 0, builder_size);
      }
      strcat(bool_arr_str, ")");

      // append object values
      append("-p1_val", bool_arr_str);
      append("-p2_val", bc->array_length_);
      append("-p3_val", bc->size_);
  }

  // method for serializing an IntColumn
  void serialize_int_col(IntColumn* ic) {
    // append classname and params
    serialize_col_class("I");

    // append column array values
    char int_arr_str[MAX_ARRAY_SIZE_BYTES];
    memset(int_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
    strcat(int_arr_str, "arr(");

    // create serialized array of integers
    int builder_size = MAX_INT_BYTES;
    char builder[builder_size];
    for (size_t i = 0; i < ic->size_; i++) {
        sprintf(builder, "%d", ic->get(i));
        if (ic->size_ - 1 != i) strcat(builder, ",");
        strcat(int_arr_str, builder);
        memset(builder, 0, builder_size);
    }
    strcat(int_arr_str, ")");

    // append object values
    append("-p1_val", int_arr_str);
    append("-p2_val", ic->array_length_);
    append("-p3_val", ic->size_);
  }

  // method for serializing a FloatColumn
  void serialize_float_col(FloatColumn* fc) {
    // append classname and params
    serialize_col_class("F");

    // append column array values
    char float_arr_str[MAX_ARRAY_SIZE_BYTES];
    memset(float_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
    strcat(float_arr_str, "arr(");

    // create serialized array of floats
    int builder_size = MAX_FLOAT_BYTES;
    char builder[builder_size];
    for (size_t i = 0; i < fc->size_; i++) {
        sprintf(builder, "%f", fc->get(i));
        if (fc->size_ - 1 != i) strcat(builder, ",");
        strcat(float_arr_str, builder);
        memset(builder, 0, builder_size);
    }
    strcat(float_arr_str, ")");

    // append object values
    append("-p1_val", float_arr_str);
    append("-p2_val", fc->array_length_);
    append("-p3_val", fc->size_);
  }

  // method for serializing a DoubleColumn
  void serialize_double_col(DoubleColumn* dc) {
    // append classname and params
    serialize_col_class("D");

    // append column array values
    char double_arr_str[MAX_ARRAY_SIZE_BYTES];
    memset(double_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
    strcat(double_arr_str, "arr(");

    // create serialized array of doubles
    int builder_size = MAX_DOUBLE_BYTES;
    char builder[builder_size];
    for (size_t i = 0; i < dc->size_; i++) {
        sprintf(builder, "%lf", dc->get(i));
        if (dc->size_ - 1 != i) strcat(builder, ",");
        strcat(double_arr_str, builder);
        memset(builder, 0, builder_size);
    }
    strcat(double_arr_str, ")");

    // append object values
    append("-p1_val", double_arr_str);
    append("-p2_val", dc->array_length_);
    append("-p3_val", dc->size_);
  }

  // method for serializing a StringColumn
  void serialize_string_col(StringColumn* sc) {
    // append classname and params
    serialize_col_class("S");

    // append column array values
    char string_arr_str[MAX_ARRAY_SIZE_BYTES];
    memset(string_arr_str, 0, MAX_ARRAY_SIZE_BYTES);
    strcat(string_arr_str, "arr(");

    // create serialized array of strings
    int builder_size = MAX_STRING_BYTES;
    char builder[builder_size];
    for (size_t i = 0; i < sc->size_; i++) {
        sprintf(builder, "[\"%s\"]", sc->get(i)->c_str());
        if (sc->size_ - 1 != i) strcat(builder, ",");
        strcat(string_arr_str, builder);
        memset(builder, 0, builder_size);
    }
    strcat(string_arr_str, ")");

    // append object values
    append("-p1_val", string_arr_str);
    append("-p2_val", sc->array_length_);
    append("-p3_val", sc->size_);
  }

};
