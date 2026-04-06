#ifndef UNIDADE_H
#define UNIDADE_H

#define SOLDADO     1
#define EXPLORADOR  2
#define ENGENHEIRO  3
#define MAGO        4

typedef struct unidade {
    int x, y;
    int tipo;
    int movimento;
    int ataque;
    struct unidade* prox;
} Unidade;

Unidade* criar_unidade(int tipo, int x, int y);
void liberar_unidades(Unidade* u);

#endif