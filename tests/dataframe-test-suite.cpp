//lang::CwC
#include <string.h>
#include <stdio.h>
#include "../src/dataframe/dataframe.h"

void FAIL() { exit(1); }

void OK(const char *m) {
    printf("%s Test Passed\n", m);
}

void t_true(bool p) {
    if (!p) {
        printf("Test Failed\n");
        FAIL();
    }
}

void t_false(bool p) {
    if (p) {
        printf("Test Failed\n");
        FAIL();
    }
}

bool float_equal(float f1, float f2) {
  float eps = 0.000001;
  if (f1 > f2) {
    return f1 - f2 < eps;
  } else {
    return f2 - f1 < eps;
  }
}

void test_int_column() {
    Column* c = new IntColumn(5,1,2,3,4,25);
    t_true(c->as_bool() == nullptr);
    t_true(c->as_float() == nullptr);
    t_true(c->as_string() == nullptr);
    IntColumn* i = c->as_int();
    t_true(i->get(0) == 1);
    t_true(i->get(1) == 2);
    t_true(i->get(2) == 3);
    t_true(i->get(3) == 4);
    t_true(i->get(4) == 25);
    t_true(i->size() == 5);
    i->set(3, 10);
    t_true(i->get(0) == 1);
    t_true(i->get(1) == 2);
    t_true(i->get(2) == 3);
    t_true(i->get(3) == 10);
    t_true(i->get(4) == 25);
    t_true(i->size() == 5);

    IntColumn* i2 = new IntColumn();
    i2->push_back(1);
    i2->push_back(2);
    t_true(i2->get(0) == 1);
    t_true(i2->get(1) == 2);
    t_true(i2->size() == 2);
    i2->push_back(5);
    t_true(i2->get(0) == 1);
    t_true(i2->get(1) == 2);
    t_true(i2->get(2) == 5);
    t_true(i2->size() == 3);
    i2->push_back(10);
    t_true(i2->get(0) == 1);
    t_true(i2->get(1) == 2);
    t_true(i2->get(2) == 5);
    t_true(i2->get(3) == 10);
    t_true(i2->size() == 4);
    i2->push_back(12);
    t_true(i2->get(0) == 1);
    t_true(i2->get(1) == 2);
    t_true(i2->get(2) == 5);
    t_true(i2->get(3) == 10);
    t_true(i2->get(4) == 12);
    t_true(i2->size() == 5);
    i2->set(0, -5);
    t_true(i2->get(0) == -5);
    t_true(i2->get(1) == 2);
    t_true(i2->get(2) == 5);
    t_true(i2->get(3) == 10);
    t_true(i2->get(4) == 12);
    t_true(i2->size() == 5);
    t_true(i2->as_int() != nullptr);
    delete i;
    delete i2;
    OK("Valid Int");
}

void test_bool_column() {
    Column* c = new BoolColumn(5,0,1,0,1,1);
    t_true(c->as_int() == nullptr);
    t_true(c->as_float() == nullptr);
    t_true(c->as_string() == nullptr);
    BoolColumn* i = c->as_bool();
    t_true(!i->get(0));
    t_true(i->get(1));
    t_true(!i->get(2));
    t_true(i->get(3));
    t_true(i->get(4));
    t_true(i->size() == 5);
    i->set(3, false);
    t_true(!i->get(0));
    t_true(i->get(1));
    t_true(!i->get(2));
    t_true(!i->get(3));
    t_true(i->get(4));
    t_true(i->size() == 5);

    BoolColumn* i2 = new BoolColumn();
    i2->push_back(false);
    i2->push_back(false);
    t_true(!i2->get(0));
    t_true(!i2->get(1));
    t_true(i2->size() == 2);
    i2->push_back(true);
    t_true(!i2->get(0));
    t_true(!i2->get(1));
    t_true(i2->get(2));
    t_true(i2->size() == 3);
    i2->push_back(false);
    t_true(!i2->get(0));
    t_true(!i2->get(1));
    t_true(i2->get(2));
    t_true(!i2->get(3));
    t_true(i2->size() == 4);
    i2->push_back(true);
    t_true(!i2->get(0));
    t_true(!i2->get(1));
    t_true(i2->get(2));
    t_true(!i2->get(3));
    t_true(i2->get(4));
    t_true(i2->size() == 5);
    i2->set(0, true);
    t_true(i2->get(0));
    t_true(!i2->get(1));
    t_true(i2->get(2));
    t_true(!i2->get(3));
    t_true(i2->get(4));
    t_true(i2->size() == 5);
    t_true(i2->as_bool() != nullptr);
    delete i;
    delete i2;
    OK("Valid Bool");
}

