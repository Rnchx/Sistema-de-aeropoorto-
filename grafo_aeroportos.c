/* ============================================================
 *  grafo_aeroportos.c
 *  Implementação da malha aérea com vetor dinâmico +
 *  matriz esparsa de adjacência.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_aeroportos.h"

/* ---- helpers internos ---- */

static void liberarVoo(void *p) { free(p); }

/* Cresce o vetor dinâmico quando cheio */
static int expandirVetor(GrafoAeroportos *g) {
    int novaCapacidade = g->capacidade * 2;
    Aeroporto *novo = (Aeroporto *)realloc(g->aeroportos,
                                           novaCapacidade * sizeof(Aeroporto));
    if (!novo) return 0;
    g->aeroportos = novo;
    g->capacidade = novaCapacidade;
    return 1;
}

/* ---- API pública ---- */

GrafoAeroportos *criarGrafo(int capacidadeInicial) {
    if (capacidadeInicial <= 0) capacidadeInicial = 4;

    GrafoAeroportos *g = (GrafoAeroportos *)malloc(sizeof(GrafoAeroportos));
    if (!g) return NULL;

    g->aeroportos = (Aeroporto *)malloc(capacidadeInicial * sizeof(Aeroporto));
    if (!g->aeroportos) { free(g); return NULL; }

    g->qtd        = 0;
    g->capacidade = capacidadeInicial;
    g->voos       = criarMatriz();
    if (!g->voos) { free(g->aeroportos); free(g); return NULL; }

    return g;
}

void destruirGrafo(GrafoAeroportos *g) {
    if (!g) return;
    destruirMatriz(g->voos, liberarVoo);
    free(g->aeroportos);
    free(g);
}

/* Retorna índice do aeroporto ou -1 */
int buscarIndiceAeroporto(GrafoAeroportos *g, const char *codigo) {
    for (int i = 0; i < g->qtd; i++)
        if (strcmp(g->aeroportos[i].codigo, codigo) == 0)
            return i;
    return -1;
}

/* --- Operação 1: cadastrar aeroporto --- */
int cadastrarAeroporto(GrafoAeroportos *g, const char *codigo,
                       const char *cidade) {
    if (!g || !codigo || !cidade) return 0;

    if (buscarIndiceAeroporto(g, codigo) != -1) {
        printf("  [FALHA] Aeroporto '%s' ja cadastrado.\n", codigo);
        return 0;
    }

    /* expande vetor se necessário */
    if (g->qtd == g->capacidade)
        if (!expandirVetor(g)) {
            printf("  [FALHA] Memoria insuficiente.\n");
            return 0;
        }

    strncpy(g->aeroportos[g->qtd].codigo, codigo, COD_MAX - 1);
    g->aeroportos[g->qtd].codigo[COD_MAX - 1] = '\0';
    strncpy(g->aeroportos[g->qtd].cidade, cidade, CIDADE_MAX - 1);
    g->aeroportos[g->qtd].cidade[CIDADE_MAX - 1] = '\0';
    g->qtd++;

    printf("  [OK] Aeroporto %s (%s) cadastrado no indice %d.\n",
           codigo, cidade, g->qtd - 1);
    return 1;
}

/* --- Operação 2: cadastrar voo --- */
int cadastrarVoo(GrafoAeroportos *g, const char *codOrigem,
                 const char *codDestino, int numeroVoo) {
    if (!g) return 0;

    int orig = buscarIndiceAeroporto(g, codOrigem);
    int dest = buscarIndiceAeroporto(g, codDestino);

    if (orig == -1) {
        printf("  [FALHA] Aeroporto de origem '%s' nao encontrado.\n", codOrigem);
        return 0;
    }
    if (dest == -1) {
        printf("  [FALHA] Aeroporto de destino '%s' nao encontrado.\n", codDestino);
        return 0;
    }
    if (orig == dest) {
        printf("  [FALHA] Origem e destino sao o mesmo aeroporto.\n");
        return 0;
    }

    /* verifica se o número de voo já existe na matriz inteira */
    Celula *c = g->voos->inicio;
    while (c) {
        Voo *v = (Voo *)c->dado;
        if (v->numero == numeroVoo) {
            printf("  [FALHA] Voo %d ja existe.\n", numeroVoo);
            return 0;
        }
        c = c->prox;
    }

    /* verifica se já existe voo nessa rota (mesma célula) */
    if (buscarCelula(g->voos, orig, dest)) {
        printf("  [FALHA] Ja existe um voo de %s para %s.\n",
               codOrigem, codDestino);
        return 0;
    }

    Voo *novoVoo = (Voo *)malloc(sizeof(Voo));
    if (!novoVoo) { printf("  [FALHA] Memoria insuficiente.\n"); return 0; }
    novoVoo->numero = numeroVoo;

    if (!inserirCelula(g->voos, orig, dest, novoVoo)) {
        free(novoVoo);
        printf("  [FALHA] Erro ao inserir na matriz.\n");
        return 0;
    }

    printf("  [OK] Voo %d cadastrado: %s (%s) -> %s (%s).\n",
           numeroVoo,
           g->aeroportos[orig].codigo, g->aeroportos[orig].cidade,
           g->aeroportos[dest].codigo, g->aeroportos[dest].cidade);
    return 1;
}

