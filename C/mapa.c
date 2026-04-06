#include <stdlib.h>
#include "mapa.h"

void gerar_mapa(int n, int m, char terreno[n][m]) {
    int total = n * m;
    int qtd_f = total * 0.40;
    int qtd_p = total * 0.40;
    int qtd_m = total - qtd_f - qtd_p;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            terreno[i][j] = ' ';

    while (qtd_f > 0 || qtd_p > 0 || qtd_m > 0) {
        int i = rand() % n;
        int j = rand() % m;

        if (terreno[i][j] != ' ') continue;

        int r = rand() % 100;
        if (r < 40 && qtd_f > 0) { terreno[i][j] = 'F'; qtd_f--; }
        else if (r < 80 && qtd_p > 0) { terreno[i][j] = 'P'; qtd_p--; }
        else if (qtd_m > 0) { terreno[i][j] = 'M'; qtd_m--; }
    }
}

void limpar_ocupacao(int n, int m, char ocupacao[n][m]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            ocupacao[i][j] = ' ';
}

void limpar_visibilidade(int n, int m, char visivel[n][m]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            visivel[i][j] = 'X';
}