void test_float_column() {
    Column* c = new FloatColumn(5,1.1,2.2,3.3,4.4,25.25);
    t_true(c->as_bool() == nullptr);
    t_true(c->as_int() == nullptr);
    t_true(c->as_string() == nullptr);
    FloatColumn* i = c->as_float();
    t_true(float_equal(i->get(0), 1.1));
    t_true(float_equal(i->get(1), 2.2));
    t_true(float_equal(i->get(2), 3.3));
    t_true(float_equal(i->get(3), 4.4));
    t_true(float_equal(i->get(4), 25.25));
    t_true(i->size() == 5);
    i->set(3, 10.9);
    t_true(float_equal(i->get(0), 1.1));
    t_true(float_equal(i->get(1), 2.2));
    t_true(float_equal(i->get(2), 3.3));
    t_true(float_equal(i->get(3), 10.9));
    t_true(float_equal(i->get(4), 25.25));
    t_true(i->size() == 5);

    FloatColumn* i2 = new FloatColumn();
    i2->push_back(1.5);
    i2->push_back(2.4);
    t_true(float_equal(i2->get(0), 1.5));
    t_true(float_equal(i2->get(1), 2.4));
    t_true(i2->size() == 2);
    i2->push_back(5.6);
    t_true(float_equal(i2->get(0), 1.5));
    t_true(float_equal(i2->get(1), 2.4));
    t_true(float_equal(i2->get(2), 5.6));
    t_true(i2->size() == 3);
    i2->push_back(10.7);
    t_true(float_equal(i2->get(0), 1.5));
    t_true(float_equal(i2->get(1), 2.4));
    t_true(float_equal(i2->get(2), 5.6));
    t_true(float_equal(i2->get(3), 10.7));
    t_true(i2->size() == 4);
    i2->push_back(12.0);
    t_true(float_equal(i2->get(0), 1.5));
    t_true(float_equal(i2->get(1), 2.4));
    t_true(float_equal(i2->get(2), 5.6));
    t_true(float_equal(i2->get(3), 10.7));
    t_true(float_equal(i2->get(4), 12.0));
    t_true(i2->size() == 5);
    i2->set(0, -5.8);
    t_true(float_equal(i2->get(0), -5.8));
    t_true(float_equal(i2->get(1), 2.4));
    t_true(float_equal(i2->get(2), 5.6));
    t_true(float_equal(i2->get(3), 10.7));
    t_true(float_equal(i2->get(4), 12.0));
    t_true(i2->size() == 5);
    t_true(i2->as_float() != nullptr);
    delete i;
    delete i2;
    OK("Valid Float");
}

