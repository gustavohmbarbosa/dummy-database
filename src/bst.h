#ifndef BST_H
#define BST_H

typedef struct Index {
	void *key;
    int key_type;
	int offset; // posição no arquivo do primeiro byte do registro
} Index;

typedef struct IndexNode {
	Index *value;
	struct IndexNode *left, *right;
} IndexNode;

typedef IndexNode * IndexTree;

void bst_insert(IndexTree *root, Index *value);

void bst_remove(IndexTree *root, Index *value);

void bst_search_offset(IndexTree root, Index *value);

#endif
