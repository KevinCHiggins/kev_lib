#include "kev_matrix.h"

float *kev_mat_mult(float **m, float *v)
{
    float *result = malloc(3 * sizeof(float));
    result[0] = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    result[1] = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
    result[2] = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
    return result;
}