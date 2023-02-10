#include "kev_caster.h"
#include "kev_test.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

kev_caster_grid grid;
kev_caster_grid_point grid_data[9];

void test_grid_creation();
void test_intersections();
void test_persistence();
int main(int argc, char *argv)
{

    kev_test_run("Make basic 3x3 grid sealed box", test_grid_creation);
    kev_test_run("Test persistence of grid struct", test_persistence);
    kev_test_run("Test non-special case intersections w/ 3x3 grid sealed box", test_intersections);

    puts(kev_test_get_report());

    return 0;
}

void test_grid_creation()
{
    kev_caster_material mat = {1, 1, 1, 0};
    kev_caster_edge vis_edge = {1, mat, mat};
    kev_caster_edge invis_edge = {0, mat, mat};

    memcpy(grid_data, (kev_caster_grid_point[]){
        (kev_caster_grid_point){vis_edge, vis_edge},
        (kev_caster_grid_point){vis_edge, invis_edge},
        (kev_caster_grid_point){invis_edge, vis_edge},
        (kev_caster_grid_point){invis_edge, vis_edge},
        (kev_caster_grid_point){invis_edge, invis_edge},
        (kev_caster_grid_point){invis_edge, vis_edge},
        (kev_caster_grid_point){vis_edge, invis_edge},
        (kev_caster_grid_point){vis_edge, invis_edge},
        (kev_caster_grid_point){invis_edge, invis_edge}
    }, sizeof(grid_data));
    grid = (kev_caster_grid){
        3,
        3,
        grid_data
    };
    // Sample some points
    kev_test_assert_true(kev_caster_point_at(grid, 0, 0).x_running.vis == 1);
    kev_test_assert_true(kev_caster_point_at(grid, 0, 0).y_running.vis == 1);
    kev_test_assert_true(kev_caster_point_at(grid, 1, 1).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 1, 1).y_running.vis == 0); 
    kev_test_assert_true(kev_caster_point_at(grid, 2, 2).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 2, 2).y_running.vis == 0); 
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).y_running.vis == 1); 

}

void test_persistence()
{
    // Sample some points
    kev_test_assert_true(kev_caster_point_at(grid, 0, 0).x_running.vis == 1);
    kev_test_assert_true(kev_caster_point_at(grid, 0, 0).y_running.vis == 1);
    kev_test_assert_true(kev_caster_point_at(grid, 1, 1).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 1, 1).y_running.vis == 0); 
    kev_test_assert_true(kev_caster_point_at(grid, 2, 2).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 2, 2).y_running.vis == 0); 
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).y_running.vis == 1); 

}

void test_intersections()
{
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).x_running.vis == 0);
    kev_test_assert_true(kev_caster_point_at(grid, 2, 1).y_running.vis == 1); 
    kev_caster_material mat;
    char ang_bit_field = 0; // facing east;
    float res = get_face_incidence(grid, ang_bit_field, 1.2, 2, &mat);
    kev_test_assert_true(res == -1.0);

    ang_bit_field = 4; // facing west;
    res = get_face_incidence(grid, ang_bit_field, 1.2, 2, &mat);
    kev_test_assert_true(res == -1.0);

    ang_bit_field = 2; // facing north;
    res = get_face_incidence(grid, ang_bit_field, 1.2, 2, &mat);
    kev_test_assert_float_equal(res, 0.2);

    ang_bit_field = 6; // facing south;
    res = get_face_incidence(grid, ang_bit_field, 1.2, 2, &mat);
    printf("%f", res);
    kev_test_assert_float_equal(res, 0.8);

    ang_bit_field = 0; // facing east;
    res = get_face_incidence(grid, ang_bit_field, 2, 1.5, &mat);
    kev_test_assert_true(res == 0.5);
}