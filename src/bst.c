#include "bst.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bst_insert(IndexTree *root, Index *value) {
    if (*root == NULL) {
        *root = (IndexTree) malloc(sizeof(Index)); 
        (*root)->right = NULL;
        (*root)->left = NULL;
        (*root)->value = value;
        return;
    }

    int is_smaller;
    switch (value->key_type) {
        case KEY_TYPE_INT:
            is_smaller = (*(int *) value->key < *(int *) (*root)->value->key);
            break;
        case KEY_TYPE_STRING:
            is_smaller = (strcmp((char *) value->key, (char *) (*root)->value->key) < 0);
            break;
        default:
            printf("Invalid key type: %d\n", value->key_type);
            exit(1);
    }

    if (is_smaller) {
        bst_insert(&(*root)->left, value);
    } else {
        bst_insert(&(*root)->right, value);
    }
}

void _bst_write_pre_order(IndexTree root, FILE *file) {
    if (root == NULL) {
        return;
    }

    fwrite(root->value, sizeof(Index), 1, file);
    _bst_write_pre_order(root->left, file);
    _bst_write_pre_order(root->right, file);
}

void bst_write_pre_order(IndexTree root, char *filename) {
    if (root == NULL) {
        return;
    }

	FILE *file;
	file = fopen(filename, "wb");
	if (file != NULL) {
        _bst_write_pre_order(root, file);
		fclose(file);
	}
}

