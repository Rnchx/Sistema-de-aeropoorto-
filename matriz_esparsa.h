#ifndef MATRIZ_ESPARSA_H
#define MATRIZ_ESPARSA_H

/* ============================================================
 *  matriz_esparsa.h
 *  Matriz esparsa genérica implementada com listas encadeadas.
 *  Cada célula guarda um void* (dado genérico) e a posição
 *  (linha, coluna) na matriz.
 * ============================================================ */

/* Nó de uma célula da matriz esparsa */
typedef struct Celula {
    int linha;
    int coluna;
    void *dado;           /* ponteiro genérico para o dado armazenado */
    struct Celula *prox;  /* próximo nó na lista encadeada */
} Celula;

/* Cabeça da matriz esparsa */
typedef struct {
    Celula *inicio;  /* início da lista de células */
    int total;       /* número de células ocupadas */
} MatrizEsparsa;

/* --- Protótipos --- */
MatrizEsparsa *criarMatriz(void);
void destruirMatriz(MatrizEsparsa *m, void (*liberarDado)(void *));

int inserirCelula(MatrizEsparsa *m, int linha, int coluna, void *dado);
int removerCelula(MatrizEsparsa *m, int linha, int coluna, void (*liberarDado)(void *));
Celula *buscarCelula(MatrizEsparsa *m, int linha, int coluna);

/* Itera sobre todas as células de uma linha */
Celula *primeiraDaLinha(MatrizEsparsa *m, int linha);
Celula *proximaDaLinha(Celula *atual, int linha);

#endif /* MATRIZ_ESPARSA_H */
