/* ============================================================
 *  matriz_esparsa.c
 *  Implementação da matriz esparsa genérica.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"

/* Aloca e inicializa uma nova matriz esparsa */
MatrizEsparsa *criarMatriz(void) {
    MatrizEsparsa *m = (MatrizEsparsa *)malloc(sizeof(MatrizEsparsa));
    if (!m) return NULL;
    m->inicio = NULL;
    m->total  = 0;
    return m;
}

/* Libera todas as células e a própria matriz.
 * liberarDado: função que sabe liberar o tipo concreto guardado. */
void destruirMatriz(MatrizEsparsa *m, void (*liberarDado)(void *)) {
    if (!m) return;
    Celula *atual = m->inicio;
    while (atual) {
        Celula *prox = atual->prox;
        if (liberarDado && atual->dado)
            liberarDado(atual->dado);
        free(atual);
        atual = prox;
    }
    free(m);
}

/* Insere uma célula (linha, coluna) com o dado apontado.
 * Retorna 1 em sucesso, 0 se já existia a célula ou falha de memória. */
int inserirCelula(MatrizEsparsa *m, int linha, int coluna, void *dado) {
    if (!m) return 0;
    /* verifica duplicata */
    if (buscarCelula(m, linha, coluna)) return 0;

    Celula *nova = (Celula *)malloc(sizeof(Celula));
    if (!nova) return 0;

    nova->linha  = linha;
    nova->coluna = coluna;
    nova->dado   = dado;
    nova->prox   = m->inicio;
    m->inicio    = nova;
    m->total++;
    return 1;
}

/* Remove a célula (linha, coluna).
 * Chama liberarDado para liberar o conteúdo se != NULL.
 * Retorna 1 em sucesso, 0 se não encontrou. */
int removerCelula(MatrizEsparsa *m, int linha, int coluna,
                  void (*liberarDado)(void *)) {
    if (!m || !m->inicio) return 0;

    Celula *ant  = NULL;
    Celula *atual = m->inicio;

    while (atual) {
        if (atual->linha == linha && atual->coluna == coluna) {
            if (ant) ant->prox = atual->prox;
            else     m->inicio = atual->prox;

            if (liberarDado && atual->dado)
                liberarDado(atual->dado);
            free(atual);
            m->total--;
            return 1;
        }
        ant  = atual;
        atual = atual->prox;
    }
    return 0; /* não encontrou */
}

/* Busca e retorna o ponteiro para a célula (linha, coluna), ou NULL. */
Celula *buscarCelula(MatrizEsparsa *m, int linha, int coluna) {
    if (!m) return NULL;
    Celula *atual = m->inicio;
    while (atual) {
        if (atual->linha == linha && atual->coluna == coluna)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

/* Retorna a primeira célula pertencente à 'linha', ou NULL. */
Celula *primeiraDaLinha(MatrizEsparsa *m, int linha) {
    if (!m) return NULL;
    Celula *atual = m->inicio;
    while (atual) {
        if (atual->linha == linha) return atual;
        atual = atual->prox;
    }
    return NULL;
}

/* Dado um nó da linha, retorna o próximo nó da MESMA linha. */
Celula *proximaDaLinha(Celula *atual, int linha) {
    if (!atual) return NULL;
    Celula *prox = atual->prox;
    while (prox) {
        if (prox->linha == linha) return prox;
        prox = prox->prox;
    }
    return NULL;
}
