#include <stddef.h>
#define KEV_DICT_ARR_MAX_ELEMENTS 4

enum implementation{FAKE, ARRAY, BST_NAIVE};

struct kev_dict_str_str kev_dict_make_str_str(enum implementation);

struct kev_dict_str_str
{
    int implementation_id;
    char * (*get)(void *, char *); // returns NULL for not found
    int (*add)(void *, char *, char *); // returns error code on failure (out of capacity)
    char * (*remove)(void *, char *); // returns NULL for not found
    void *data;
};