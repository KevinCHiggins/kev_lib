#include "kev_matrix.h"
#include "kev_test.h"
#include <stdio.h>

void test_mult_mat3x3_vec3();

int main(int argc, char *argv)
{

    kev_test_run("3x3 matrix * vec3.", test_mult_mat3x3_vec3);

    puts(kev_test_get_report());

    return 0;
}

void test_mult_mat3x3_vec3()
{
    float pos[3] = {12.0, 20.0, 1.0};
    float translate[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {10.0, 15.0, 1.0}};
    
    float *result = kev_mat_mult((float **) translate, pos);
    kev_test_assert_float_equal(result[0], 22.0);
    kev_test_assert_float_equal(result[1], 35.0);
    kev_test_assert_float_equal(result[2], 1.0);

}
