#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================================
 * ESTRUTURA DA BST DE PISTAS
 * Representa os nós da árvore de busca que armazena as pistas.
 * ========================================================== */
typedef struct PistaNode {
    char descricao[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* ==========================================================
 * ESTRUTURA DA SALA (NÓ DA ÁRVORE DO MAPA)
 * Modificada para incluir a pista associada ao cômodo.
 * ========================================================== */
typedef struct Sala {
    char nome[50];
    char pista[100]; // Pista contida nesta sala (vazia se não houver)
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/* ==========================================================
 * Função: criarSala()
 * Propósito: Cria dinamicamente um cômodo com nome e pista.
 * Inicializa os caminhos (esquerda e direita) como NULL.
 * ========================================================== */
Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        if (pista != NULL) {
            strcpy(novaSala->pista, pista);
        } else {
            strcpy(novaSala->pista, ""); // Define como string vazia se não houver pista
        }
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    
    return novaSala;
}

/* ==========================================================
 * Função: inserirPista()
 * Propósito: Insere recursivamente uma nova pista na árvore 
 * BST de pistas, garantindo a ordenação alfabética via strcmp.
 * ========================================================== */
PistaNode* inserirPista(PistaNode *raiz, char *novaPista) {
    // Se chegamos em um ponteiro nulo, encontramos o local de inserção
    if (raiz == NULL) {
        PistaNode *novoNo = (PistaNode*) malloc(sizeof(PistaNode));
        if (novoNo != NULL) {
            strcpy(novoNo->descricao, novaPista);
            novoNo->esquerda = NULL;
            novoNo->direita = NULL;
        }
        return novoNo;
    }

    // Compara a nova pista com a pista do nó atual para decidir a ramificação
    int comparacao = strcmp(novaPista, raiz->descricao);

    if (comparacao < 0) {
        // Se for alfabeticamente menor, vai para a esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        // Se for alfabeticamente maior, vai para a direita
        raiz->direita = inserirPista(raiz->direita, novaPista);
    }
    // Se comparacao == 0, a pista já existe na árvore (evita duplicatas)

    return raiz;
}

/* ==========================================================
 * Função: exibirPistas()
 * Propósito: Imprime a árvore BST de pistas em ordem alfabética
 * utilizando o percurso em-ordem (Esquerda -> Raiz -> Direita).
 * ========================================================== */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->descricao);
        exibirPistas(raiz->direita);
    }
}

/* ==========================================================
 * Função: liberarPistas()
 * Propósito: Libera recursivamente a memória alocada para a BST de pistas.
 * ========================================================== */
void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

/* ==========================================================
 * Função: explorarSalasComPistas()
 * Propósito: Controla a navegação interativa entre as salas 
 * e gerencia a coleta automática de pistas na BST.
 * ========================================================== */
PistaNode* explorarSalasComPistas(Sala *raiz) {
    Sala *atual = raiz;
    PistaNode *bstPistas = NULL; // Ponteiro para a raiz da árvore de pistas
    char escolha;

    while (atual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Você está em: %s\n", atual->nome);

        // Coleta automática se o cômodo atual contiver uma pista válida
        if (strlen(atual->pista) > 0) {
            printf("[🔎 PISTA ENCONTRADA]: %s\n", atual->pista);
            bstPistas = inserirPista(bstPistas, atual->pista);
        } else {
            printf("Não há pistas visíveis neste local.\n");
        }

        // Exibição dos caminhos disponíveis de forma dinâmica
        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a porta à esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf(" [d] Ir para a porta à direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair da mansão e analisar o caso\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        // Processamento da escolha do jogador
        if (escolha == 's' || escolha == 'S') {
            printf("\nVocê decidiu sair da mansão para organizar suas anotações.\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda; // Avança para o filho da esquerda
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;  // Avança para o filho da direita
        } else {
            printf("\nOpção inválida ou caminho inexistente! Tente novamente.\n");
        }
    }

    return bstPistas; // Retorna a árvore contendo todas as pistas coletadas
}

/* ==========================================================
 * Função: main()
 * Propósito: Configura o cenário inicial conectando os cômodos,
 * inicializa o jogo e faz o gerenciamento de memória final.
 * ========================================================== */
int main() {
    // Configuração de codificação padrão para saídas de console limpas
    printf("========================================\n");
    printf("     BEM-VINDO AO DETECTIVE QUEST      \n");
    printf("========================================\n");

    // 1. Criação das salas com o mapa e suas respectivas pistas
    Sala *hall = criarSala("Hall de entrada", "Bilhete antigo rasgado");
    
    Sala *corredorEsq = criarSala("Corredor Esquerdo", NULL); // Sala sem pista
    Sala *corredorDir = criarSala("Corredor Direito", "Pegada de lama perto da janela");
    
    Sala *biblioteca = criarSala("Biblioteca", "Diário secreto com cadeado quebrado");
    Sala *cozinha = criarSala("Cozinha", "Xícara de chá ainda morna");
    
    Sala *jardim = criarSala("Jardim de Inverno", "Chave dourada caída no vaso");
    Sala *escritorio = criarSala("Escritório do Barão", "Testamento alterado recentemente");

    // 2. Montagem da Árvore Binária de Navegação (Conexões)
    hall->esquerda = corredorEsq;
    hall->direita = corredorDir;

    corredorEsq->esquerda = biblioteca;
    corredorEsq->direita = cozinha;

    corredorDir->esquerda = jardim;
    corredorDir->direita = escritorio;

    // 3. Execução da rotina de exploração do cenário
    PistaNode *pistasColetadas = explorarSalasComPistas(hall);

    // 4. Relatório final: Exibição das pistas em ordem alfabética
    printf("\n========================================\n");
    printf("       RELATÓRIO FINAL DO DETETIVE      \n");
    printf("========================================\n");
    if (pistasColetadas == NULL) {
        printf("Você encerrou a investigação sem coletar pistas.\n");
    } else {
        printf("Pistas coletadas estruturadas alfabeticamente:\n");
        exibirPistas(pistasColetadas);
    }
    printf("========================================\n");

    // 5. Liberação correta de toda a memória dinâmica do Heap
    liberarPistas(pistasColetadas);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(escritorio);
    free(corredorEsq);
    free(corredorDir);
    free(hall);

    return 0;
}