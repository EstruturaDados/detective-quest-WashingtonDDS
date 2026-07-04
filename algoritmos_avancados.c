#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 11

/* ==========================================================
 * ESTRUTURAS DE DADOS
 * ========================================================== */

// Nó da BST de Pistas Coletadas
typedef struct PistaNode {
    char descricao[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Nó da Árvore Binária do Mapa da Mansão
typedef struct Sala {
    char nome[50];
    char pista[100]; 
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Nó para encadeamento na Tabela Hash (Associação Pista -> Suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// Estrutura da Tabela Hash
typedef struct {
    HashNode *tabela[TAMANHO_HASH];
} TabelaHash;

/* ==========================================================
 * FUNÇÕES DA TABELA HASH
 * ========================================================== */

// Função Hash simples (método do polinômio/DJB2 adaptado) para strings
unsigned int calcularHash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % TAMANHO_HASH;
}

/* ----------------------------------------------------------
 * Função: inserirNaHash()
 * Propósito: Insere uma associação pista/suspeito na tabela hash.
 * ---------------------------------------------------------- */
void inserirNaHash(TabelaHash *th, char *pista, char *suspeito) {
    unsigned int indice = calcularHash(pista);
    
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (novo != NULL) {
        strcpy(novo->pista, pista);
        strcpy(novo->suspeito, suspeito);
        // Insere no início da lista encadeada daquela posição (posição 'indice')
        novo->proximo = th->tabela[indice];
        th->tabela[indice] = novo;
    }
}

/* ----------------------------------------------------------
 * Função: encontrarSuspeito()
 * Propósito: Consulta a tabela hash e retorna o suspeito associado a uma pista.
 * ---------------------------------------------------------- */
char* encontrarSuspeito(TabelaHash *th, char *pista) {
    unsigned int indice = calcularHash(pista);
    HashNode *atual = th->tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL; // Caso não encontre (pista inválida ou não mapeada)
}

void liberarHash(TabelaHash *th) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode *atual = th->tabela[i];
        while (atual != NULL) {
            HashNode *aux = atual;
            atual = atual->proximo;
            free(aux);
        }
    }
}

/* ==========================================================
 * FUNÇÕES DA BST (ÁRVORE DE PISTAS)
 * ========================================================== */

/* ----------------------------------------------------------
 * Função: inserirPista()
 * Propósito: Insere uma pista coletada na árvore BST de forma ordenada.
 * ---------------------------------------------------------- */
PistaNode* inserirPista(PistaNode *raiz, char *novaPista) {
    if (raiz == NULL) {
        PistaNode *novoNo = (PistaNode*) malloc(sizeof(PistaNode));
        if (novoNo != NULL) {
            strcpy(novoNo->descricao, novaPista);
            novoNo->esquerda = NULL;
            novoNo->direita = NULL;
        }
        return novoNo;
    }

    int comparacao = strcmp(novaPista, raiz->descricao);
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, novaPista);
    }
    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->descricao);
        exibirPistas(raiz->direita);
    }
}

void liberarPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

/* ==========================================================
 * FUNÇÕES DO MAPA DA MANSÃO (ÁRVORE BINÁRIA)
 * ========================================================== */

/* ----------------------------------------------------------
 * Função: criarSala()
 * Propósito: Cria dinamicamente um cômodo com nome e pista.
 * ---------------------------------------------------------- */
Sala* criarSala(char *nome, char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));
    if (novaSala != NULL) {
        strcpy(novaSala->nome, nome);
        if (pista != NULL) {
            strcpy(novaSala->pista, pista);
        } else {
            strcpy(novaSala->pista, "");
        }
        novaSala->esquerda = NULL;
        novaSala->direita = NULL;
    }
    return novaSala;
}

/* ----------------------------------------------------------
 * Função: explorarSalas()
 * Propósito: Navega pela árvore, exibe a pista e a insere na BST.
 * ---------------------------------------------------------- */
