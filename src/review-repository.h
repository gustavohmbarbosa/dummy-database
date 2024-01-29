#ifndef REVIEW_REPOSITORY_H
#define REVIEW_REPOSITORY_H

#include "review.h"

#define REVIEWS_FILENAME "reviews.dat"

#define REVIEWS_INDEX_ID "reviews_id.idx"
#define REVIEWS_INDEX_MOVIE "reviews_movie.idx"
#define REVIEWS_INDEX_RATING "reviews_rating.idx"

void boot_reviews(Table *table);

void shutdown_reviews(Table *table);

void store_review(Table *table, Review *review);

// int save_all_reviews(ReviewList *list);
// ReviewList *load_all_reviews();

#endif
