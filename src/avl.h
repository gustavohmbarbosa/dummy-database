#ifndef AVL_H
#define AVL_H

#include "bst.h"

typedef struct IndexAVLNode {
	Index *value;
	struct IndexAVLNode *left, *right;
    short int balance_factor;
} IndexAVLNode;

typedef IndexAVLNode *IndexAVLTree;

void avl_insert(IndexAVLTree *root, Index *value);

#endif
