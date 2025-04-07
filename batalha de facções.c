#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"

// Estruturas de representação
typedef struct unidade {
    int x;
    int y;
    int tipo; // 1-soldado 2-explorador
    struct unidade *prox;
} Tunidade;

typedef struct edificio {
    int x;
    int y;
    int tipo; // 1- Edificio de Recursos 2- Campo de Treinamento 3- Laboratorio de Pesquisa
    struct edificio *prox;
} Tedificio;

typedef struct aliancas {
    char nome[15];
    struct aliancas *prox;
} Taliancas;

typedef struct faccao {
    char letra;
    char nome[15];
    int pontos_recurso;
    int pontos_poder;
    int x, y; 
    struct faccao *prox;
    Tunidade *proxunidade;
    Tedificio *proxedificio;
    Taliancas *proxalianca;
} Tfaccao;

/// Funções
void distribuir_aleatoriamente(char **area, int n, int m);
void exibir_faccoes(Tfaccao *lista_faccoes);
void adicionar_faccao(Tfaccao **lista_faccoes, char **area, int n, int m);
void desalocar(char ***area, int n, int m);
void mover_faccao(Tfaccao *faccao, char **area, int n, int m, int direcao);

int main(){
    int n = 0, m = 0, i = 0, j = 0;
    Tfaccao *faccoes = NULL;
    char **area;

    //// Criar a matriz
    printf("\nInforme o tamanho da Matriz (colunas X linhas):\n");
    scanf("%d %d", &n, &m);

    area = (char **)malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        area[i] = (char *)malloc(m * sizeof(char));
    }

    distribuir_aleatoriamente(area, n, m);

    //// Criar Facções
    int t = 0;
    printf("Quantas faccoes deseja criar?\n");
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        adicionar_faccao(&faccoes, area, n, m);
    }

    system("cls || clear");  // cross-platform clear
    exibir_faccoes(faccoes);

    //// Mostrar mapa
    printf("\nMapa Inicial:\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf(" %c", area[i][j]);
        }
        printf("\n");
    }

    //// Lógica de movimentação
    int op = 0, op1 = 0;
    while (op != 5) {
        printf("\nO que deseja fazer agora?\n");
        printf("1 - Avancar Terreno\n");
        printf("5 - Exit\n");
        scanf("%d", &op);

        switch(op) {
            case 1: {
                Tfaccao *f = faccoes;
                printf("Escolha a letra da faccao para mover:\n");
                char letra_escolhida;
                scanf(" %c", &letra_escolhida);

                // Procurar facção pela letra
                while (f && f->letra != letra_escolhida) {
                    f = f->prox;
                }

                if (!f) {
                    printf("Faccao nao encontrada.\n");
                    break;
                }

                printf("Em qual direcao?\n");
                printf("1 - Cima\n2 - Baixo\n3 - Esquerda\n4 - Direita\n5 - Cancelar\n");
                scanf("%d", &op1);

                if (op1 == 5) {
                    printf("Cancelando movimentacao...\n");
                } else {
                    mover_faccao(f, area, n, m, op1);

                    // Mostrar mapa atualizado
                    printf("\nMapa Atualizado:\n");
                    for(i = 0; i < n; i++){
                        for(j = 0; j < m; j++){
                            printf(" %c", area[i][j]);
                        }
                        printf("\n");
                    }
                }
                break;
            }

            case 5:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    }

    //// Desalocar matriz
    desalocar(&area, n, m);
    if(area != NULL){
        printf("Desalocar deu errado!");
    }
    return 0;
}

/// Mapa com disposição aleatória de tipos de terreno
void distribuir_aleatoriamente(char **area, int n, int m) {
    int i, j;
    int total = n * m;
    int qtd_p = total * 0.50;  // 50% planícies
    int qtd_f = total * 0.30;  // 30% florestas
    int qtd_m = total - qtd_p - qtd_f;  // 20% montanhas

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            area[i][j] = ' ';
        }
    }

    srand(time(NULL));
    while (qtd_p > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {
            area[i][j] = 'P';
            qtd_p--;
        }
    }

    while (qtd_f > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {
            area[i][j] = 'F';
            qtd_f--;
        }
    }

    while (qtd_m > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {
            area[i][j] = 'M';
            qtd_m--;
        }
    }
}

/// Adicionar nova facção
void adicionar_faccao(Tfaccao **lista_faccoes, char **area, int n, int m){
    Tfaccao *nova_faccao = (Tfaccao *)malloc(sizeof(Tfaccao));
    Tfaccao *ultimo = *lista_faccoes;
    if (nova_faccao == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    printf("Digite o nome da faccao: ");
    scanf("%s", nova_faccao->nome);

    // Posicionamento aleatório em planície
    int x, y;
    do {
        x = rand() % n;
        y = rand() % m;
    } while (area[x][y] != 'P');

    nova_faccao->x = x;
    nova_faccao->y = y;
    nova_faccao->letra = nova_faccao->nome[0];
    area[x][y] = nova_faccao->letra;
    nova_faccao->pontos_recurso = 24;
    nova_faccao->pontos_poder = 0;
    nova_faccao->prox = NULL;
    nova_faccao->proxunidade = NULL;
    nova_faccao->proxedificio = NULL;
    nova_faccao->proxalianca = NULL;

    if (*lista_faccoes == NULL) {
        *lista_faccoes = nova_faccao;
    } else {
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = nova_faccao;
    }
}

/// Exibir facções
void exibir_faccoes(Tfaccao *lista_faccoes) {
    Tfaccao *atual = lista_faccoes;

    if (atual == NULL) {
        printf("Nenhuma faccao na lista.\n");
        return;
    }

    while (atual != NULL) {
        printf("Letra: [%c], Nome: %s, Pontos de Recurso: %d, Pontos de Poder: %d\n", atual->letra,
               atual->nome, atual->pontos_recurso, atual->pontos_poder);
        atual = atual->prox;
    }
}

/// Libera memória da matriz
void desalocar(char ***area, int n, int m){
    for(int i = 0; i < n; i++){
        free((*area)[i]);
    }
    free(*area);
    *area = NULL;
}

/// Movimento de facção
void mover_faccao(Tfaccao *faccao, char **area, int n, int m, int direcao) {
    int novo_x = faccao->x;
    int novo_y = faccao->y;

    switch(direcao) {
        case 1: novo_x--; break; // Cima
        case 2: novo_x++; break; // Baixo
        case 3: novo_y--; break; // Esquerda
        case 4: novo_y++; break; // Direita
        default: 
            printf("Direcao Invalida. \n");
            return;
    }

    if(novo_x >= 0 && novo_x < n && novo_y >= 0 && novo_y < m) {
        char terreno = area[novo_x][novo_y];
        if (terreno == 'P' || terreno == 'F' || terreno == 'M') {
             if (terreno == 'P') {
                faccao->pontos_recurso += 5;
                faccao->pontos_poder += 1;
            } else if (terreno == 'F') {
                faccao->pontos_recurso += 15;
                faccao->pontos_poder += 15;
            } else if (terreno == 'M') {
                // Montanha: sem pontos, mas permite movimento
            }
            area[faccao->x][faccao->y] = 'P';  // Supomos que facções sempre andam sobre planície antes
            // Atualiza posição
            faccao->x = novo_x;
            faccao->y = novo_y;
            area[novo_x][novo_y] = faccao->letra;

        } else {
            printf("Terreno ocupado por outra faccao.\n");
        }
    } else {
        printf("Fora dos limites do mapa.\n");
    }
}
