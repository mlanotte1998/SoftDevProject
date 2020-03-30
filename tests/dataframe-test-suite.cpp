//lang::CwC
#include <string.h>
#include <stdio.h>
#include "../src/application/application.h"
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

    String* hello = new String("Hello");
    // set empty row
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, hello);

    // get all types
    t_true(r.get_bool(0) == true);
    t_true(r.get_int(1) == 0);
    t_true(float_equal(r.get_float(2), 1));
    t_true(r.get_string(3)->equals(hello));

    String* goodbye = new String("Goodbye");

    // set overwrites existing values
    r.set(0, false);
    r.set(1, 1);
    r.set(2, (float)2.0);
    r.set(3, goodbye);

    t_true(r.get_bool(0) == false);
    t_true(r.get_int(1) == 1);
    t_true(float_equal(r.get_float(2), 2));
    t_true(r.get_string(3)->equals(goodbye));

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

    delete goodbye;
    delete hello;

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

    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    t_true(dframe.nrows() == 4);
    t_true(dframe.ncols() == 4);

    delete hello;

    OK("Add Row");
}

void test_add_col() {

    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    String* hello = new String("Hello");

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, hello);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    BoolColumn* c = new BoolColumn();
    c->push_back(true);
    c->push_back(false);
    c->push_back(true);
    c->push_back(false);
    dframe.add_column(c);

    t_true(dframe.nrows() == 4);
    t_true(dframe.ncols() == 5);
    t_true(dframe.get_bool(4, 0) == true);
    t_true(dframe.get_bool(4, 1) == false);
    t_true(dframe.get_bool(4, 2) == true);
    t_true(dframe.get_bool(4, 3) == false);

    Row r2(dframe.get_schema());

    dframe.fill_row(3,r2);

    t_true(r2.get_bool(0) == true);
    t_true(r2.get_int(1) == 0);
    t_true(r2.get_float(2) == (float)1.0);
    t_true(r2.get_string(3)->equals(hello));
    t_true(r2.get_bool(4) == false);

    delete hello;

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
    dframe.add_column(c1);
    dframe.add_column(c2);

    t_true(dframe.nrows() == 3);
    t_true(dframe.ncols() == 2);
    t_true(dframe.get_bool(0, 0) == true);
    t_true(dframe.get_bool(0, 1) == false);
    t_true(dframe.get_bool(0, 2) == true);
    t_true(dframe.get_int(1, 0) == 1);
    t_true(dframe.get_int(1, 1) == 2);
    t_true(dframe.get_int(1, 2) == 3);

    delete bifs;

    OK("Construct with Columns");
}

void test_fill_row() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    String* hello = new String("Hello");

    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, hello);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    Row r2(dframe.get_schema());
    dframe.fill_row(3,r2);

    t_true(r2.get_bool(0) == true);
    t_true(r2.get_int(1) == 0);
    t_true(r2.get_float(2) == (float)1.0);
    t_true(r2.get_string(3)->equals(hello));

    delete hello;

    OK("Fill Row");
}

void test_data_frame_copy() {
    Schema bifs("BIFS");
    DataFrame dframe(bifs);

    String* hello = new String("Hello");
    Row r(dframe.get_schema());
    r.set(0, true);
    r.set(1, 0);
    r.set(2, (float)1.0);
    r.set(3, hello);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);
    dframe.add_row(r);

    String* hey = new String("Hey");
    DataFrame dframe2(dframe);
    Row r2(dframe2.get_schema());
    r2.set(0, false);
    r2.set(1, 1);
    r2.set(2, (float)2.0);
    r2.set(3, hey);
    dframe2.add_row(r2);

    t_true(dframe2.nrows() == 1);
    t_true(dframe2.ncols() == 4);
    t_true(dframe2.get_bool(0,0) == false);
    t_true(dframe2.get_int(1,0) == 1);
    t_true(dframe2.get_float(2,0) == (float)2.0);
    t_true(dframe2.get_string(3,0)->equals(hey));

    delete hey;
    delete hello;

    OK("Copy Dataframe");
}

void test_sum_rower() {

    Schema builder("IFFSB");

    String* hi = new String("Hi");
    String* hey = new String("Hey");
    String* hello = new String("Hello");
    String* sup = new String("sup");
    String* hej = new String("hej");

    DataFrame dframe(builder);

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, (float)2.0);
    r.set(3, hi);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, (float)4.0);
    r.set(3, hey);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 3);
    r.set(1, (float)3.0);
    r.set(2, (float)6.0);
    r.set(3, hello);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, (float)8.0);
    r.set(3, sup);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 5);
    r.set(1, (float)5.0);
    r.set(2, (float)10.0);
    r.set(3, hej);
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


    delete hi;
    delete hey;
    delete hello;
    delete sup;
    delete hej;

    delete sum_rower_0;
    delete sum_rower_1;
    delete sum_rower_2;
    delete sum_rower_3;
    delete sum_rower_4;
    delete sum_rower_oob;
    OK("Sum Rower");
}

