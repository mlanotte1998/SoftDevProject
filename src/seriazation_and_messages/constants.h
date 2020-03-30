#ifndef _CONSTANTS_H
#define _CONSTANTS_H

static const int MAX_MSGKIND_BYTES = 11;
static const int MAX_SIZET_BYTES = 20;
static const int MAX_BOOL_BYTES = 1;
static const int MAX_INT_BYTES = 11;
static const int MAX_FLOAT_BYTES = 20;
static const int MAX_DOUBLE_BYTES = 20;
static const int MAX_STRING_BYTES = 2048;
static const int MAX_BUFFER_SIZE = 10000;
static const int MAX_ARRAY_SIZE_BYTES = MAX_BUFFER_SIZE - 1000;

static const char* ACK_STRING = "-o_type::Ack";
static const int ACK_STRING_SIZE = 12;
static const char* STATUS_STRING = "-o_type::Status";
static const int STATUS_STRING_SIZE = 15;
static const char* MESSAGE_STRING = "-o_type::Message";
static const int MESSAGE_STRING_SIZE = 15;
static const char* BOOL_COL_STRING = "-o_type::BoolColumn";
static const int BOOL_COL_STRING_SIZE = 19;
static const char* INT_COL_STRING = "-o_type::IntColumn";
static const int INT_COL_STRING_SIZE = 18;
static const char* FLOAT_COL_STRING = "-o_type::FloatColumn";
static const int FLOAT_COL_STRING_SIZE = 20;
static const char* DOUBLE_COL_STRING = "-o_type::DoubleColumn";
static const int DOUBLE_COL_STRING_SIZE = 21;
static const char* STRING_COL_STRING = "-o_type::StringColumn";
static const int STRING_COL_STRING_SIZE = 21;

#endif