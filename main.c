/* ============================================================
 *  main.c
 *  Menu interativo para o sistema de malha aérea da ANAC.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo_aeroportos.h"

/* Limpa o buffer do teclado */
static void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lê uma string do teclado com limite de tamanho.
 * Sempre esvazia o restante do buffer para evitar que
 * '\n' sobrando atrapalhe a próxima leitura. */
static void lerString(const char *prompt, char *dest, int maxLen) {
    printf("%s", prompt);
    fflush(stdout);

    if (fgets(dest, maxLen, stdin)) {
        /* Se o '\n' foi lido, ele fica em dest — removemos */
        char *nl = strchr(dest, '\n');
        if (nl) {
            *nl = '\0';          /* apaga o '\n' normalmente */
        } else {
            /* '\n' ficou no buffer (entrada maior que maxLen-1): limpa */
            limparBuffer();
        }
    }
}

/* ---- Popula o grafo com os dados do enunciado para teste ---- */
static void carregarDadosIniciais(GrafoAeroportos *g) {
    printf("\n=== Carregando aeroportos e voos do enunciado ===\n");

    /* Aeroportos: índice 0=BSB, 1=CNF, 2=GIG, 3=GRU, 4=SSA */
    cadastrarAeroporto(g, "BSB", "Brasilia");
    cadastrarAeroporto(g, "CNF", "Belo Horizonte");
    cadastrarAeroporto(g, "GIG", "Rio de Janeiro");
    cadastrarAeroporto(g, "GRU", "Sao Paulo");
    cadastrarAeroporto(g, "SSA", "Salvador");

    /* Voos conforme a matriz do enunciado (linha x coluna = origem x destino) */
    /* Linha 0 (BSB): col 4 (SSA) = voo 107 */
    cadastrarVoo(g, "BSB", "SSA", 107);
    /* Linha 1 (CNF): col 2 (GIG)=555, col 3 (GRU)=101, col 4 (SSA)=214 */
    cadastrarVoo(g, "CNF", "GIG", 555);
    cadastrarVoo(g, "CNF", "GRU", 101);
    cadastrarVoo(g, "CNF", "SSA", 214);
    /* Linha 2 (GIG): col 1 (CNF)=554, col 3 (GRU)=090 */
    cadastrarVoo(g, "GIG", "CNF", 554);
    cadastrarVoo(g, "GIG", "GRU",  90);
    /* Linha 3 (GRU): col 0 (BSB)=050, col 1 (CNF)=102, col 2 (GIG)=089 */
    cadastrarVoo(g, "GRU", "BSB",  50);
    cadastrarVoo(g, "GRU", "CNF", 102);
    cadastrarVoo(g, "GRU", "GIG",  89);
    /* Linha 4 (SSA): col 1 (CNF)=215 */
    cadastrarVoo(g, "SSA", "CNF", 215);

    printf("=================================================\n\n");
}

/* ---- Funções do menu ---- */

static void menuCadastrarAeroporto(GrafoAeroportos *g) {
    /* Buffer maior que COD_MAX para que o fgets consiga capturar
     * o '\n' dentro dele mesmo — evita que ele vaze para a próxima leitura */
    char cod[32];
    char cidade[CIDADE_MAX];
    lerString("  Codigo IATA (ex: GRU): ", cod, sizeof(cod));
    cod[COD_MAX - 1] = '\0'; /* garante truncamento no tamanho correto */
    lerString("  Nome da cidade       : ", cidade, CIDADE_MAX);
    cadastrarAeroporto(g, cod, cidade);
}

static void menuCadastrarVoo(GrafoAeroportos *g) {
    char orig[32], dest[32];
    int num;
    lerString("  Codigo origem  : ", orig, sizeof(orig));
    orig[COD_MAX - 1] = '\0';
    lerString("  Codigo destino : ", dest, sizeof(dest));
    dest[COD_MAX - 1] = '\0';
    printf("  Numero do voo  : ");
    if (scanf("%d", &num) != 1) { limparBuffer(); printf("  Entrada invalida.\n"); return; }
    limparBuffer();
    cadastrarVoo(g, orig, dest, num);
}

static void menuRemoverVoo(GrafoAeroportos *g) {
    int num;
    printf("  Numero do voo a remover: ");
    if (scanf("%d", &num) != 1) { limparBuffer(); printf("  Entrada invalida.\n"); return; }
    limparBuffer();
    removerVoo(g, num);
}

static void menuListarVoos(GrafoAeroportos *g) {
    char cod[32];
    lerString("  Codigo do aeroporto de origem: ", cod, sizeof(cod));
    cod[COD_MAX - 1] = '\0';
    listarVoosDeAeroporto(g, cod);
}

static void menuListarTrajetos(GrafoAeroportos *g) {
    char orig[32], dest[32];
    lerString("  Codigo origem  : ", orig, sizeof(orig));
    orig[COD_MAX - 1] = '\0';
    lerString("  Codigo destino : ", dest, sizeof(dest));
    dest[COD_MAX - 1] = '\0';
    listarTrajetos(g, orig, dest);
}

static void menuListarAeroportos(GrafoAeroportos *g) {
    if (g->qtd == 0) { printf("  Nenhum aeroporto cadastrado.\n"); return; }
    printf("\n  %-6s  %-s\n", "Codigo", "Cidade");
    printf("  ------  ----------------\n");
    for (int i = 0; i < g->qtd; i++)
        printf("  %-6s  %s\n", g->aeroportos[i].codigo, g->aeroportos[i].cidade);
    printf("  Total: %d aeroporto(s).\n", g->qtd);
}

/* ---- Programa principal ---- */

int main(void) {
    GrafoAeroportos *grafo = criarGrafo(4);
    if (!grafo) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return 1;
    }

    carregarDadosIniciais(grafo);

    int opcao;
    do {
        printf("\n╔══════════════════════════════════╗\n");
        printf("║    SISTEMA DE MALHA AEREA ANAC   ║\n");
        printf("╠══════════════════════════════════╣\n");
        printf("║ 1. Cadastrar aeroporto            ║\n");
        printf("║ 2. Cadastrar voo                  ║\n");
        printf("║ 3. Remover voo                    ║\n");
        printf("║ 4. Listar voos de um aeroporto    ║\n");
        printf("║ 5. Listar trajetos possiveis      ║\n");
        printf("║ 6. Listar todos os aeroportos     ║\n");
        printf("║ 0. Sair                           ║\n");
        printf("╚══════════════════════════════════╝\n");
        printf("  Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        } else {
            limparBuffer();
        }

        printf("\n");
        switch (opcao) {
            case 1: menuCadastrarAeroporto(grafo); break;
            case 2: menuCadastrarVoo(grafo);       break;
            case 3: menuRemoverVoo(grafo);         break;
            case 4: menuListarVoos(grafo);         break;
            case 5: menuListarTrajetos(grafo);     break;
            case 6: menuListarAeroportos(grafo);   break;
            case 0: printf("  Encerrando. Ate logo!\n"); break;
            default: printf("  Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    destruirGrafo(grafo);
    return 0;
}