PistaNode* explorarSalas(Sala *raiz) {
    Sala *atual = raiz;
    PistaNode *bstPistas = NULL;
    char escolha;

    while (atual != NULL) {
        printf("\n----------------------------------------\n");
        printf("Você está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("[🔎 PISTA ENCONTRADA]: %s\n", atual->pista);
            bstPistas = inserirPista(bstPistas, atual->pista);
        } else {
            printf("Não há pistas aparentes neste local.\n");
        }

        printf("\nCaminhos disponíveis:\n");
        if (atual->esquerda != NULL) printf(" [e] Ir para a porta à esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf(" [d] Ir para a porta à direita (%s)\n", atual->direita->nome);
        printf(" [s] Parar exploração e ir para o Julgamento Final\n");
        
        printf("Sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 's' || escolha == 'S') {
            printf("\nExploração finalizada! Preparando o tribunal...\n");
            break;
        } else if ((escolha == 'e' || escolha == 'E') && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if ((escolha == 'd' || escolha == 'D') && atual->direita != NULL) {
            atual = atual->direita;
        } else {
            printf("\nCaminho inválido! Tente novamente.\n");
        }
    }
    return bstPistas;
}

/* ==========================================================
 * LOGICA DE JULGAMENTO
 * ========================================================== */

// Conta recursivamente quantas pistas na BST apontam para o suspeito acusado usando a Hash Table
int contarPistasDoSuspeito(PistaNode *raizBST, TabelaHash *th, char *suspeitoAcusado) {
    if (raizBST == NULL) return 0;
    
    int contagemAtual = 0;
    char *suspeitoVinculado = encontrarSuspeito(th, raizBST->descricao);
    
    if (suspeitoVinculado != NULL && strcasecmp(suspeitoVinculado, suspeitoAcusado) == 0) {
        contagemAtual = 1;
    }
    
    return contagemAtual + 
           contarPistasDoSuspeito(raizBST->esquerda, th, suspeitoAcusado) + 
           contarPistasDoSuspeito(raizBST->direita, th, suspeitoAcusado);
}

/* ----------------------------------------------------------
 * Função: verificarSuspeitoFinal()
 * Propósito: Conduz o julgamento, pede a acusação e valida as provas.
 * ---------------------------------------------------------- */
void verificarSuspeitoFinal(PistaNode *bstPistas, TabelaHash *th) {
    char acusado[50];
    
    printf("\n========================================\n");
    printf("           O JULGAMENTO FINAL           \n");
    printf("========================================\n");
    
    if (bstPistas == NULL) {
        printf("Você não coletou nenhuma evidência! O culpado saiu livre por falta de provas.\n");
        return;
    }
    
    printf("Pistas Coletadas em sua jornada:\n");
    exibirPistas(bstPistas);
    printf("----------------------------------------\n");
    
    printf("Digite o nome do seu Suspeito Principal (Ex: Mordomo, Governanta, Jardineiro): ");
    scanf(" %[^\n]", acusado); // Lê a string com espaços
    
    // Processamento automático via algoritmo
    int totalProvas = contarPistasDoSuspeito(bstPistas, th, acusado);
    
    printf("\n[RESULTADO DA ACUSAÇÃO]:\n");
    printf("Você apresentou %d prova(s) contra o(a) %s.\n", totalProvas, acusado);
    
    if (totalProvas >= 2) {
        printf("⚖️ PARABÉNS! As provas são incontestáveis. O(A) %s foi condenado(a)!\n", acusado);
        printf("Caso Encerrado com Sucesso, Detetive!\n");
    } else {
        printf("❌ CASO ARQUIVADO! Provas insuficientes. O(A) %s foi absolvido(a) por falta de evidências substanciais.\n", acusado);
    }
    printf("========================================\n");
}

/* ==========================================================
 * EXECUÇÃO PRINCIPAL
 * ========================================================== */
int main() {
    // Inicializa Tabela Hash com ponteiros nulos
    TabelaHash th;
    for (int i = 0; i < TAMANHO_HASH; i++) th.tabela[i] = NULL;

    // Definição das pistas estáticas e seus suspeitos (Carga da Hash)
    inserirNaHash(&th, "Bilhete antigo rasgado", "Mordomo");
    inserirNaHash(&th, "Diário secreto com cadeado quebrado", "Governanta");
    inserirNaHash(&th, "Xícara de chá ainda morna", "Mordomo");
    inserirNaHash(&th, "Pegada de lama perto da janela", "Jardineiro");
    inserirNaHash(&th, "Chave dourada caída no vaso", "Jardineiro");
    inserirNaHash(&th, "Testamento alterado recentemente", "Governanta");

    // Construção do Cenário do Mapa da Mansão
    Sala *hall = criarSala("Hall de entrada", "Bilhete antigo rasgado");
    Sala *corredorEsq = criarSala("Corredor Esquerdo", NULL);
    Sala *corredorDir = criarSala("Corredor Direito", "Pegada de lama perto da janela");
    Sala *biblioteca = criarSala("Biblioteca", "Diário secreto com cadeado quebrado");
    Sala *cozinha = criarSala("Cozinha", "Xícara de chá ainda morna");
    Sala *jardim = criarSala("Jardim de Inverno", "Chave dourada caída no vaso");
    Sala *escritorio = criarSala("Escritório do Barão", "Testamento alterado recentemente");

    hall->esquerda = corredorEsq;
    hall->direita = corredorDir;
    corredorEsq->esquerda = biblioteca;
    corredorEsq->direita = cozinha;
    corredorDir->esquerda = jardim;
    corredorDir->direita = escritorio;

    // Fase 1: Exploração e Coleta (BST)
    PistaNode *pistasColetadas = explorarSalas(hall);

    // Fase 2: Julgamento Cruzando Dados (BST + Hash)
    verificarSuspeitoFinal(pistasColetadas, &th);

    // Fase 3: Desalocação Limpa de Memória
    liberarPistas(pistasColetadas);
    liberarHash(&th);
    free(biblioteca); free(cozinha); free(jardim); free(escritorio);
    free(corredorEsq); free(corredorDir); free(hall);

    return 0;
}