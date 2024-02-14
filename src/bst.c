#include "bst.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _is_smaller(int key_type, void *smallest, void *biggest) {
    switch (key_type) {
        case KEY_TYPE_INT:
            return (*(int *) smallest < *(int *) biggest);
        case KEY_TYPE_STRING:
            return (strcmp((char *) smallest, (char *) biggest) > 0);
        default:
            printf("Invalid key type: %d\n", key_type);
            exit(1);
    }
}

int _is_bigger(int key_type, void *biggest, void *smallest) {
    switch (key_type) {
        case KEY_TYPE_INT:
            return (*(int *) biggest > *(int *) smallest);
        case KEY_TYPE_STRING:
            return (strcmp((char *) smallest, (char *) biggest) < 0);
        default:
            printf("Invalid key type: %d\n", key_type);
            exit(1);
    }
}

int _is_equal(int key_type, void *first, void *second) {
    switch (key_type) {
        case KEY_TYPE_INT:
            return (*(int *) first == *(int *) second);
        case KEY_TYPE_STRING:
            return (strcmp((char *) first, (char *) second) == 0);
        default:
            printf("Invalid key type: %d\n", key_type);
            exit(1);
    }
}

void bst_insert(IndexTree *root, Index *value) {
    if (*root == NULL) {
        *root = (IndexTree) malloc(sizeof(Index)); 
        (*root)->right = NULL;
        (*root)->left = NULL;
        (*root)->value = value;
        return;
    }

    if (_is_smaller(value->key_type, value->key, (*root)->value->key)) {
        bst_insert(&(*root)->left, value);
    } else {
        bst_insert(&(*root)->right, value);
    }
}

void bst_cut_sheet(IndexTree *sheet) {
    free(*sheet);
    *sheet = NULL;
}

void bst_remove(IndexTree *root, int key_type, void *key_value) {
    if (*root == NULL) {
        return;
    }
    
    if (_is_smaller(key_type, key_value, (*root)->value->key)) {
        return bst_remove(&(*root)->left, key_type, key_value);
    }

    if (_is_bigger(key_type, key_value, (*root)->value->key)) {
        return bst_remove(&(*root)->right, key_type, key_value);
    }

    if (!_is_equal(key_type, key_value, (*root)->value->key)) {
        return;
    }

    if ((*root)->left == NULL && (*root)->right == NULL) {
        bst_cut_sheet(root);
        return;
    }

    if ((*root)->left == NULL) {
        IndexTree temp = (*root)->right;
        *root = temp;
        free((*root)->right);
        return;
    }

    if ((*root)->right == NULL) {
        IndexTree temp = (*root)->left;
        *root = temp;
        free((*root)->left);
        return;
    }

    IndexTree *inorder_successor = &(*root)->right;
    while ((*inorder_successor)->left != NULL) {
        inorder_successor = &(*inorder_successor)->left;
    }

    (*root)->value = (*inorder_successor)->value;
    bst_cut_sheet(inorder_successor);
}

void bst_search_offset(IndexTree root, Index *value) {
    if (root == NULL) {
        value->offset = -1;
        return;
    }

    if (_is_smaller(value->key_type, value->key, root->value->key)) {
        return bst_search_offset(root->left, value);
    }

    if (_is_bigger(value->key_type, value->key, root->value->key)) {
        return bst_search_offset(root->right, value);
    }

    if (_is_equal(value->key_type, value->key, root->value->key)) {
        value->offset = root->value->offset;
        return;
    }

    value->offset = -1;
}
