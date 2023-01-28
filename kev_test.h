#ifndef KEV_TEST_H
#define KEV_TEST_H
void kev_test_run(char *name, void (*function_ptr)());
char *kev_test_get_report(void);
void kev_test_assert_str_equal(char *s1, char *s2);
void kev_test_assert_int_equal(int i, int j);
void kev_test_assert_true(int b);
int kev_test_this_test_failed;
//
#endif
