#include "review.h"
#include "review-repository.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	Table table;
	int option;
	boot_reviews(&table);

	while(1) {
        printf("==============================\n");
        printf("     AVALIAÇÕES DE FILMES\n");
        printf("==============================\n");
        printf("1 - Nova avaliação\n");
        printf("2 - Buscar avaliação por ID\n");
        // printf("3 - Buscar avaliação por filme\n");
        // printf("4 - Buscar avaliação por nota\n");
        printf("99 - Sair\n");

        printf("Escolha uma opção: ");
		scanf("%d", &option);
        switch(option) {
            case 1:
                store_review(&table, create_review());
                system("clear");
                printf("Avaliação realizada com sucesso!\n\n");
                break;
            case 99:
                shutdown_reviews(&table);
                exit(0);
        }
    }



















    // ReviewList *list = load_all_reviews();

    // // create a review and add it to the list from terminal
    // Review new;
    // printf("What's your name? ");
    // scanf("%s", new.reviewer);
    // printf("Enter movie name: ");
    // scanf("%s", new.movie);
    // printf("Enter rating: ");
    // scanf("%d", &new.rating);
    // create(list, &new);

    // int wasSaved = save_all_reviews(list);
    // printf("Was saved? %d\n", wasSaved);

    // for (int i = 0; i < list->length; i++) {
    //     Review review = list->items[i];
    //     printf("#%d - %s - %d\n", review.id, review.movie, review.rating);
    // }

    // return 0;
}
