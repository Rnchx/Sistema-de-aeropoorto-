#ifndef GRAFO_AEROPORTOS_H
#define GRAFO_AEROPORTOS_H

/* ============================================================
 *  grafo_aeroportos.h
 *  Módulo que representa a malha aérea:
 *    - Vetor dinâmico de aeroportos (vértices)
 *    - Matriz esparsa de voos (arestas)
 * ============================================================ */

#include "matriz_esparsa.h"

#define COD_MAX   4   /* tamanho do código IATA, ex: "GRU" + '\0' */
#define CIDADE_MAX 50 /* tamanho máximo do nome da cidade          */

/* ---- Estruturas ---- */

typedef struct {
    char codigo[COD_MAX];    /* ex: "GRU" */
    char cidade[CIDADE_MAX]; /* ex: "São Paulo" */
} Aeroporto;

typedef struct {
    int numero; /* número do voo, ex: 555 */
} Voo;

typedef struct {
    Aeroporto *aeroportos; /* vetor dinâmico de aeroportos */
    int qtd;               /* quantidade atual de aeroportos */
    int capacidade;        /* capacidade alocada do vetor    */
    MatrizEsparsa *voos;   /* matriz esparsa de voos         */
} GrafoAeroportos;

/* ---- Protótipos ---- */

GrafoAeroportos *criarGrafo(int capacidadeInicial);
void destruirGrafo(GrafoAeroportos *g);

/* Operação 1 – cadastrar aeroporto */
int cadastrarAeroporto(GrafoAeroportos *g, const char *codigo, const char *cidade);

/* Operação 2 – cadastrar voo */
int cadastrarVoo(GrafoAeroportos *g, const char *codOrigem,
                 const char *codDestino, int numeroVoo);

/* Operação 3 – remover voo pelo número */
int removerVoo(GrafoAeroportos *g, int numeroVoo);

/* Operação 4 – listar voos que saem de um aeroporto */
void listarVoosDeAeroporto(GrafoAeroportos *g, const char *codOrigem);

/* Operação 5 – listar todos os trajetos entre dois aeroportos */
void listarTrajetos(GrafoAeroportos *g, const char *codOrigem,
                    const char *codDestino);

/* Utilitário: retorna índice do aeroporto no vetor, ou -1 */
int buscarIndiceAeroporto(GrafoAeroportos *g, const char *codigo);

#endif /* GRAFO_AEROPORTOS_H */
