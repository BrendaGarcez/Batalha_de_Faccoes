#include <stdlib.h>
#include "unidade.h"

void liberar_unidades(Unidade* u) {
    while (u != NULL) {
        Unidade* t = u;
        u = u->prox;
        free(t);
    }
}