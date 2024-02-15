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
                int temp_color = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp_color;
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
            int temp_color = parent->color;
            parent->color = grandparent->color;
            grandparent->color = temp_color;
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

void _rb_transplant(IndexRBTree *root, IndexRBTree u, IndexRBTree v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }

    if (v != NULL) {
        v->parent = u->parent;
    }
}

IndexRBTree _rb_minimum(IndexRBTree node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

void _rb_delete_fixup(IndexRBTree *root, IndexRBTree x) {
    if (x == NULL) {
        return;
    }

    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            IndexRBTree w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                _rb_left_rotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    _rb_right_rotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                _rb_left_rotate(root, x->parent);
                x = *root;
            }
        } else {
            IndexRBTree w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                _rb_right_rotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    _rb_left_rotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                _rb_right_rotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

void rb_delete(IndexRBTree *root, int key_type, void *key_value) {
    IndexRBTree node = *root;
    while (node != NULL) {
        if (bst_value_is_equal(key_type, key_value, node->value->key)) {
            break;
        } else if (bst_value_is_smaller(key_type, key_value, node->value->key)) {
            node = node->left;
        } else {
            node = node->right;
        }
    }

    if (node == NULL) {
        return;
    }

    IndexRBTree to_delete = node;
    RBNodeColor original_color = to_delete->color;
    IndexRBTree x;

    if (node->left == NULL) {
        x = node->right;
        _rb_transplant(root, node, node->right);
    } else if (node->right == NULL) {
        x = node->left;
        _rb_transplant(root, node, node->left);
    } else {
        to_delete = _rb_minimum(node->right);
        original_color = to_delete->color;
        x = to_delete->right;

        if (to_delete->parent == node) {
            if (x != NULL) {
                x->parent = to_delete;
            }
        } else {
            _rb_transplant(root, to_delete, to_delete->right);
            to_delete->right = node->right;
            to_delete->right->parent = to_delete;
        }

        _rb_transplant(root, node, to_delete);
        to_delete->left = node->left;
        to_delete->left->parent = to_delete;
        to_delete->color = node->color;
    }

    free(node);

    if (original_color == BLACK) {
        _rb_delete_fixup(root, x);
    }
}
