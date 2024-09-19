#include "kev_test.h"
#include <stdio.h>
#include <math.h>

void test_floats_exactly()
{
    float a = 1.2;
    float b = 1.0;
    float c = a - b;
    kev_test_assert_true(c == 0.2);
}

// worked with KEV_TEST_FLOAT_MARGIN = 0.00000005;
void test_floats_with_margin()
{
    float a = 1.2;
    float b = 1.0;
    float c = a - b;
    kev_test_assert_float_equal(c, 0.2);
}

// worked with KEV_TEST_FLOAT_MARGIN = 0.00000006;
void test_floats_needing_bigger_margin()
{
    float b = 1;
    float a = modff(1.2, &b);

    float c = b - a;
    kev_test_assert_float_equal(c, 0.8);
}


void test_doubles_exactly()
{
    double a = 1.2;
    double b = 1.0;
    double c = a - b;
    kev_test_assert_true(c == 0.2);
}

// works with KEV_TEST_DOUBLE_MARGIN = 0.00000000000000006;
void test_doubles_with_margin()
{
    double a = 1.2;
    double b = 1.0;
    double c = a - b;
    kev_test_assert_double_equal(c, 0.2);
}


int main(int argc, char *argv)
{

    kev_test_run("1.2 - 1 = 0.2 - PRECISELY", test_floats_exactly);
    kev_test_run("1.2 - 1 = 0.2 - WITH MARGIN", test_floats_with_margin);
    kev_test_run("1.0 - 0.2 = 0.8 - WITH MARGIN", test_floats_needing_bigger_margin);
    
    kev_test_run("1.2 - 1 = 0.2 - DOUBLES PRECISELY", test_doubles_exactly);
    kev_test_run("1.2 - 1 = 0.2 - DOUBLES WITH MARGIN", test_doubles_with_margin);
    puts(kev_test_get_report());
}


