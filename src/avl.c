#include "bst.h"
#include "avl.h"
#include <stdlib.h>

short int _get_balance_factor(IndexAVLTree node) {
    if (node == NULL) {
        return 0;
    }

    return bst_height((IndexTree) node->left) - bst_height((IndexTree) node->right);
}

IndexAVLNode* _right_rotate(IndexAVLNode* y) {
    IndexAVLNode* x = y->left;
    IndexAVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

IndexAVLNode* _left_rotate(IndexAVLNode* x) {
    IndexAVLNode* y = x->right;
    IndexAVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

void avl_insert(IndexAVLTree *root, Index *value) {
    if (*root == NULL) {
        *root = (IndexAVLTree) malloc(sizeof(IndexAVLNode));
        (*root)->value = value;
        (*root)->left = NULL;
        (*root)->right = NULL;
        (*root)->balance_factor = 0;
        return;
    }

    if (bst_value_is_smaller(value->key_type, value->key, (*root)->value->key)) {
        avl_insert(&(*root)->left, value);
    } else {
        avl_insert(&(*root)->right, value);
    }

    (*root)->balance_factor = _get_balance_factor(*root);

    // Left Left Case
    if ((*root)->balance_factor > 1 && bst_value_is_smaller(value->key_type, value->key, (*root)->left->value->key)) {
        *root = _right_rotate(*root);
        return;
    }

    // Right Right Case
    if ((*root)->balance_factor < -1 && bst_value_is_bigger(value->key_type, value->key, (*root)->right->value->key)) {
        *root = _left_rotate(*root);
        return;
    }

    // Left Right Case
    if ((*root)->balance_factor > 1 && bst_value_is_bigger(value->key_type, value->key, (*root)->left->value->key)) {
        (*root)->left = _left_rotate((*root)->left);
        *root = _right_rotate(*root);
        return;
    }

    // Right Left Case
    if ((*root)->balance_factor < -1 && bst_value_is_smaller(value->key_type, value->key, (*root)->right->value->key)) {
        (*root)->right = _right_rotate((*root)->right);
        *root = _left_rotate(*root);
        return;
    }
}