void test_zero_filter() {
    Schema builder("IFISB");


    DataFrame dframe(builder);

    String* hi = new String("Hi");
    String* hey = new String("Hey");
    String* hello = new String("Hello");
    String* sup = new String("sup");
    String* hej = new String("hej");

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, 0);
    r.set(3, hi);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, 0);
    r.set(3, hey);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 0);
    r.set(1, (float)0);
    r.set(2, 0);
    r.set(3, hello);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, 0);
    r.set(3, sup);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 0);
    r.set(1, (float)5.0);
    r.set(2, 0);
    r.set(3, hej);
    r.set(4, true);
    dframe.add_row(r);

    FilterIntColumnNoZero* zero_filter_0 = new FilterIntColumnNoZero(0);
    DataFrame* df0 = dframe.filter(*zero_filter_0);
    t_true(df0->ncols() == 5);
    t_true(df0->nrows() == 3);
    t_true(df0->get_int(0,0) == 1);
    t_true(df0->get_int(0,1) == 2);
    t_true(df0->get_int(0,2) == 4);
    t_true(float_equal(df0->get_float(1,0), 1));
    t_true(float_equal(df0->get_float(1,1), 2));
    t_true(float_equal(df0->get_float(1,2), 4));
    t_true(df0->get_int(2,0) == 0);
    t_true(df0->get_int(2,1) == 0);
    t_true(df0->get_int(2,2) == 0);
    t_true(df0->get_string(3,0)->equals(hi));
    t_true(df0->get_string(3,1)->equals(hey));
    t_true(df0->get_string(3,2)->equals(sup));
    t_true(df0->get_bool(4,0) == true);
    t_true(df0->get_bool(4,1) == false);
    t_true(df0->get_bool(4,2) == false);

    FilterIntColumnNoZero* zero_filter_1 = new FilterIntColumnNoZero(1);
    DataFrame* df1 = dframe.filter(*zero_filter_1);
    t_true(df1->ncols() == 5);
    t_true(df1->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_2 = new FilterIntColumnNoZero(2);
    DataFrame* df2 = dframe.filter(*zero_filter_2);
    t_true(df2->ncols() == 5);
    t_true(df2->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_3 = new FilterIntColumnNoZero(3);
    DataFrame* df3 = dframe.filter(*zero_filter_3);
    t_true(df3->ncols() == 5);
    t_true(df3->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_4 = new FilterIntColumnNoZero(4);
    DataFrame* df4 = dframe.filter(*zero_filter_4);
    t_true(df4->ncols() == 5);
    t_true(df4->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_5 = new FilterIntColumnNoZero(5);
    DataFrame* df5 = dframe.filter(*zero_filter_5);
    t_true(df5->ncols() == 5);
    t_true(df5->nrows() == 0);

    FilterIntColumnNoZero* zero_filter_oob = new FilterIntColumnNoZero(100);
    DataFrame* dfoob = dframe.filter(*zero_filter_oob);
    t_true(dfoob->ncols() == 5);
    t_true(dfoob->nrows() == 0);

    delete hello;
    delete hi;
    delete hey;
    delete hej;
    delete sup;
    delete zero_filter_0;
    delete zero_filter_1;
    delete zero_filter_2;
    delete zero_filter_3;
    delete zero_filter_4;
    delete zero_filter_5;
    delete zero_filter_oob;

    delete df0;
    delete df1;
    delete df2;
    delete df3;
    delete df4;
    delete df5;
    delete dfoob;
    OK("Zero Filter");
}

void test_print() {

    String* hi = new String("Hi");
    String* hey = new String("Hey");
    String* hello = new String("Hello");
    String* sup = new String("sup");
    String* hej = new String("hej");

    Schema builder("IFFSB");

    DataFrame dframe(builder);

    Row r(builder);
    r.set(0, 1);
    r.set(1, (float)1.0);
    r.set(2, (float)2.0);
    r.set(3, hi);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 2);
    r.set(1, (float)2.0);
    r.set(2, (float)4.0);
    r.set(3, hey);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 3);
    r.set(1, (float)3.0);
    r.set(2, (float)6.0);
    r.set(3, hello);
    r.set(4, true);
    dframe.add_row(r);

    r.set(0, 4);
    r.set(1, (float)4.0);
    r.set(2, (float)8.0);
    r.set(3, sup);
    r.set(4, false);
    dframe.add_row(r);

    r.set(0, 5);
    r.set(1, (float)5.0);
    r.set(2, (float)10.0);
    r.set(3, hej);
    r.set(4, true);
    dframe.add_row(r);

    delete hey;
    delete hi;
    delete hello;
    delete hej;
    delete sup;

    OK("Print");
}

int main() {
    test_int_column();
    test_bool_column();
    test_float_column();
    test_string_column();
    test_row();
    test_add_row();
    test_add_col();
    test_add_col_construct();
    test_fill_row();
    test_data_frame_copy();
    test_sum_rower();
    test_zero_filter();
    test_print();
    return 0;
};
