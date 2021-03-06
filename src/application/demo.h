#include "application.h"
#include "../dataframe/dataframe.h"
#include "../store/kdstore.h"

#include <stdlib.h>



/**
 * Demo Application given in Project Milestone 1
 * authors: jv
*/
class Demo : public Application {
public:
    Key main{"main", 0};
    Key verify{"verif", 0};
    Key check{"ck", 0};

    Demo(size_t idx, KDStore *kd) : Application(idx, kd) {}

    void run_() override {
        switch (this_node()) {
            case 0:
                producer();
                break;
            case 1:
                counter();
                break;
            case 2:
                summarizer();
            case 3:
                break;
        }
    }


    void producer() {
        size_t SZ = 100 * 1000;
        double *vals = new double[SZ];
        double sum = 0;
        for (size_t i = 0; i < SZ; ++i) sum += vals[i] = i;
        DataFrame::fromArray(&main, kv, SZ, vals);
        DataFrame::fromScalar(&check, kv, sum);
    }


    void counter() {
        DataFrame *v = kv->waitAndGet(main);
        size_t sum = 0;
        for (size_t i = 0; i < 100 *1000; ++i) sum += v->get_double(0, i);
        p("The sum is  ").pln(sum);
        DataFrame* s = DataFrame::fromScalar(&verify, kv, sum);
        delete v;
        delete s;
    }

    void summarizer() {
        DataFrame *result = kv->waitAndGet(verify);
        DataFrame *expected = kv->waitAndGet(check);
        pln(expected->get_double(0, 0) == result->get_double(0, 0) ? "SUCCESS" : "FAILURE");
        delete result;
        delete expected;
    }

};
