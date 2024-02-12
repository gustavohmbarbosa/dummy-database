#include "review.h"
#include "review-repository.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constants.h"

void load_bst_index(char *filename, IndexTree *tree) {
    *tree = NULL;

    FILE *file = fopen(filename, "r+b");
    if (file == NULL) {
        return;
    }

    Index *temp = NULL;
    long offset = 0;
    fseek(file, 0L, SEEK_SET);
    do {
        temp = (Index *) malloc(sizeof(Index));
        offset = fscanf(file, "%d;", &temp->key_type);

        switch(temp->key_type) {
            case 0:
                free(temp);
                fclose(file);
                return;
            case KEY_TYPE_INT:
                temp->key = (int *) malloc(sizeof(int));
                fscanf(file, "%d;%d\n", (int *) temp->key, &(temp->offset));
                break;
            case KEY_TYPE_STRING:
                fscanf(file, "%m[^;];%d\n", (char **) &temp->key, &(temp->offset));
                break;
            default:
                printf("Invalid key type: %d\n", temp->key_type);
                exit(1);
        }
        bst_insert(tree, temp);
    } while (offset != EOF);

    fclose(file);
}

void _bst_write_pre_order(IndexTree root, FILE *file) {
    if (root == NULL) {
        return;
    }

    switch (root->value->key_type) {
        case KEY_TYPE_INT:
            fprintf(file, "%d;%d;%d\n", root->value->key_type, *(int *) root->value->key, root->value->offset);
            break;
        case KEY_TYPE_STRING:
            fprintf(file, "%d;%s;%d\n", root->value->key_type, (char *) root->value->key, root->value->offset);
            break;
        default:
            printf("Invalid key type: %d\n", root->value->key_type);
            exit(1);
    }

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

void boot_reviews(Table *table) {
    FILE *file = fopen(REVIEWS_FILENAME, "r+b");
    if (file == NULL) {
        printf("Error opening %s", REVIEWS_FILENAME);
        exit(1);
    }

    table->file = file;
    load_bst_index(REVIEWS_INDEX_ID, &table->id_index);
    load_bst_index(REVIEWS_INDEX_MOVIE, &table->movie_index);
    load_bst_index(REVIEWS_INDEX_RATING, &table->rating_index);
}

void shutdown_reviews(Table *table) {
    fclose(table->file);
    bst_write_pre_order(table->id_index, REVIEWS_INDEX_ID);
    bst_write_pre_order(table->movie_index, REVIEWS_INDEX_MOVIE);
    bst_write_pre_order(table->rating_index, REVIEWS_INDEX_RATING);
}

int get_next_id(IndexTree tree) {
    if (tree == NULL) {
        return 1;
    }

    int left = get_next_id(tree->left);
    int right = get_next_id(tree->right);

    return (left > right ? left : right) + 1;
}

void store_review(Table *table, Review *review) {
    if (table->file == NULL) {
        return;
    }

    review->id = get_next_id(table->id_index);
    time(&review->timestamp);

    fseek(table->file, 0L, SEEK_END);
    int offset = ftell(table->file);

    Index *id_index = (Index *) malloc(sizeof(Index));
    id_index->key_type = KEY_TYPE_INT;
    id_index->key = &review->id;
    id_index->offset = offset;

    Index *rating_index = (Index *) malloc(sizeof(Index));
    rating_index->key_type = KEY_TYPE_INT;
    rating_index->key = &review->rating;
    rating_index->offset = offset;
    
    Index *movie_index = (Index *) malloc(sizeof(Index));
    movie_index->key_type = KEY_TYPE_STRING;
    movie_index->key = review->movie;
    movie_index->offset = offset;

    fprintf(table->file, "%d;%s;%s;%d;%ld\n", review->id, review->reviewer, review->movie, review->rating, review->timestamp);
    bst_insert(&table->id_index, id_index);
    bst_insert(&table->rating_index, rating_index);
    bst_insert(&table->movie_index, movie_index);
}

void print_review(Table *table, int offset) {
    fseek(table->file, offset, SEEK_SET);
    Review *review = (Review *) malloc(sizeof(Review));
    fscanf(table->file, "%d;%m[^;];%m[^;];%d;%ld\n", &review->id, &review->reviewer, &review->movie, &review->rating, &review->timestamp);
    printf("FILE OFFSET: %d\n", offset);
    printf("ID: %d\n", review->id);
    printf("Reviewer: %s\n", review->reviewer);
    printf("Movie: %s\n", review->movie);
    printf("Rating: %d\n", review->rating);
    printf("Timestamp: %ld\n\n", review->timestamp);
    free(review);
}

void print_reviews(Table *table, IndexTree tree) {
    if (tree == NULL || table->file == NULL) {
        return;
    }

    print_reviews(table, tree->left);
    print_review(table, tree->value->offset);    
    print_reviews(table, tree->right);
}
