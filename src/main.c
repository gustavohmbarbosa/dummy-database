#include "bst.h"
#include "constants.h"
#include "review.h"
#include "review-repository.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void list_header() {
    system("clear");
    printf("==============================\n");
    printf("       LISTA DE AVALIAÇÕES\n");
    printf("==============================\n");
}

void search_header() {
    system("clear");
    printf("==============================\n");
    printf("       BUSCAR AVALIAÇÃO       \n");
    printf("==============================\n");
}

void search_by_id(Table *table) {
    Index temp_index;
    search_header();
    temp_index.key_type = KEY_TYPE_INT;
    temp_index.key = malloc(sizeof(int));
    printf("Informe o ID da avaliação: ");
    scanf("%d", (int *) temp_index.key);

    bst_search_offset(table->id_index, &temp_index);
    if (temp_index.offset == -1) {
        printf("\nAvaliação não encontrada\n");
    } else {
        printf("\n");
        print_review(table, temp_index.offset);
    }

    free(temp_index.key); // TODO: is this necessary?
}

void search_by_rating(Table *table) {
    Index temp_index;
    search_header();
    temp_index.key_type = KEY_TYPE_INT;
    temp_index.key = malloc(sizeof(int));
    printf("Informe a nota da avaliação: ");
    scanf("%d", (int *) temp_index.key);

    bst_search_offset(table->rating_index, &temp_index);
    if (temp_index.offset == -1) {
        printf("\nAvaliação não encontrada\n");
    } else {
        printf("\n");
        print_review(table, temp_index.offset);
    }

    free(temp_index.key);
}

void search_by_movie(Table *table) {
    search_header();
    Index temp_index;
    temp_index.key_type = KEY_TYPE_STRING;
    getchar();
    printf("Informe o nome do filme: ");
    size_t len = 0;
    getline((char **) &temp_index.key, &len, stdin);
    ((char *) temp_index.key)[strcspn(temp_index.key, "\n")] = '\0';  // Remove newline if present

    bst_search_offset(table->movie_index, &temp_index);
    if (temp_index.offset == -1) {
        printf("\nAvaliação não encontrada\n");
    } else {
        printf("\n");
        print_review(table, temp_index.offset);
    }

    free(temp_index.key); // TODO: is this necessary?
}

int main() {
	Table table;
	int option;
	boot_reviews(&table);

	while(1) {
        printf("==============================\n");
        printf("     AVALIAÇÕES DE FILMES     \n");
        printf("==============================\n");
        printf("[1] Nova avaliação\n");
        printf("[2] Listar (ordem por ID)\n");
        printf("[3] Listar (ordem por AVALIAÇÃO)\n");
        printf("[4] Listar (ordem por FILME)\n");
        printf("[5] Buscar por ID\n");
        printf("[6] Buscar por AVALIAÇÃO\n");
        printf("[7] Buscar por FILME\n");
        // printf("[5] Excluir por ID\n");
        // printf("[6] Excluir por filme\n");
        // printf("[7] Excluir por nota\n");
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
            case 5:
                search_by_id(&table);
                break;
            case 6:
                search_by_rating(&table);
                break;
            case 7:
                search_by_movie(&table);
                break;
            case 8:
                system("clear");
                printf("============================\n");
                printf("       EXCLUIR POR ID       \n");
                printf("============================\n");
                Index temp_index;
                temp_index.key_type = KEY_TYPE_INT;
                temp_index.key = malloc(sizeof(int));
                printf("Informe o ID da avaliação: ");
                scanf("%d", (int *) temp_index.key);
                bst_remove(&(table.id_index), &temp_index);
                printf("\n");
                break;
            case 99:
                shutdown_reviews(&table);
                exit(0);
        }
    }
}
