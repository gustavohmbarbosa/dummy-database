#include "review.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Review *create_review() {
    Review *review = (Review *) malloc(sizeof(Review));

	getchar();
    printf("Qual o seu nome? ");
    size_t len = 0;
    getline(&review->reviewer, &len, stdin);
    review->reviewer[strcspn(review->reviewer, "\n")] = '\0';  // Remove newline if present

    printf("Qual o nome do filme? ");
    getline(&review->movie, &len, stdin);
    review->movie[strcspn(review->movie, "\n")] = '\0';  // Remove newline if present

    printf("Qual a nota? ");
    scanf("%d", &review->rating);

    return review;
}
