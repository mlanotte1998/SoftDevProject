#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../src/dataframe/modified_dataframe.h"

// test pmap on large Dataframe
void test_thread_sum_rower_larger() {
  Schema s("II");
  ModifiedDataFrame df(s);
  Row  r(df.get_schema());
  for(size_t i = 0; i <  1000*1000*10; i++) {
    r.set(0,(int)1);
    r.set(1,(int)0);
    df.add_row(r);
  }

  SumColumnRower* sum_rower_0 = new SumColumnRower(0);
  df.pmap(*sum_rower_0);
  assert(sum_rower_0->sum_ == 1000*1000*10);
}

// main function
int main(int argc, char **argv) {
  // instantiate dataframe
  Schema file_schema("IIIIIIIISFS");
  ModifiedDataFrame file_df(file_schema);

  // error if given incorrect arguments
	if (argc < 4) {
		printf("Tool takes filename, rower type, map type, num rows as arguments\n");
		return 1;
	}

  // get filename from arguments
	char* filename = argv[1];

  // get rower type from arguments
  char* rower_type = argv[2];

  // get map type from arguments
  char* map_type = argv[3];

  int num_rows = -1;
  if (argc == 5) {
    num_rows = atoi(argv[4]);
  }
	
	// open file for reading
	const char* mode = "r";
	FILE* inp_file = fopen(filename, mode);

	// if unable to open file error
	if (inp_file == NULL) {
		printf("Failed to open file\n");
		return 1;
	}

  char line[256];
  // iterate through every line in file
  while (fgets(line, sizeof(line), inp_file) && num_rows != 0) {
    // delimit line by commas
    char* arg_val = strtok(line, ",\n");
    int col_idx = 0;

    // row to add to dataframe
    Row file_row(file_df.get_schema());

    // iterate through every arg in row
    while(arg_val != NULL) {
      // add value to column once comma is reached
      char file_col_type = file_df.get_schema().col_type(col_idx);
      if (file_col_type == 'S') {
        String* arg = new String(arg_val);
        file_row.set(col_idx, arg);
      } else if (file_col_type == 'I') {
        int arg = atoi(arg_val);
        file_row.set(col_idx, arg);
      } else if (file_col_type == 'F') {
        float arg = atof(arg_val);
        file_row.set(col_idx, arg);
      } else if (file_col_type == 'B') {
        bool arg = arg_val[0] == 1;
        file_row.set(col_idx, arg);
      }
      col_idx++;

      // continue to next value
      arg_val = strtok(NULL, ",\n");
    }

    // add row to dataframe
    file_df.add_row(file_row);

    num_rows--;
  }

  // close file
	fclose(inp_file);

  printf("Finished Building Dataframe\n");

  // time execution of map/pmap
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);

  if (strncmp(map_type, "map", 3) == 0) {
    if (strncmp(rower_type, "sumall", 6) == 0) {
      SumAllColumnRower* sum_all_rower = new SumAllColumnRower();
      file_df.map(*sum_all_rower);
      printf("Map Sum: %f\n", sum_all_rower->sum_);
    } else if (strncmp(rower_type, "sum", 3) == 0) {
      SumColumnRower* add_indices_rower = new SumColumnRower(0);
      file_df.map(*add_indices_rower);
      printf("Map Sum: %f\n", add_indices_rower->sum_);
    } else {
      return -1;
    }
  } 
  else {
    if (strncmp(rower_type, "sumall", 6) == 0) {
      SumAllColumnRower* sum_all_rower = new SumAllColumnRower();
      file_df.pmap(*sum_all_rower);
      printf("PMap Sum: %f\n", sum_all_rower->sum_);
    } else if (strncmp(rower_type, "sum", 3) == 0) {
      SumColumnRower* add_indices_rower = new SumColumnRower(0);
      file_df.pmap(*add_indices_rower);
      printf("PMap Sum: %f\n", add_indices_rower->sum_);
    } else {
      return -1;
    }
  }

  // print time of map / pmap execution
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  uint64_t delta_us = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
  printf("\n\tRuntime (microseconds): %llu\n\n", delta_us);

	return 0;
}
