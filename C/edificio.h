#ifndef EDIFICIO_H
#define EDIFICIO_H

typedef struct edificio {
    int x, y;
    int tipo;  // 1-R , 2-T , 3-L
    struct edificio* prox;
} Edificio;

void liberar_edificios(Edificio* e);

#end
