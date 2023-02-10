#include "kev_test.h"
#include <stdio.h>
#include <math.h>

void test_floats_exactly();
void test_floats_with_margin();
void test_floats_needing_bigger_margin();
int main(int argc, char *argv)
{

    kev_test_run("1.2 - 1 = 0.2 - PRECISELY", test_floats_exactly);
    kev_test_run("1.2 - 1 = 0.2 - WITH MARGIN", test_floats_with_margin);
    kev_test_run("1.0 - 0.2 = 0.8 - WITH MARGIN", test_floats_needing_bigger_margin);
    puts(kev_test_get_report());
}

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