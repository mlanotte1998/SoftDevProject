#pragma once

// out of bounds error messages
char* int_col_index_out_of_bounds = (char*)"Int Col Index out of Bounds";
char* bool_col_index_out_of_bounds = (char*)"Bool Col Index out of Bounds";
char* float_col_index_out_of_bounds = (char*)"Float Col Index out of Bounds";
char* double_col_index_out_of_bounds = (char*)"Double Col Index out of Bounds";
char* string_col_index_out_of_bounds = (char*)"String Col Index out of Bounds";

// empty column error messages
char* int_col_emtpy = (char*)"Int Col is empty";
char* bool_col_emtpy = (char*)"Bool Col is empty";
char* float_col_emtpy = (char*)"Float Col is empty";
char* double_col_emtpy = (char*)"Double Col is empty";
char* string_col_emtpy = (char*)"String Col is empty";

// incorrect type column error messages
char* non_int_col =  (char*)"Non-Integer Column";
char* non_float_col =  (char*)"Non-Float Column";
char* non_double_col =  (char*)"Non-Double Column";
char* non_string_col =  (char*)"Non-String Column";
char* non_bool_col =  (char*)"Non-Boolean Column";

// invalid column size error message
char* invalid_col_size = (char*)"Invalid Column Size";
char* col_index_out_of_bounds = (char*)"Col Index out of Bounds";
char* row_index_out_of_bounds = (char*)"Row Index out of Bounds";
char* given_malformed_schema = (char*)"Given Malformed Schema";

// maximum byte bounds of data types
static const int MAX_MSGKIND_BYTES = 11;
static const int MAX_SIZET_BYTES = 20;
static const int MAX_BOOL_BYTES = 1;
static const int MAX_INT_BYTES = 11;
static const int MAX_FLOAT_BYTES = 20;
static const int MAX_DOUBLE_BYTES = 20;
static const int MAX_STRING_BYTES = 2048;

// maximum byte bounds for serialization
static const int MAX_BUFFER_SIZE_STANDARD = 10000;
static const int MAX_BUFFER_SIZE_LARGE = 2000000;
static const int MAX_ARRAY_SIZE_BYTES_STANDARD = MAX_BUFFER_SIZE_STANDARD - 1000;
static const int MAX_ARRAY_SIZE_BYTES_LARGE = MAX_BUFFER_SIZE_LARGE - 1000;

// serialization string constants
static const char* ACK_STRING = "-o_type::Ack";
static const int ACK_STRING_SIZE = 12;
static const char* DIRECTORY_STRING = "-o_type::Directory";
static const int DIRECTORY_STRING_SIZE = 18;
static const char* REGISTER_STRING = "-o_type::Register";
static const int REGISTER_STRING_SIZE = 17;
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
