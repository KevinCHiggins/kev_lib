#include "kev_dict.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    char *key;
    char *val;
} kev_dict_str_str_pair;

typedef struct 
{
    kev_dict_str_str_pair *arr;
    size_t num_pairs;
} kev_dict_str_str_arr;

struct kev_dict_str_str_bst_naive_node
{
    struct kev_dict_str_str_bst_naive_node *left;
    struct kev_dict_str_str_bst_naive_node *right;
    char *key;
    char *val;
};

typedef struct 
{
    struct kev_dict_str_str_bst_naive_node *root;
    size_t num_pairs;
} kev_dict_str_str_bst_naive;

char *fake_get(void *data, char *key)
{
    return "Fake gotten value";
}

int fake_add(void *data, char *key, char *val)
{
    return 0;
}

char *fake_remove(void *data, char *key)
{
    return "Fake removed value";
}


char *bst_naive_get_from_node(void *data, char *key)
{
    struct kev_dict_str_str_bst_naive_node *node = data;
    int result = strcmp(node->key, key);
    if (result > 0)
    {
        if (node-> left)
        {
            return bst_naive_get_from_node(node->left, key);
        }
        return NULL;
    }
    else if (result == 0)
    {
        return node->val;
    }
    else
    {
        if (node->right)
        {    
            return bst_naive_get_from_node(node->right, key);
        }
        return NULL;
    }
}

char *bst_naive_get(void *data, char *key)
{
    kev_dict_str_str_bst_naive *tree = data;
    if (tree->root)
    {
        return bst_naive_get_from_node(tree->root, key);
    }
    return NULL;
}

int bst_naive_add_node_at_node(struct kev_dict_str_str_bst_naive_node *node, struct kev_dict_str_str_bst_naive_node *added)
{
    int result = strcmp(node->key, added->key);
    if (result > 0)
    {
        if (node->left)
        {
            return bst_naive_add_node_at_node(node->left, added);
        }
        node->left = added;
        return 0;
    }
    else if (result == 0)
    {
        node->val = added->val;
        free(added);
        return 0;
    }
    else
    {
        if (node->right)
        {    
            return bst_naive_add_node_at_node(node->left, added);
        }
        node->right = added;
        return 0;
    }
}

int bst_naive_add(void *data, char *key, char *val)
{
    kev_dict_str_str_bst_naive *tree = data;
    struct kev_dict_str_str_bst_naive_node *new_node = malloc(sizeof(struct kev_dict_str_str_bst_naive_node));
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->key = key;
    new_node->val = val;
    if (tree->root)
    {
        return bst_naive_add_node_at_node(tree->root, new_node);
    }

    tree->root = new_node;
    return 0;
    
}

char *bst_naive_remove(void *data, char *key)
{
    return "Fake removed value";
}



char *arr_get(void *data, char *key)
{
    kev_dict_str_str_arr *struct_ptr = data;
    for (size_t i = 0; i < struct_ptr->num_pairs; i++)
    {
        if (strcmp(struct_ptr->arr[i].key, key) == 0)
        {
            return struct_ptr->arr[i].val;
        }
    }
    return NULL;
}

int arr_add(void *data, char *key, char *val)
{
    kev_dict_str_str_arr *struct_ptr = data;
    size_t i = 0;
    for (; i < struct_ptr->num_pairs; i++)
    {
        if (strcmp(struct_ptr->arr[i].key, key) == 0)
        {
            struct_ptr->arr[i].key = key;
            struct_ptr->arr[i].val = val;
            return 0;
        }
    }
    if (i == KEV_DICT_ARR_MAX_ELEMENTS)
    {
        return 1;
    }
    struct_ptr->arr[i].key = key;
    struct_ptr->arr[i].val = val;
    struct_ptr->num_pairs++;
    return 0;
}

char *arr_remove(void *data, char *key)
{
    return "Fake removed value";
}

struct kev_dict_str_str kev_dict_make_str_str(enum implementation implementation)
{
    struct kev_dict_str_str result;
    if (implementation == FAKE){
        result = (struct kev_dict_str_str){1, fake_get, fake_add, fake_remove, NULL};   
    }
    else if (implementation == ARRAY)
    {
        kev_dict_str_str_pair *arr = malloc(sizeof(kev_dict_str_str_pair) * KEV_DICT_ARR_MAX_ELEMENTS);
        kev_dict_str_str_arr *arr_struct = malloc(sizeof(kev_dict_str_str_arr));
        arr_struct->arr = arr;
        arr_struct->num_pairs = 0;
        result = (struct kev_dict_str_str){2, arr_get, arr_add, arr_remove, arr_struct};
    }
    else
    {
        kev_dict_str_str_bst_naive *bst_struct = malloc(sizeof(kev_dict_str_str_bst_naive));
        bst_struct->root = NULL;
        bst_struct->num_pairs = 0;
        result = (struct kev_dict_str_str){3, bst_naive_get, bst_naive_add, bst_naive_remove, bst_struct};  
    }

    return result;
}
