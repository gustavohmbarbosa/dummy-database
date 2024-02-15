#ifndef RB_H
#define RB_H

#include "bst.h"

typedef enum { RED, BLACK } RBNodeColor;

typedef struct IndexRBNode {
	Index *value;
	struct IndexRBNode *left, *right, *parent;
    RBNodeColor color;
} IndexRBNode;

typedef IndexRBNode *IndexRBTree;

void rb_insert(IndexRBTree *root, Index *value);

// void rb_delete(IndexRBTree *root, int key_type, void *key_value);

#endif
