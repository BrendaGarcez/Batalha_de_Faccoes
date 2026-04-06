#ifndef FACCAO_H
#define FACCAO_H

#include "unidade.h"
#include "edificio.h"

typedef struct faccao {
    char nome[15];
    char id;
    int cor;
    int recurso;
    int poder;
    int is_bot;

    Unidade* unidades;
    Edificio* edificios;

    struct faccao* prox;
} Faccao;

void adicionar_faccao(Faccao** lista, int n, int m, char terreno[n][m], char ocupacao[n][m]);
void liberar_faccoes(Faccao* lista);

void atualizar_visibilidade(Faccao* faccoes, int n, int m, char visivel[n][m]);

#endif