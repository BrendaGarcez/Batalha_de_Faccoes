#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estruturas de representa��o
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
    struct faccao *prox;
    Tunidade *proxunidade;
    Tedificio *proxedificio;
    Taliancas *proxalianca;
} Tfaccao;

/// Fun��es
void distribuir_aleatoriamente(char **area, int n, int m);
void exibir_faccoes(Tfaccao *lista_faccoes);
void adicionar_faccao(Tfaccao **lista_faccoes);
void desalocar(char ***area, int n, int m);

int main(){
    int n = 0, m = 0, i = 0, j = 0;
    Tfaccao *faccoes = NULL;
    char **area;

    //// Criar a matriz
    printf("\nInforme o tamanho da Matriz(colunas X linhas):\n");
    scanf("%d %d", &n, &m);

    area = (char **)malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        area[i] = (char *)malloc(m * sizeof(char));
    }

    distribuir_aleatoriamente(area, n, m);

    ////Criar Fac��es
    int t = 0;
    printf("Quantas faccoes deseja criar?\n");
    scanf("%d", &t);
    for(int i = 0; i < t; i++){
        adicionar_faccao(&faccoes);
    }

    system("cls");
    exibir_faccoes(faccoes);

    printf("\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            printf(" %c", area[i][j]);
        }
        printf("\n");
    }


    ////L�gica de movimenta��o
    int op = 0, op1 = 0;
    while (op != 5) {  // Continua at� que o usu�rio escolha "Exit" (5)
        printf("O que deseja fazer agora?\n");
        printf("1 - Avancar Terreno\n");
        printf("5 - Exit\n");  // Op��o para sair
        scanf("%d", &op);
        switch(op) {
            case 1:
                printf("Em qual direcao?\n");
                printf("1 - Cima\n2 - Baixo\n3 - Esquerda\n4 - Direita\n5 - Exit\n");
                scanf("%d", &op1);

                if (op1 == 5) {
                    printf("Saindo do movimento...\n");
                } else {
                    printf("Movendo na direcao %d\n", op1);
                    ////chama fun��o de movimenta��o
                }
                break;

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
/////mapa com  disposi��o de tipos, floresta, montanha e plan�ces
void distribuir_aleatoriamente(char **area, int n, int m) {
    int i, j;
    int total = n * m;
    int qtd_p = total * 0.50;  // 50% de 'p'
    int qtd_f = total * 0.30;  // 30% de 'f'
    int qtd_m = total - qtd_p - qtd_f;  // 20% de 'm'

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            area[i][j] = ' ';  // Inicializa com espa�os em branco
        }
    }
    srand(time(NULL));  // Inicializa a semente do gerador de n�meros aleat�rios
    // Preenche a matriz com 'p'
    while (qtd_p > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {  // Verifica se o espa�o est� vazio
            area[i][j] = 'P';
            qtd_p--;
        }
    }
    // Preenche a matriz com 'f'
    while (qtd_f > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {
            area[i][j] = 'F';
            qtd_f--;
        }
    }
    // Preenche a matriz com 'm'
    while (qtd_m > 0) {
        i = rand() % n;
        j = rand() % m;
        if (area[i][j] == ' ') {
            area[i][j] = 'M';
            qtd_m--;
        }
    }
}

void adicionar_faccao(Tfaccao **lista_faccoes) {
    Tfaccao *nova_faccao = (Tfaccao *)malloc(sizeof(Tfaccao));
    Tfaccao *ultimo = *lista_faccoes;

    if (nova_faccao == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    // Solicitar informa��es da nova fac��o
    printf("Digite o nome da faccao: ");
    scanf("%s", nova_faccao->nome); // Limitar a 14 caracteres para deixar espa�o para o terminador nulo
    nova_faccao->letra = nova_faccao->nome[0];
    nova_faccao->pontos_recurso = 24; // Inicializar com valor padr�o
    nova_faccao->pontos_poder = 0;   // Inicializar com valor padr�o
    nova_faccao->prox = NULL;        // Inicialmente, o pr�ximo � NULL

    // Adicionar a nova fac��o ao final da lista
    if (*lista_faccoes == NULL) {
        // Se a lista estiver vazia, a nova fac��o se torna a cabe�a da lista
        *lista_faccoes = nova_faccao;
    } else {
        // Caso contr�rio, percorra a lista at� o final e adicione a nova fac��o
        while (ultimo->prox != NULL) {
            ultimo = ultimo->prox;
        }
        ultimo->prox = nova_faccao;
    }
}

// Fun��o para exibir a lista de fac��es
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

void desalocar(char ***area, int n, int m){
    int i = 0;
    for(i = 0; i < n; i++){
        free((*area)[i]);  // Libera cada linha da matriz
    }
    free(*area);  // Libera o ponteiro principal da matriz
    *area = NULL;  // Define o ponteiro principal como NULL
}
