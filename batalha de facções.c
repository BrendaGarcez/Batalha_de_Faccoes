#include <windows.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Códigos de cores para exibição
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"


// Função para definir cor (Windows)
void definir_cor(int cor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cor);
}

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
    int cor;
    int pontos_recurso;
    int pontos_poder;
    int x, y; 
    struct faccao *prox;
    Tunidade *proxunidade;
    Tedificio *proxedificio;
    Taliancas *proxalianca;
} Tfaccao;

// Funções de mapa e memória
void distribuir_aleatoriamente(char **area, int n, int m);
void desalocar(char ***area, int n, int m);

// Funções de facção
void adicionar_faccao(Tfaccao **lista_faccoes, char **area, int n, int m);
void exibir_faccoes_coloridas(Tfaccao *lista_faccoes);
void mover_faccao(Tfaccao *faccao, char **area, int n, int m, int direcao);
void liberar_faccoes(Tfaccao *faccoes);
void adicionar_edificio(Tfaccao *faccao, char **area, int n, int m);

// Função auxiliar
int contar_letra_usada(Tfaccao *lista, char letra);

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

    system("cls || clear"); 
    exibir_faccoes_coloridas(faccoes);

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
    //Encerramento
    liberar_faccoes(faccoes);
    desalocar(&area, n, m);
    if(area != NULL){
        printf("Desalocar deu errado!");
    } else {
        printf("\nMemória liberada com sucesso. Até logo!\n");
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
    while (qtd_p > 0 || qtd_f > 0 || qtd_m > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] != ' ') continue;

        int tipo = rand() % 100;
        if (tipo < 50 && qtd_p > 0) {
            area[i][j] = 'P';
            qtd_p--;
        } else if (tipo < 80 && qtd_f > 0) {
            area[i][j] = 'F';
            qtd_f--;
        } else if (qtd_m > 0) {
            area[i][j] = 'M';
            qtd_m--;
        }
    }
}

// Verifica se a letra já está em uso e conta quantas vezes foi usada
int contar_letra_usada(Tfaccao *lista, char letra) {
    int count = 0;
    while (lista != NULL) {
        if (lista->letra == letra) {
            count++;
        }
        lista = lista->prox;
    }
    return count;
}

void adicionar_faccao(Tfaccao **lista_faccoes, char **area, int n, int m){
    Tfaccao *nova = (Tfaccao *)malloc(sizeof(Tfaccao));
    if (!nova) return;

    printf("Digite o nome da faccao: ");
    scanf("%s", nova->nome);

    nova->letra = nova->nome[0];
    int rep = contar_letra_usada(*lista_faccoes, nova->letra);
    nova->cor = 9 + rep % 6; // Varia de 9 a 14 (cores diferentes)

    int x, y;
    do {
        x = rand() % n;
        y = rand() % m;
    } while (area[x][y] != 'P');

    nova->x = x;
    nova->y = y;
    nova->pontos_recurso = 24;
    nova->pontos_poder = 0;
    nova->proxunidade = NULL;
    nova->proxedificio = NULL;
    nova->proxalianca = NULL;
    nova->prox = NULL;

    area[x][y] = nova->letra;
    
    // Inserir na lista encadeada
    if (*lista_faccoes == NULL) {
        *lista_faccoes = nova;
    } else {
        Tfaccao *temp = *lista_faccoes;
        while (temp->prox != NULL) temp = temp->prox;
        temp->prox = nova;
    }
}
void exibir_faccoes_coloridas(Tfaccao *lista_faccoes) {
    Tfaccao *atual = lista_faccoes;
    int index = 0;
    while(atual != NULL ){
        if (atual == NULL) {
            printf("Nenhuma faccao na lista.\n");
            return;
        }
        int cor = 9 + index % 6;
        definir_cor(cor);
        printf("Letra: [%c], Nome: %s, Pontos de Recurso: %d, Pontos de Poder: %d\n", atual->letra,
               atual->nome, atual->pontos_recurso, atual->pontos_poder);
        definir_cor(7);
        atual = atual->prox;
        index++;
    }
}

void adicionar_edificio(Tfaccao *faccao, char **area, int n, int m) {
    if (faccao == NULL) return;

    printf("\nDeseja construir algo na nova posição (%d, %d)?\n", f->x, f->y);
    printf("1 - Edificio de Recursos (R)\n");
    printf("2 - Campo de Treinamento (T)\n");
    printf("3 - Laboratorio de Pesquisa (L)\n");
    printf("0 - Nada\n");
    int tipo;
    scanf("%d", &tipo);

    if (tipo < 1 || tipo > 3) {
        printf("Nenhuma construção feita.\n");
        return;
    }

    Tedificio *novo = (Tedificio *)malloc(sizeof(Tedificio));
    novo->x = faccao->x;
    novo->y = faccao->y;
    novo->tipo = tipo;
    novo->prox = faccao->proxedificio;
    faccao->proxedificio = novo;

    char letra;
    switch (tipo) {
        case 1: letra = 'R'; break;
        case 2: letra = 'T'; break;
        case 3: letra = 'L'; break;
    }
    
    // Se quiser, você pode alterar o mapa para marcar com um símbolo de edifício, tipo 'E'
    area[novo->x][novo->y] = letra;
    printf("Construção concluída! Edifício [%c]\n", letra);
}


/// Movimento de facção
void mover_faccao(Tfaccao *faccao, char **area, int n, int m, int direcao) {
    if (!f) return;
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

    if (novo_x < 0 || novo_x >= n || novo_y < 0 || novo_y >= m) {
        printf("Fora dos limites!\n");
        return;
    }

    char destino = area[novo_x][novo_y];

    if (destino == 'P' || destino == 'F' || destino == 'M') {
        // Atualiza pontos
        if (destino == 'P') {
            f->pontos_recurso += 5;
            f->pontos_poder += 1;
        } else if (destino == 'F') {
            f->pontos_recurso += 15;
            f->pontos_poder += 15;
        }

        // Atualiza mapa
        area[f->x][f->y] = 'P'; // Deixa a antiga como planície
        f->x = novo_x;
        f->y = novo_y;
        area[novo_x][novo_y] = f->letra;

        printf("Movimentacao realizada!\n");

        // Após movimento, permitir construção
        adicionar_edificio(f, area, n, m);
    } else {
        printf("Terreno ocupado! Movimento cancelado.\n");
    }
}
void liberar_faccoes(Tfaccao *faccoes) {
    while (faccoes != NULL) {
        Tfaccao *temp = faccoes;
        faccoes = faccoes->prox;

        // Libera unidades
        while (temp->proxunidade != NULL) {
            Tunidade *u = temp->proxunidade;
            temp->proxunidade = u->prox;
            free(u);
        }

        // Libera edificios
        while (temp->proxedificio != NULL) {
            Tedificio *e = temp->proxedificio;
            temp->proxedificio = e->prox;
            free(e);
        }

        // Libera aliancas
        while (temp->proxalianca != NULL) {
            Taliancas *a = temp->proxalianca;
            temp->proxalianca = a->prox;
            free(a);
        }

        free(temp);
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
