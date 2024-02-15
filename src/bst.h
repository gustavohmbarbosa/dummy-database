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

int bst_value_is_smaller(int key_type, void *smallest, void *biggest);

int bst_value_is_bigger(int key_type, void *biggest, void *smallest);

int bst_value_is_equal(int key_type, void *first, void *second);

void bst_insert(IndexTree *root, Index *value);

void bst_remove(IndexTree *root, int key_type, void * key_value);

void bst_search_offset(IndexTree root, Index *value);

#endif
