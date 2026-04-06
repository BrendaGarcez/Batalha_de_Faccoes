#include <stdlib.h>
#include "edificio.h"

void liberar_edificios(Edificio* e) {
    while (e != NULL) {
        Edificio* t = e;
        e = e->prox;
        free(t);
    }
}