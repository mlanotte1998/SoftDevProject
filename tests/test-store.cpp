//lang::CwC
#include <string.h>
#include <stdio.h>
#include "../src/application/application.h"

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

// testing basic put and get functionality
void test_put_and_get() {
		KDStore* k = new KDStore(0);

		Schema onecol("I");
    DataFrame* dframe = new DataFrame(onecol);

    Row r(dframe->get_schema());
    r.set(0, 3);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);

		int sum = 0;
		for (int i = 0; i < 4; i++) {
			sum += dframe->get_int(0, i);
		}

		Key key("first",0);
		k->put(key, dframe);

		DataFrame* dframe2 = k->get(key);

		for (int i = 0; i < 4; i++) {
			sum -= dframe->get_int(0, i);
		}
		t_true(sum==0);

		delete k;
		delete dframe;
    OK("Put and Get");
}

// testing handling of multiple kd pair entries
void put_multiple() {
		KDStore* k = new KDStore(0);

		Schema onecol("I");
    DataFrame* dframe = new DataFrame(onecol);

    Row r(dframe->get_schema());
    r.set(0, 3);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);

		int sum = 0;
		for (int i = 0; i < 4; i++) {
			sum += dframe->get_int(0, i);
		}

		DataFrame* dframe2 = new DataFrame(onecol);

    Row r2(dframe2->get_schema());
    r2.set(0, 5);
    dframe2->add_row(r2);
    dframe2->add_row(r2);
    dframe2->add_row(r2);
    dframe2->add_row(r2);

		int sum2 = 0;
		for (int i = 0; i < 4; i++) {
			sum2 += dframe2->get_int(0, i);
		}

		Key key("first",0);
		k->put(key, dframe);

		Key key2("second",0);
		k->put(key2, dframe2);

		DataFrame* dframe01 = k->get(key);
		for (int i = 0; i < 4; i++) {
			sum -= dframe01->get_int(0, i);
		}

		DataFrame* dframe02 = k->get(key2);
		for (int i = 0; i < 4; i++) {
			sum2 -= dframe02->get_int(0, i);
		}

		t_true(sum==0);
		t_true(sum2==0);

		delete k;
		delete dframe;
		delete dframe2;
    OK("Put Multiple");
}

// testing that retrieving df by key not in kvstore returns nullptr
void test_invalid_key() {
		KDStore* k = new KDStore(0);

		Schema onecol("I");
    DataFrame* dframe = new DataFrame(onecol);

    Row r(dframe->get_schema());
    r.set(0, 3);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);
    dframe->add_row(r);

		Key key("first",0);
		k->put(key, dframe);

		Key key2("second",0);
		DataFrame* dframe2 = k->get(key2);

		t_true(dframe2==nullptr);

		delete k;
		delete dframe;
    OK("Invalid Key");
}

// main function
int main() {
		test_put_and_get();
		put_multiple();
		test_invalid_key();
    return 0;
};
