#include "review.h"
#include "review-repository.h"
#include <stdio.h>
#include <stdlib.h>

void list_header() {
    system("clear");
    printf("==============================\n");
    printf("       LISTA DE AVALIAÇÕES\n");
    printf("==============================\n");
}

int main() {
	Table table;
	int option;
	boot_reviews(&table);

	while(1) {
        printf("==============================\n");
        printf("     AVALIAÇÕES DE FILMES\n");
        printf("==============================\n");
        printf("[1] Nova avaliação\n");
        printf("[2] Listar (ordem por ID)\n");
        printf("[3] Listar (ordem por AVALIAÇÃO)\n");
        printf("[4] Listar (ordem por FILME)\n");
        // printf("[3] Buscar avaliação por filme\n");
        // printf("[4] Buscar avaliação por nota\n");
        printf("[99] Sair\n");

        printf("Escolha uma opção: ");
		scanf("%d", &option);
        switch(option) {
            case 1:
                store_review(&table, create_review());
                system("clear");
                printf("Avaliação realizada com sucesso!\n\n");
                break;
            case 2:
                list_header();
                printf("ORDENADO POR ID\n\n");
                print_reviews(&table, table.id_index);
                printf("\n");
                break;
            case 3:
                list_header();
                printf("ORDENADO POR AVALIAÇÃO\n\n");
                print_reviews(&table, table.rating_index);
                printf("\n");
                break;
            case 4:
                list_header();
                printf("ORDENADO POR FILME\n\n");
                print_reviews(&table, table.movie_index);
                printf("\n");
                break;
            case 99:
                shutdown_reviews(&table);
                exit(0);
        }
    }
}
