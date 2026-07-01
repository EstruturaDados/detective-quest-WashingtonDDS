#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================================
 * ESTRUTURA DA SALA (NÓ DA ÁRVORE)
 * Representa cada cômodo da mansão.
 * ========================================================== */
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ==========================================================
 * Função: criarSala()
 * Propósito: Cria, de forma dinâmica, uma sala com o nome
 * fornecido. Inicializa os ponteiros esquerda e direita como
 * NULL (vazios), indicando que ainda não há caminhos a partir dela.
 * ========================================================== */
Sala* criarSala(char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala)); // Alocação dinâmica
    
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    
    return novaSala;
}

/* ==========================================================
 * Função: explorarSalas()
 * Propósito: Permite a navegação interativa do jogador pela 
 * árvore binária. O jogador escolhe os caminhos até chegar a
 * um nó-folha (cômodo sem saída) ou até decidir sair.
 * ========================================================== */
void explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    char escolha;

    while (atual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Você está em: %s\n", atual->nome);

        // Verifica se é um nó-folha (sem filhos à esquerda ou direita)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você explorou até o fim e não há mais saídas neste cômodo.\n");
            printf("Fim da exploração!\n");
            break; // Encerra a navegação
        }

        // Menu de opções para o jogador
        printf("Caminhos disponíveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a porta à esquerda\n");
        if (atual->direita != NULL) printf(" [d] Ir para a porta à direita\n");
        printf(" [s] Sair da mansão\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        // Processa a escolha do jogador utilizando operadores condicionais
        if (escolha == 's' || escolha == 'S') {
            printf("Você decidiu sair da mansão. Até a próxima detetive!\n");
            break;
            
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda; // Avança para a sala da esquerda
            
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita; // Avança para a sala da direita
            
        } else {
            printf("\nOpção inválida ou caminho inexistente! Tente novamente.\n");
        }
    }
}

/* ==========================================================
 * Função: main()
 * Propósito: Monta o mapa inicial da mansão ligando os nós,
 * dá início à exploração chamando explorarSalas() e, ao
 * final, libera a memória alocada.
 * ========================================================== */
int main() {
    printf("========================================\n");
    printf("      BEM-VINDO AO DETECTIVE QUEST      \n");
    printf("========================================\n");

    // 1. Criação de todas as salas dinamicamente
    Sala *hall = criarSala("Hall de entrada");
    
    Sala *corredorEsq = criarSala("Corredor Esquerdo");
    Sala *corredorDir = criarSala("Corredor Direito");
    
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    
    Sala *jardim = criarSala("Jardim de Inverno");
    Sala *escritorio = criarSala("Escritório do Barão");

    // 2. Montagem da Árvore Binária (Conectando os cômodos)
    // Nível 0 para Nível 1
    hall->esquerda = corredorEsq;
    hall->direita = corredorDir;

    // Nível 1 para Nível 2 (Lado Esquerdo)
    corredorEsq->esquerda = biblioteca;
    corredorEsq->direita = cozinha;

    // Nível 1 para Nível 2 (Lado Direito)
    corredorDir->esquerda = jardim;
    corredorDir->direita = escritorio;

    // 3. Início da exploração a partir da raiz (Hall)
    explorarSalas(hall);

    // 4. Boas práticas: Liberação da memória alocada dinamicamente
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(escritorio);
    free(corredorEsq);
    free(corredorDir);
    free(hall);

    return 0;
}