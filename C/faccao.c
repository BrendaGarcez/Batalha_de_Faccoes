#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "faccao.h"

void adicionar_faccao(Faccao** lista, int n, int m, char terreno[n][m], char ocupacao[n][m]) {
    Faccao* nova = malloc(sizeof(Faccao));

    printf("Nome da faccao: ");
    scanf("%s", nova->nome);

    nova->id = nova->nome[0];
    nova->recurso = 20;
    nova->poder = 5;

    nova->is_bot = 0; // depois você permite escolher

    nova->unidades = NULL;
    nova->edificios = NULL;

    int x, y;
    do {
        x = rand() % n;
        y = rand() % m;
    } while (ocupacao[x][y] != ' ' || terreno[x][y] == 'M');

    ocupacao[x][y] = nova->id;

    nova->prox = *lista;
    *lista = nova;
}

void liberar_faccoes(Faccao* lista) {
    while (lista != NULL) {
        Faccao* t = lista;
        lista = lista->prox;

        liberar_unidades(t->unidades);
        liberar_edificios(t->edificios);

        free(t);
    }
}

void atualizar_visibilidade(Faccao* fac, int n, int m, char visivel[n][m]) {
    while (fac != NULL) {
        Unidade* u = fac->unidades;
        while (u != NULL) {
            int x = u->x, y = u->y;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    if (x+i >= 0 && x+i < n && y+j >= 0 && y+j < m)
                        visivel[x+i][y+j] = ' ';

            u = u->prox;
        }
        fac = fac->prox;
    }
}