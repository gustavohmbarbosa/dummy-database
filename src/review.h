#ifndef REVIEW_H
#define REVIEW_H

#include <stdio.h>
#include "bst.h"
#include <time.h>

typedef struct Review {
    int id;
    char *reviewer;
    char *movie;
    int rating;
    time_t timestamp;
} Review;

typedef struct ReviewList {
    Review *items;
    int length;
} ReviewList;

typedef struct Table {
	FILE *file;
	IndexTree id_index;
    IndexTree movie_index;
    IndexTree rating_index;
} Table;

Review *create_review();

#endif
