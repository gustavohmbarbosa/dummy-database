#include <stdlib.h>
#include "rb.h"

void _rb_left_rotate(IndexRBTree *root, IndexRBTree x) {
    IndexRBTree y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void _rb_right_rotate(IndexRBTree *root, IndexRBTree y) {
    IndexRBTree x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NULL) {
        *root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void _fix(IndexRBTree *root, IndexRBTree newNode) {
    IndexRBTree parent = NULL;
    IndexRBTree grandparent = NULL;

    while ((newNode != *root) && (newNode->color != BLACK) && (newNode->parent->color == RED)) {
        parent = newNode->parent;
        grandparent = newNode->parent->parent;

        // Case 1: Parent of newNode is left child of Grand-parent of newNode
        if (parent == grandparent->left) {
            IndexRBTree uncle = grandparent->right;

            // Case 1a: Uncle is also red. Only recoloring needed.
            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                newNode = grandparent;
            } else {
                // Case 1b: newNode is right child of parent. Left-rotation required.
                if (newNode == parent->right) {
                    _rb_left_rotate(root, parent);
                    newNode = parent;
                    parent = newNode->parent;
                }

                // Case 1c: newNode is left child of parent. Right-rotation required.
                _rb_right_rotate(root, grandparent);
                int tempColor = parent->color;
                parent->color = grandparent->color;
                grandparent->color = tempColor;
                newNode = parent;
            }
            continue;
        }
        
        // Case 2: Parent of newNode is right child of Grand-parent of newNode
        IndexRBTree uncle = grandparent->left;

        // Case 2a: Uncle is also red. Only recoloring needed.
        if ((uncle != NULL) && (uncle->color == RED)) {
            grandparent->color = RED;
            parent->color = BLACK;
            uncle->color = BLACK;
            newNode = grandparent;
        } else {
            // Case 2b: newNode is left child of parent. Right-rotation required.
            if (newNode == parent->left) {
                _rb_right_rotate(root, parent);
                newNode = parent;
                parent = newNode->parent;
            }

            // Case 2c: newNode is right child of parent. Left-rotation required.
            _rb_left_rotate(root, grandparent);
            int tempColor = parent->color;
            parent->color = grandparent->color;
            grandparent->color = tempColor;
            newNode = parent;
        }
    }

    (*root)->color = BLACK;
}

void rb_insert(IndexRBTree *root, Index *value) {
    IndexRBTree new_node = (IndexRBTree) malloc(sizeof(IndexRBNode));
    new_node->value = value;
    new_node->color = RED;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    IndexRBTree parent = NULL;
    IndexRBTree current = *root;
    while (current != NULL) {
        parent = current;
        if (bst_value_is_smaller(new_node->value->key_type, new_node->value->key, (*root)->value->key)) {
            current = current->left;
            continue;
        }
        
        current = current->right;
    }
    new_node->parent = parent;

    if (parent == NULL) {
        *root = new_node;
    } else if (bst_value_is_smaller(new_node->value->key_type, new_node->value->key, parent->value->key)) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }

    _fix(root, new_node);
}