void test_string_column() {
    String* s1 = new String("s1");
    String* s2 = new String("s2");
    String* s3 = new String("s3");
    String* s4 = new String("s4");
    String* s5 = new String("s5");
    Column* c = new StringColumn(5,s1,s2,s3,s4,s5);
    t_true(c->as_int() == nullptr);
    t_true(c->as_float() == nullptr);
    t_true(c->as_bool() == nullptr);
    StringColumn* i = c->as_string();
    t_true(i->get(0)->equals(s1));
    t_true(i->get(1)->equals(s2));
    t_true(i->get(2)->equals(s3));
    t_true(i->get(3)->equals(s4));
    t_true(i->get(4)->equals(s5));
    t_true(i->size() == 5);

    StringColumn* i2 = new StringColumn();
    i2->push_back(s1);
    i2->push_back(s2);
    t_true(i2->get(0)->equals(s1));
    t_true(i2->get(1)->equals(s2));
    t_true(i2->size() == 2);
    i2->push_back(s5);
    t_true(i2->get(0)->equals(s1));
    t_true(i2->get(1)->equals(s2));
    t_true(i2->get(2)->equals(s5));
    t_true(i2->size() == 3);
    i2->push_back(s3);
    t_true(i2->get(0)->equals(s1));
    t_true(i2->get(1)->equals(s2));
    t_true(i2->get(2)->equals(s5));
    t_true(i2->get(3)->equals(s3));
    t_true(i2->size() == 4);
    i2->push_back(s4);
    t_true(i2->get(0)->equals(s1));
    t_true(i2->get(1)->equals(s2));
    t_true(i2->get(2)->equals(s5));
    t_true(i2->get(3)->equals(s3));
    t_true(i2->get(4)->equals(s4));
    t_true(i2->size() == 5);
    i2->set(0, s5);
    t_true(i2->get(0)->equals(s5));
    t_true(i2->get(1)->equals(s2));
    t_true(i2->get(2)->equals(s5));
    t_true(i2->get(3)->equals(s3));
    t_true(i2->get(4)->equals(s4));
    t_true(i2->size() == 5);
    t_true(i2->as_string() != nullptr);

    delete i;
    delete i2;
    delete s1;
    delete s2;
    delete s3;
    delete s4;
    delete s5;
    OK("Valid String");
}

void test_row() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    Row r(dframe.get_schema());

    // types reflect schema
    t_true(r.col_type(0) == 'B');
    t_true(r.col_type(1) == 'I');
    t_true(r.col_type(2) == 'F');
    t_true(r.col_type(3) == 'S');

    // width reflects schema width
    t_true(r.width() == 4);

    // set empty row
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, new String("Hello"));

    // get all types
    t_true(r.get_bool(0) == true);
    t_true(r.get_int(1) == 0);
    t_true(float_equal(r.get_float(2), 1));
    t_true(r.get_string(3)->equals(new String("Hello")));

    // set overwrites existing values
    r.set(0, false);
    r.set(1, 1);
    r.set(2, (float)2.0);
    r.set(3, new String("Goodbye"));

    t_true(r.get_bool(0) == false);
    t_true(r.get_int(1) == 1);
    t_true(float_equal(r.get_float(2), 2));
    t_true(r.get_string(3)->equals(new String("Goodbye")));

    // row idx
    t_true(r.get_idx() == 0);
    r.set_idx(25);
    t_true(r.get_idx() == 25);

    // modifying row doesn't modify width
    t_true(r.width() == 4);

    // modifying row doesn't modify schema
    t_true(r.col_type(0) == 'B');
    t_true(r.col_type(1) == 'I');
    t_true(r.col_type(2) == 'F');
    t_true(r.col_type(3) == 'S');

    // test visit with print fielder
    PrintFielder* print_fielder = new PrintFielder();
    r.visit(0, *print_fielder);

    OK("Row");
}

void test_add_row() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    String* hello = new String("Hello");

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, hello);



    std::cout << "confused" << std::endl;
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    
    OK("Add Row");
}

void test_add_col() {

    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, new String("Hello"));
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    BoolColumn* c = new BoolColumn();
    c->push_back(true);
    c->push_back(false);
    c->push_back(true);
    c->push_back(false);
    String* c_name = new String("Column 5");
    dframe.add_column(c, c_name);

    t_true(dframe.nrows() == 4);
    t_true(dframe.ncols() == 5);
    t_true(dframe.get_bool(4, 0) == true);
    t_true(dframe.get_bool(4, 1) == false);
    t_true(dframe.get_bool(4, 2) == true);
    t_true(dframe.get_bool(4, 3) == false);
    t_true(dframe.get_col(*(new String("Column 5"))) == 4);

    Row r2(dframe.get_schema());

    dframe.fill_row(3,r2);

    t_true(r2.get_bool(0) == true);
    t_true(r2.get_int(1) == 0);
    t_true(r2.get_float(2) == (float)1.0);
    t_true(r2.get_string(3)->equals(new String("Hello")));
    t_true(r2.get_bool(4) == false);

    OK("Add Column");
}

