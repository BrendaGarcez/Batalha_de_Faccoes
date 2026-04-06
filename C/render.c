#include <stdio.h>
#include "render.h"

void renderizar_mapa(int n, int m, char terreno[n][m], char ocupacao[n][m], char visivel[n][m]) {

    printf("\n");
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < m; j++) {

            if (visivel[i][j] == 'X') {
                printf(" X ");
            }
            else if (ocupacao[i][j] != ' ') {
                printf(" %c ", ocupacao[i][j]);
            }
            else {
                printf(" %c ", terreno[i][j]);
            }
        }
        printf("\n");
    }
}