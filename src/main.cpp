// Lang::CwC

#include <stdio.h>
#include <stdlib.h>

#include "./sorer/parser.h"
#include "./dataframe/dataframe.h"

/**
 * Enum representing different states of parsing command line arguments.
 */
enum class ParseState {
    DEFAULT,
    FLAG_F,
    FLAG_FROM,
    FLAG_LEN,
    FLAG_COL_TYPE,
    FLAG_COL_IDX_COL,
    FLAG_COL_IDX_OFF,
    FLAG_MISSING_IDX_COL,
    FLAG_MISSING_IDX_OFF
};

/**
 * Helper function to terminate with an error print if the given bool is not true.
 * @param test The bool
 */
void cli_assert(bool test) {
    if (!test) {
        printf("Unexpected command line arguments provided\n");
        exit(-1);
    }
}

/**
 * Asserts that the given ssize_t has not already been changed from -1, and then
 * parses the given c-style string as long to set it.
 * @param arg_loc The location of the ssize_t to work with
 * @param arg A string containing a long to parse
 */
void parse_size_t_arg(ssize_t* arg_loc, char* arg) {
    cli_assert(*arg_loc == -1);
    *arg_loc = atol(arg);
}

/**
 * Parses command line args given by argc and argv. Updates the given ssize_t pointers to
 * -1 for each arg that is not present on the command line (nullptr for file), or the value of that
 * command line argument.
 * @param argc, argv The command line arguments
 * @param file Pointer to result of parsing -f
 * @param start Pointer to result of parsing -from
 * @param len Pointer to result of parsing -len
 * @param col_type Pointer to result of parsing -print_col_type
 * @param col_idx_col, col_idx_off Pointer to result of parsing -print_col_idx
 * @param missing_idx_col, missing_idx_off Pointer to result of parsing -is_missing_idx
 */
void parse_args(int argc, char* argv[], char** file, ssize_t* start, ssize_t* len,
                ssize_t* col_type, ssize_t* col_idx_col, ssize_t* col_idx_off,
                ssize_t* missing_idx_col, ssize_t* missing_idx_off) {
    *file = nullptr;
    // -1 represents argument not provided
    *start = -1;
    *len = -1;
    *col_type = -1;
    *col_idx_col = -1;
    *col_idx_off = -1;
    *missing_idx_col = -1;
    *missing_idx_off = -1;

    ParseState state = ParseState::DEFAULT;

    for (int i = 1; i < argc; i++) {
        char* arg = argv[i];
        switch (state) {
            case ParseState::DEFAULT:
                if (strcmp(arg, "-f") == 0) {
                    state = ParseState::FLAG_F;
                } else if (strcmp(arg, "-from") == 0) {
                    state = ParseState::FLAG_FROM;
                } else if (strcmp(arg, "-len") == 0) {
                    state = ParseState::FLAG_LEN;
                } else if (strcmp(arg, "-print_col_type") == 0) {
                    state = ParseState::FLAG_COL_TYPE;
                } else if (strcmp(arg, "-print_col_idx") == 0) {
                    state = ParseState::FLAG_COL_IDX_COL;
                } else if (strcmp(arg, "-is_missing_idx") == 0) {
                    state = ParseState::FLAG_MISSING_IDX_COL;
                } else {
                    cli_assert(false);
                }
                break;
            case ParseState::FLAG_F:
                cli_assert(*file == nullptr);
                *file = arg;
                state = ParseState::DEFAULT;
                break;
            case ParseState::FLAG_FROM:
                parse_size_t_arg(start, arg);
                state = ParseState::DEFAULT;
                break;
            case ParseState::FLAG_LEN:
                parse_size_t_arg(len, arg);
                state = ParseState::DEFAULT;
                break;
            case ParseState::FLAG_COL_TYPE:
                parse_size_t_arg(col_type, arg);
                state = ParseState::DEFAULT;
                break;
            case ParseState::FLAG_COL_IDX_COL:
                parse_size_t_arg(col_idx_col, arg);
                state = ParseState::FLAG_COL_IDX_OFF;
                break;
            case ParseState::FLAG_COL_IDX_OFF:
                parse_size_t_arg(col_idx_off, arg);
                state = ParseState::DEFAULT;
                break;
            case ParseState::FLAG_MISSING_IDX_COL:
                parse_size_t_arg(missing_idx_col, arg);
                state = ParseState::FLAG_MISSING_IDX_OFF;
                break;
            case ParseState::FLAG_MISSING_IDX_OFF:
                parse_size_t_arg(missing_idx_off, arg);
                state = ParseState::DEFAULT;
                break;
            default:
                cli_assert(false);
        }
    }
    cli_assert(state == ParseState::DEFAULT);
}