/* --- Operação 3: remover voo pelo número --- */
int removerVoo(GrafoAeroportos *g, int numeroVoo) {
    if (!g) return 0;

    Celula *c = g->voos->inicio;
    while (c) {
        Voo *v = (Voo *)c->dado;
        if (v->numero == numeroVoo) {
            int lin = c->linha, col = c->coluna;
            printf("  [OK] Removendo voo %d: %s -> %s.\n",
                   numeroVoo,
                   g->aeroportos[lin].cidade,
                   g->aeroportos[col].cidade);
            removerCelula(g->voos, lin, col, liberarVoo);
            return 1;
        }
        c = c->prox;
    }

    printf("  [FALHA] Voo %d nao encontrado.\n", numeroVoo);
    return 0;
}

/* --- Operação 4: listar voos de um aeroporto --- */
void listarVoosDeAeroporto(GrafoAeroportos *g, const char *codOrigem) {
    if (!g) return;

    int orig = buscarIndiceAeroporto(g, codOrigem);
    if (orig == -1) {
        printf("  [FALHA] Aeroporto '%s' nao encontrado.\n", codOrigem);
        return;
    }

    printf("\n  Voos partindo de %s (%s):\n",
           g->aeroportos[orig].codigo, g->aeroportos[orig].cidade);

    Celula *c = primeiraDaLinha(g->voos, orig);
    if (!c) { printf("    Nenhum voo cadastrado.\n"); return; }

    int cont = 0;
    while (c) {
        Voo *v = (Voo *)c->dado;
        printf("    Voo %03d  ->  %s (%s)\n",
               v->numero,
               g->aeroportos[c->coluna].cidade,
               g->aeroportos[c->coluna].codigo);
        cont++;
        c = proximaDaLinha(c, orig);
    }
    printf("  Total: %d voo(s).\n", cont);
}

/* --- Operação 5: listar trajetos (DFS recursivo) ---
 * visitados: array de flags para evitar ciclos
 * caminho  : pilha com índices do caminho atual
 * profund  : profundidade atual                         */

static void dfsTrajetoRec(GrafoAeroportos *g,
                           int atual, int destino,
                           int *visitados, int *caminho, int profund,
                           int *encontrou) {
    visitados[atual] = 1;
    caminho[profund] = atual;

    if (atual == destino) {
        *encontrou = 1;
        printf("    Trajeto: ");
        for (int i = 0; i <= profund; i++) {
            printf("%s(%s)", g->aeroportos[caminho[i]].codigo,
                             g->aeroportos[caminho[i]].cidade);
            if (i < profund) printf(" -> ");
        }
        printf("\n");
    } else {
        /* percorre todos os vizinhos de 'atual' */
        Celula *c = primeiraDaLinha(g->voos, atual);
        while (c) {
            int prox = c->coluna;
            if (!visitados[prox])
                dfsTrajetoRec(g, prox, destino,
                              visitados, caminho, profund + 1, encontrou);
            c = proximaDaLinha(c, atual);
        }
    }

    visitados[atual] = 0; /* desmarca para permitir outros trajetos */
}

void listarTrajetos(GrafoAeroportos *g,
                    const char *codOrigem, const char *codDestino) {
    if (!g) return;

    int orig = buscarIndiceAeroporto(g, codOrigem);
    int dest = buscarIndiceAeroporto(g, codDestino);

    if (orig == -1) {
        printf("  [FALHA] Aeroporto de origem '%s' nao encontrado.\n", codOrigem);
        return;
    }
    if (dest == -1) {
        printf("  [FALHA] Aeroporto de destino '%s' nao encontrado.\n", codDestino);
        return;
    }
    if (orig == dest) {
        printf("  [FALHA] Origem e destino sao o mesmo aeroporto.\n");
        return;
    }

    int *visitados = (int *)calloc(g->qtd, sizeof(int));
    int *caminho   = (int *)malloc(g->qtd  * sizeof(int));
    if (!visitados || !caminho) {
        printf("  [FALHA] Memoria insuficiente.\n");
        free(visitados); free(caminho);
        return;
    }

    int encontrou = 0;
    printf("\n  Trajetos de %s (%s) ate %s (%s):\n",
           g->aeroportos[orig].codigo, g->aeroportos[orig].cidade,
           g->aeroportos[dest].codigo, g->aeroportos[dest].cidade);

    dfsTrajetoRec(g, orig, dest, visitados, caminho, 0, &encontrou);

    if (!encontrou)
        printf("    Nenhum trajeto encontrado.\n");

    free(visitados);
    free(caminho);
}