void test_add_col_construct() {
    Schema* bifs = new Schema();
    DataFrame dframe(*bifs);
    BoolColumn* c1 = new BoolColumn();
    c1->push_back(true);
    c1->push_back(false);
    c1->push_back(true);
    IntColumn* c2 = new IntColumn();
    c2->push_back(1);
    c2->push_back(2);
    c2->push_back(3);
    dframe.add_column(c1, nullptr);
    dframe.add_column(c2, nullptr);

    t_true(dframe.nrows() == 3);
    t_true(dframe.ncols() == 2);
    t_true(dframe.get_bool(0, 0) == true);
    t_true(dframe.get_bool(0, 1) == false);
    t_true(dframe.get_bool(0, 2) == true);
    t_true(dframe.get_int(1, 0) == 1);
    t_true(dframe.get_int(1, 1) == 2);
    t_true(dframe.get_int(1, 2) == 3);

    OK("Construct with Columns");
}

void test_fill_row() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, new String("Hello"));
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    Row r2(dframe.get_schema());
    dframe.fill_row(3,r2);

    t_true(r2.get_bool(0) == true);
    t_true(r2.get_int(1) == 0);
    t_true(r2.get_float(2) == (float)1.0);
    t_true(r2.get_string(3)->equals(new String("Hello")));

    OK("Fill Row");
}

void test_data_frame_copy() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, new String("Hello"));
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    DataFrame dframe2(dframe);
    Row r2(dframe2.get_schema());
    r2.set(0, false);
    r2.set(1, 1);
    r2.set(2, (float)2.0);
    r2.set(3, new String("Hey"));
    dframe2.add_row(r2);

    t_true(dframe2.nrows() == 1);
    t_true(dframe2.ncols() == 4);
    t_true(dframe2.get_bool(0,0) == false);
    t_true(dframe2.get_int(1,0) == 1);
    t_true(dframe2.get_float(2,0) == (float)2.0);
    t_true(dframe2.get_string(3,0)->equals(new String("Hey")));
    
    OK("Copy Dataframe");
}

void test_col_names() {
    Schema* builder = new Schema();
    builder->add_column('I', nullptr);
    builder->add_column('I', new String("Integer Col"));
    builder->add_column('S', new String("String Col"));
    builder->add_column('F', new String("Float Col"));
    builder->add_column('B', new String("Bool Col"));
    builder->add_column('F', nullptr);

    t_true(builder->col_name(0) == nullptr);
    t_true(builder->col_name(1)->equals(new String("Integer Col")));
    t_true(builder->col_name(2)->equals(new String("String Col")));
    t_true(builder->col_name(3)->equals(new String("Float Col")));
    t_true(builder->col_name(4)->equals(new String("Bool Col")));
    t_true(builder->col_name(5) == nullptr);

    t_true(builder->col_idx(nullptr) == 0);
    t_true(builder->col_idx("Integer Col") == 1);
    t_true(builder->col_idx("String Col") == 2);
    t_true(builder->col_idx("Float Col") == 3);
    t_true(builder->col_idx("Bool Col") == 4);

    DataFrame dframe(*builder);
    t_true(dframe.get_col(*(new String("Integer Col"))) == 1);
    t_true(dframe.get_col(*(new String("String Col"))) == 2);
    t_true(dframe.get_col(*(new String("Float Col"))) == 3);
    t_true(dframe.get_col(*(new String("Bool Col"))) == 4);

    IntColumn* c = new IntColumn();
    dframe.add_column(c, new String("Additional Col"));
    t_true(dframe.get_col(*(new String("Additional Col"))) == 6);

    OK("Column Names");
}

void test_row_names() {
    Schema builder("IFF");
    builder.add_row(new String("Row 1"));
    builder.add_row(new String("Row 2"));
    builder.add_row(nullptr);
    builder.add_row(new String("Row 4"));

    t_true(builder.row_name(0)->equals(new String("Row 1")));
    t_true(builder.row_name(1)->equals(new String("Row 2")));
    t_true(builder.row_name(2) == nullptr);
    t_true(builder.row_name(3)->equals(new String("Row 4")));

    t_true(builder.row_idx(nullptr) == 2);
    t_true(builder.row_idx("Row 1") == 0);
    t_true(builder.row_idx("Row 2") == 1);
    t_true(builder.row_idx("Row 4") == 3);

    DataFrame dframe(builder);

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, (float)2.0);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, (float)4.0);
    dframe.add_row(r);
    
    r.set(0, 3);
    r.set(1, (float)3.0);
    r.set(2, (float)6.0);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, (float)8.0);
    dframe.add_row(r);

    t_true(dframe.get_row(*(new String("Row 1"))) == 0);
    t_true(dframe.get_row(*(new String("Row 2"))) == 1);
    t_true(dframe.get_row(*(new String("Row 4"))) == 3);

    r.set(0, 5);
    r.set(1, (float)5.0);
    r.set(2, (float)10.0);
    dframe.add_row(r);

    t_true(dframe.nrows() == 5);
    
    OK("Row Names");
}

