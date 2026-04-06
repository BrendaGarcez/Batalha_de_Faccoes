#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mapa.h"
#include "faccao.h"
#include "render.h"
#include "ia.h


int main() {
    srand(time(NULL));

    int n, m;
    printf("Tamanho do mapa (linhas colunas): ");
    scanf("%d %d", &n, &m);

    char terreno[n][m];
    char ocupacao[n][m];
    char visivel[n][m];

    gerar_mapa(n, m, terreno);
    limpar_ocupacao(n, m, ocupacao);
    limpar_visibilidade(n, m, visivel);

    Faccao *faccoes = NULL;
    int qtd;

    printf("Quantas faccoes? ");
    scanf("%d", &qtd);

    for (int i = 0; i < qtd; i++) {
        adicionar_faccao(&faccoes, n, m, terreno, ocupacao);
    }

    int turno = 1;
    while (1) {
        printf("\n===== TURNO %d =====\n", turno);

        atualizar_visibilidade(faccoes, n, m, visivel);
        renderizar_mapa(n, m, terreno, ocupacao, visivel);

        Faccao *f = faccoes;
        while (f != NULL) {
            if (f->is_bot)
                ia_executar(f, n, m, terreno, ocupacao);
            else
                printf("Turno da faccao %c\n", f->id);
                printf("\nOpções:\n");
                printf("1 - Criar unidade\n");
                printf("2 - Pular turno\n");
                int op;
                scanf("%d", &op);

                if (op == 1) {
                    printf("Tipos:\n");
                    printf("1 - Soldado\n");
                    printf("2 - Explorador\n");
                    printf("3 - Engenheiro\n");
                    printf("4 - Mago\n");

                    int tipo;
                    scanf("%d", &tipo);

                    criar_unidade_faccao(f, tipo, n, m, terreno, ocupacao);
                }


            f = f->prox;
        }

        turno++;
        if (turno > 9999) break;
    }

    liberar_faccoes(faccoes);

    return 0;
}
