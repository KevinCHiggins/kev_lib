#include "kev_dict.h"
#include "kev_test.h"
#include <stdio.h>

void test_nothing();

int main(int argc, char *argv)
{

    kev_test_run("Baaaaa.", test_nothing);

    kev_test_run("A.", test_nothing);

    kev_test_run("B.", test_nothing);

    kev_test_run("Broken? Longer name.", test_nothing);

    kev_test_run("Broken? A very much longer name with more parts to it that runs on for a while.", test_nothing);

    kev_test_run("Broken? A very much longer name with more parts to it that runs on for a while.", test_nothing);

    kev_test_run("Broken? A very much longer name with more parts to it that runs on for a while more.", test_nothing);

    kev_test_run("Broken? A short name.", test_nothing);

    puts(kev_test_get_report());

    return 0;
}

void test_nothing()
{
    int a = 1;
    kev_test_assert_true(a == 1);
}