void test_schema_names() {
    Schema* bifs = new Schema();
	DataFrame dframe(*bifs);

    BoolColumn* c1 = new BoolColumn();
    IntColumn* c2 = new IntColumn();

    String* c1_name = new String("Boolean Column");
    String* c2_name = new String("Integer Column");

    dframe.add_column(c1, c1_name);
    dframe.add_column(c2, c2_name);

    String* r1_name = new String("Row 1");
    String* r2_name = new String("Row 2");

    dframe.get_schema().add_row(r1_name);
    dframe.get_schema().add_row(r2_name);

    t_true(dframe.nrows() == 0);
    t_true(dframe.ncols() == 2);
    t_true(dframe.get_schema().length() == 2);
    t_true(dframe.get_col(*c1_name) == 0);
    t_true(dframe.get_col(*c2_name) == 1);
    t_true(dframe.get_row(*r1_name) == 0);
    t_true(dframe.get_row(*r2_name) == 1);

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 1);
    dframe.add_row(r);

    r.set(0, false);
    r.set(1, 2);
    dframe.add_row(r);

    t_true(dframe.nrows() == 2);
    t_true(dframe.ncols() == 2);
    t_true(dframe.get_schema().length() == 2);
    t_true(dframe.get_col(*c1_name) == 0);
    t_true(dframe.get_col(*c2_name) == 1);
    t_true(dframe.get_row(*r1_name) == 0);
    t_true(dframe.get_row(*r2_name) == 1);

    OK("Schema Names");
}

void test_sum_rower() {
    Schema builder("IFFSB");
    builder.add_row(new String("Row 1"));
    builder.add_row(new String("Row 2"));
    builder.add_row(nullptr);
    builder.add_row(new String("Row 4"));

    DataFrame dframe(builder);

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, (float)2.0);
    r.set(3, new String("Hi"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, (float)4.0);
    r.set(3, new String("Hey"));
    r.set(4, false);
    dframe.add_row(r);
    
    r.set(0, 3);
    r.set(1, (float)3.0);
    r.set(2, (float)6.0);
    r.set(3, new String("Hello"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, (float)8.0);
    r.set(3, new String("Sup"));
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 5);
    r.set(1, (float)5.0);
    r.set(2, (float)10.0);
    r.set(3, new String("Hej"));
    r.set(4, true);
    dframe.add_row(r);

    SumColumnRower* sum_rower_0 = new SumColumnRower(0);
    dframe.map(*sum_rower_0);
    t_true(sum_rower_0->sum_ == 15);

    SumColumnRower* sum_rower_1 = new SumColumnRower(1);
    dframe.map(*sum_rower_1);
    t_true(float_equal(sum_rower_1->sum_, 15));

    SumColumnRower* sum_rower_2 = new SumColumnRower(2);
    dframe.map(*sum_rower_2);
    t_true(float_equal(sum_rower_2->sum_, 30));

    SumColumnRower* sum_rower_3 = new SumColumnRower(3);
    dframe.map(*sum_rower_3);
    t_true(sum_rower_3->sum_ == 0);

    SumColumnRower* sum_rower_4 = new SumColumnRower(4);
    dframe.map(*sum_rower_4);
    t_true(sum_rower_4->sum_ == 0);

    SumColumnRower* sum_rower_oob = new SumColumnRower(100);
    dframe.map(*sum_rower_oob);
    t_true(sum_rower_oob->sum_ == 0);

    OK("Sum Rower");
}

