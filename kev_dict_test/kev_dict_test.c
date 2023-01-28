#include "kev_dict.h"
#include "kev_test.h"
#include <stdio.h>

void test_dict_fake();
void test_dict_arr();
void test_dict_arr_longer();
void test_dict_bst_naive();
void kev_dict_test(struct kev_dict_str_str);
void kev_dict_test_longer(struct kev_dict_str_str);

int main(int argc, char *argv)
{

    kev_test_run("Fake implementation, string:string.", test_dict_fake);

    kev_test_run("Array implementation, string:string.", test_dict_arr);

    kev_test_run("Array implementation, string:string, longer.", test_dict_arr_longer);

    kev_test_run("Naive BST implementation, string:string.", test_dict_bst_naive);

    puts(kev_test_get_report());

    return 0;
}

void test_dict_fake()
{
    struct kev_dict_str_str d;
    d = kev_dict_make_str_str(FAKE);
    kev_dict_test(d);
}

void test_dict_arr()
{
    struct kev_dict_str_str d;
    d = kev_dict_make_str_str(ARRAY);
    kev_dict_test(d);
}

void test_dict_arr_longer()
{
    struct kev_dict_str_str d;
    d = kev_dict_make_str_str(ARRAY);
    kev_dict_test_longer(d);
}

void test_dict_bst_naive()
{
    struct kev_dict_str_str d;
    d = kev_dict_make_str_str(BST_NAIVE);
    //kev_dict_test(d);
    char *result;
    result = d.get(d.data, "Test");
    kev_test_assert_true(result == NULL);
}

void kev_dict_test(struct kev_dict_str_str d)
{
    char *result;
    result = d.get(d.data, "Test");
    kev_test_assert_true(result == NULL);
    d.add(d.data, "Test", "yep");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    d.add(d.data, "Test2", "nope");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    result = d.get(d.data, "Test2");
    kev_test_assert_str_equal(result, "nope");
    d.add(d.data, "Test3", "hmm");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    result = d.get(d.data, "Test3");
    kev_test_assert_true(result != NULL);
    kev_test_assert_str_equal(result, "hmm");

}

void kev_dict_test_longer(struct kev_dict_str_str d)
{
    char *result;
    result = d.get(d.data, "Test");
    kev_test_assert_true(result == NULL);
    d.add(d.data, "Test", "yep");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    d.add(d.data, "Test2", "nope");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    result = d.get(d.data, "Test2");
    kev_test_assert_str_equal(result, "nope");
    d.add(d.data, "Test3", "hmm");
    result = d.get(d.data, "Test");
    kev_test_assert_str_equal(result, "yep");
    result = d.get(d.data, "Test3");
    kev_test_assert_true(result != NULL);
    kev_test_assert_str_equal(result, "hmm");
    d.add(d.data, "More", "fruit");
    d.add(d.data, "Even more", "veg");
    d.add(d.data, "Yet more", "nanners");
    d.add(d.data, "And more", "ham");
    result = d.get(d.data, "Test");
    kev_test_assert_true(result != NULL);
    kev_test_assert_str_equal(result, "yep");
    result = d.get(d.data, "Test3");
    kev_test_assert_true(result != NULL);
    kev_test_assert_str_equal(result, "hmm");
    result = d.get(d.data, "Even more");
    kev_test_assert_true(result != NULL);
    kev_test_assert_str_equal(result, "veg");    

}