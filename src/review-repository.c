#include "review.h"
#include "review-repository.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "constants.h"

void load_bst_index(char *filename, IndexTree *tree) {
    *tree = NULL;

    FILE *file = fopen(filename, "rb");
    Index *temp;
    if (file == NULL) {
        return;
    }

    temp = (Index *) malloc(sizeof(Index));
    while (fread(temp, sizeof(Index), 1, file)) {
        bst_insert(tree, temp);
        temp = (Index *) malloc(sizeof(Index));
    }
    fclose(file);
}

void boot_reviews(Table *table) {
    FILE *file = fopen(REVIEWS_FILENAME, "a+b");
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















// int save_all_reviews(ReviewList *list) {
//     FILE *file = fopen(REVIEWS_FILENAME, "wb");

//     if (file == NULL) {
//         perror("Error opening file");
//         return 0;
//     }

//     fwrite(list->items, sizeof(Review), list->length, file);
//     fclose(file);
//     return 1;
// }

// ReviewList *load_all_reviews() {
//     ReviewList *list = NULL;

//     FILE *file = fopen(REVIEWS_FILENAME, "rb");
//     if (file == NULL) {
//         perror("Error opening file");
//         return list;
//     }

//     fseek(file, 0, SEEK_END);
//     long file_size = ftell(file);
//     rewind(file);

//     list = (ReviewList *) malloc(sizeof(ReviewList));
//     list->length = file_size / sizeof(Review);
//     list->items = (Review *) malloc(list->length * sizeof(Review));

//     fread(list->items, sizeof(Review), list->length, file);
//     fclose(file);

//     return list;
// }
