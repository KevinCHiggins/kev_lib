#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include "kev_test.h"

jmp_buf test_run;

char *kev_test_report;
size_t kev_test_report_len;
size_t kev_test_report_buff_len = 2;
char *kev_test_running_test;
const char *kev_test_FAILED = "Test failed:\t";
const char *kev_test_SUCCEEDED = "Test succeeded:\t";
int kev_test_tests_run;
int kev_test_tests_passed;
int kev_test_assertions_evaluated_in_this_test;

void kev_test_report_add_line(char *line)
{
    size_t line_len = strlen(line);
    while (kev_test_report_len + line_len + 1 >= kev_test_report_buff_len)
    {
        kev_test_report_buff_len = kev_test_report_buff_len * 2;
        kev_test_report = realloc(kev_test_report, kev_test_report_buff_len);
        //printf("Name len: %ld; previous report len: %ld; sum len: %ld; buff len: %ld\n", name_len, kev_test_report_len, name_len + kev_test_report_len, kev_test_report_buff_len);
    }
    memcpy(kev_test_report + kev_test_report_len, line, line_len);
    kev_test_report_len += line_len + 1;
    kev_test_report[kev_test_report_len - 1] = '\n';
    kev_test_report[kev_test_report_len] = (char)0;
}

void kev_test_report_add_failure(char *name)
{
    size_t msg_len = strlen(kev_test_FAILED);
    size_t name_len = strlen(name);
    char full_line[name_len + msg_len + 1];
    memcpy(full_line, kev_test_FAILED, msg_len);
    memcpy(full_line + msg_len, name, name_len + 1);
    kev_test_report_add_line(full_line);
}

void kev_test_report_add_success(char *name)
{
    size_t msg_len = strlen(kev_test_SUCCEEDED);
    size_t name_len = strlen(name);
    char full_line[name_len + msg_len + 1];
    memcpy(full_line, kev_test_SUCCEEDED, msg_len);
    memcpy(full_line + msg_len, name, name_len + 1);
    kev_test_report_add_line(full_line);
}

void kev_test_run(char *name, void (*test_function_ptr)())
{
    if (kev_test_running_test)
    {
        printf("Cannot start a test if there is already a test running. Exiting.\n");
        exit(1);
    }

    if (!kev_test_report)
    {
        kev_test_report = malloc(kev_test_report_buff_len);
    }

    kev_test_running_test = name;
    kev_test_assertions_evaluated_in_this_test = 0;

    if (setjmp(test_run))
    {
        kev_test_report_add_failure(kev_test_running_test);
    }
    else
    {
        test_function_ptr();
        kev_test_tests_passed++;
        kev_test_report_add_success(kev_test_running_test);
    }

    kev_test_tests_run++;
    kev_test_running_test = NULL;
}

char *kev_test_get_report()
{
    char *summary = malloc(sizeof(char) * 100);

    sprintf(summary, "%d/%d tests passed.", kev_test_tests_passed, kev_test_tests_run);
    kev_test_report_add_line(summary);

    return kev_test_report;
}

void kev_test_assert_str_equal(char *s1, char *s2)
{
    if (!kev_test_running_test)
    {
        printf("Assertions can only be made while a test is running. Exiting.\n");
        exit(1);
    }
    if (strcmp(s1, s2) != 0)
    {
        longjmp(test_run, -1);
    }
    kev_test_assertions_evaluated_in_this_test++;
}

void kev_test_assert_int_equal(int i, int j)
{
    if (!kev_test_running_test)
    {
        printf("Assertions can only be made while a test is running. Exiting.\n");
        exit(1);
    }
    if (i != j)
    {
        longjmp(test_run, -1);
    }
    kev_test_assertions_evaluated_in_this_test++;
}

void kev_test_assert_true(int b)
{
    if (!kev_test_running_test)
    {
        printf("Assertions can only be made while a test is running. Exiting.\n");
        exit(1);
    }   
    if (!b)
    {
        longjmp(test_run, -1);
    }
    kev_test_assertions_evaluated_in_this_test++;
}
