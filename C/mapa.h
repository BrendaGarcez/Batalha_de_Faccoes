#ifndef MAPA_H
#define MAPA_H

void gerar_mapa(int n, int m, char terreno[n][m]);
void limpar_ocupacao(int n, int m, char ocupacao[n][m]);
void limpar_visibilidade(int n, int m, char visivel[n][m]);

#endif