void test_zero_filter() {
    Schema builder("IFISB");
    builder.add_row(new String("Row 1"));
    builder.add_row(new String("Row 2"));
    builder.add_row(nullptr);
    builder.add_row(new String("Row 4"));

    DataFrame dframe(builder);

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, 0);
    r.set(3, new String("Hi"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, 0);
    r.set(3, new String("Hey"));
    r.set(4, false);
    dframe.add_row(r);
    
    r.set(0, 0);
    r.set(1, (float)0);
    r.set(2, 0);
    r.set(3, new String("Hello"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, 0);
    r.set(3, new String("Sup"));
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 0);
    r.set(1, (float)5.0);
    r.set(2, 0);
    r.set(3, new String("Hej"));
    r.set(4, true);
    dframe.add_row(r);

    FilterIntColumnNoZero* zero_filter_0 = new FilterIntColumnNoZero(0);
    t_true(dframe.filter(*zero_filter_0)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_0)->nrows() == 3);
    t_true(dframe.filter(*zero_filter_0)->get_int(0,0) == 1);
    t_true(dframe.filter(*zero_filter_0)->get_int(0,1) == 2);
    t_true(dframe.filter(*zero_filter_0)->get_int(0,2) == 4);
    t_true(float_equal(dframe.filter(*zero_filter_0)->get_float(1,0), 1));
    t_true(float_equal(dframe.filter(*zero_filter_0)->get_float(1,1), 2));
    t_true(float_equal(dframe.filter(*zero_filter_0)->get_float(1,2), 4));
    t_true(dframe.filter(*zero_filter_0)->get_int(2,0) == 0);
    t_true(dframe.filter(*zero_filter_0)->get_int(2,1) == 0);
    t_true(dframe.filter(*zero_filter_0)->get_int(2,2) == 0);
    t_true(dframe.filter(*zero_filter_0)->get_string(3,0)->equals(new String("Hi")));
    t_true(dframe.filter(*zero_filter_0)->get_string(3,1)->equals(new String("Hey")));
    t_true(dframe.filter(*zero_filter_0)->get_string(3,2)->equals(new String("Sup")));
    t_true(dframe.filter(*zero_filter_0)->get_bool(4,0) == true);
    t_true(dframe.filter(*zero_filter_0)->get_bool(4,1) == false);
    t_true(dframe.filter(*zero_filter_0)->get_bool(4,2) == false);

    FilterIntColumnNoZero* zero_filter_1 = new FilterIntColumnNoZero(1);
    t_true(dframe.filter(*zero_filter_1)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_1)->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_2 = new FilterIntColumnNoZero(2);
    t_true(dframe.filter(*zero_filter_2)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_2)->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_3 = new FilterIntColumnNoZero(3);
    t_true(dframe.filter(*zero_filter_3)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_3)->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_4 = new FilterIntColumnNoZero(4);
    t_true(dframe.filter(*zero_filter_4)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_4)->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_5 = new FilterIntColumnNoZero(5);
    t_true(dframe.filter(*zero_filter_5)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_5)->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_oob = new FilterIntColumnNoZero(100);
    t_true(dframe.filter(*zero_filter_oob)->ncols() == 5);
    t_true(dframe.filter(*zero_filter_oob)->nrows() == 0);

    OK("Zero Filter");
}

void test_print() {
    Schema builder("IFFSB");
    builder.add_row(new String("Row 1"));
    builder.add_row(new String("Row 2"));
    builder.add_row(nullptr);
    builder.add_row(new String("Row 4"));

    DataFrame dframe(builder);
    dframe.print();

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, (float)2.0);
    r.set(3, new String("Hi"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, (float)4.0);
    r.set(3, new String("Hey"));
    r.set(4, false);
    dframe.add_row(r);
    
    r.set(0, 3);
    r.set(1, (float)3.0);
    r.set(2, (float)6.0);
    r.set(3, new String("Hello"));
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, (float)8.0);
    r.set(3, new String("Sup"));
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 5);
    r.set(1, (float)5.0);
    r.set(2, (float)10.0);
    r.set(3, new String("Hej"));
    r.set(4, true);
    dframe.add_row(r);

    dframe.print();

    OK("Print");
}

int main() {
    test_int_column();
    test_bool_column();
    test_float_column();
    test_string_column();
    test_row();
    test_add_row();
    return 0;
};