/**
 * Gets the column at the given index from the given set. Terminates with an error print if which
 * is not valid.
 * @param set The ColumnSet to get from
 * @param which The column index to get
 */
BaseColumn* getColumnChecked(ColumnSet* set, size_t which) {
    if (which >= set->getLength()) {
        printf("No such column\n");
        exit(-1);
    }
    return set->getColumn(which);
}

/**
 * Terminates if the given column is not large enough to have the given entry index.
 * @param col The column
 * @param which The entry index
 */
void checkColumnEntry(BaseColumn* col, size_t which) {
    if (which >= col->getLength()) {
        printf("No such entry\n");
        exit(-1);
    }
}

/**
 * The main function.
 */
int main(int argc, char* argv[]) {
    // Parse arguments
    char* filename = nullptr;
    // -1 represents argument not provided
    ssize_t start = -1;
    ssize_t len = -1;
    ssize_t col_type = -1;
    ssize_t col_idx_col = -1;
    ssize_t col_idx_off = -1;
    ssize_t missing_idx_col = -1;
    ssize_t missing_idx_off = -1;

    parse_args(argc, argv, &filename, &start, &len, &col_type, &col_idx_col, &col_idx_off,
               &missing_idx_col, &missing_idx_off);

    // Check arguments
    if (filename == nullptr) {
        printf("No file provided\n");
        return -1;
    }

    // Open requested file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return -1;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Set argument defaults
    if (start == -1) {
        start = 0;
    }

    if (len == -1) {
        len = file_size - start;
    }

    {
        // Run parsing
        SorParser parser{file, (size_t)start, (size_t)start + len, file_size};
        parser.guessSchema();
        parser.parseFile();
        ColumnSet* set = parser.getColumnSet();
        char* schema_string = new char[set->getLength() + 1];
        int height;

        for(int i = 0; i < set->getLength(); i++) {
            BaseColumn* col = set->getColumn(i);
            if (i == 0) {
                height = col->getLength();
            }
            if (col->getType() == ColumnType::STRING) {
                schema_string[i] = 'S';
            } else if (col->getType() == ColumnType::INTEGER) {
                schema_string[i] = 'I';
            } else if (col->getType() == ColumnType ::FLOAT) {
                schema_string[i] = 'F';
            } else if (col->getType() == ColumnType ::BOOL) {
                schema_string[i] = 'B';
            }
        }

        schema_string[set->getLength()] = '\0';
        Schema s(schema_string);
        DataFrame df(s);


        for(int i = 0; i < height; i++) {
            Row r(df.get_schema());
            for (int j = 0; j < set->getLength(); j++) {
                if (df.get_schema().col_type(j) == 'S') {
                    SorerStringColumn* sc = dynamic_cast<SorerStringColumn*>(set->getColumn(j));
                    if (sc->isEntryPresent(i)) {
                        String* s = new String(sc->getEntry(i));
                        r.set(j, s);
                        delete s;
                    } else {
                        String* s = new String("");
                        r.set(j, s);
                        delete s;
                    }
                } else if (df.get_schema().col_type(j) == 'I') {
                    SorerIntegerColumn* ic = dynamic_cast<SorerIntegerColumn*>(set->getColumn(j));
                    if (ic->isEntryPresent(i)) {
                        r.set(j, ic->getEntry(i));
                    } else {
                        r.set(j, 0);
                    }
                } else if (df.get_schema().col_type(j) == 'B') {
                    SorerBoolColumn* bc = dynamic_cast<SorerBoolColumn*>(set->getColumn(j));
                    if (bc->isEntryPresent(i)) {
                        r.set(j, bc->getEntry(i));
                    } else {
                        r.set(j, 0);
                    }
                } else if (df.get_schema().col_type(j) == 'F') {
                    SorerFloatColumn* fc = dynamic_cast<SorerFloatColumn*>(set->getColumn(j));
                    if (fc->isEntryPresent(i)) {
                        r.set(j, fc->getEntry(i));
                    } else {
                        r.set(j, (float)0.0);
                    }
                }
            }
            df.add_row(r);
        }

        SumColumnRower* sum_rower = new SumColumnRower(1);
        df.map(*sum_rower);
        std::cout << sum_rower->sum_ << std::endl;
        delete sum_rower;
        delete [] schema_string;
    }

    fclose(file);
}
