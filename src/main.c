#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "teatro.h"

/* Lê uma linha da entrada, sem o '\n'. Retorna 0 no fim da entrada. */
static int le_linha(const char *mensagem, char *buffer, size_t tamanho) {
    printf("%s", mensagem);
    fflush(stdout);
    if (fgets(buffer, (int)tamanho, stdin) == NULL) {
        return 0;
    }
    size_t len = strcspn(buffer, "\n");
    if (buffer[len] != '\n' && !feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            /* descarta o restante de uma linha longa demais */
        }
    }
    buffer[len] = '\0';
    return 1;
}

/* Pede um inteiro em [minimo, maximo] até receber um valor válido. Retorna 0 no fim da entrada. */
static int le_inteiro(const char *mensagem, int minimo, int maximo, int *valor) {
    char buffer[64];
    while (le_linha(mensagem, buffer, sizeof buffer)) {
        char *fim;
        long lido = strtol(buffer, &fim, 10);
        if (fim != buffer && *fim == '\0' && lido >= minimo && lido <= maximo) {
            *valor = (int)lido;
            return 1;
        }
        printf("Valor inválido: digite um número entre %d e %d.\n", minimo, maximo);
    }
    return 0;
}

static int le_nome(const char *mensagem, char nome[TAM_NOME]) {
    while (le_linha(mensagem, nome, TAM_NOME)) {
        if (nome[0] != '\0') {
            return 1;
        }
        printf("O nome não pode ser vazio.\n");
    }
    return 0;
}

static void criar(Teatro *teatro) {
    if (teatro_existe(teatro) && !teatro_sem_vendas(teatro)) {
        printf("Já existem ingressos vendidos; o teatro atual não pode ser substituído.\n");
        return;
    }
    int filas, assentos;
    if (!le_inteiro("Número de filas: ", 1, 100, &filas) ||
        !le_inteiro("Assentos por fila: ", 1, 100, &assentos)) {
        return;
    }
    teatro_libera(teatro);
    if (teatro_cria(teatro, filas, assentos) != 0) {
        printf("Erro: memória insuficiente.\n");
        return;
    }
    printf("Teatro criado com %d filas de %d assentos.\n", filas, assentos);
}

static void vender(Teatro *teatro) {
    char nome[TAM_NOME];
    int quantidade, fila;
    if (!le_nome("Nome do comprador: ", nome) ||
        !le_inteiro("Quantidade de ingressos: ", 1, teatro->filas[0].num_assentos, &quantidade) ||
        !le_inteiro("Fila desejada: ", 1, teatro->num_filas, &fila)) {
        return;
    }
    if (teatro_vende(teatro, fila, quantidade, nome) != 0) {
        printf("Não há %d lugares livres na fila %d (livres: %d). Escolha outra fila.\n", quantidade, fila,
               teatro_livres_na_fila(teatro, fila));
        return;
    }
    printf("Venda concluída: %d ingresso(s) na fila %d para %s.\n", quantidade, fila, nome);
    teatro_imprime(teatro);
}

static void devolver(Teatro *teatro) {
    char nome[TAM_NOME];
    if (!le_nome("Nome do cliente: ", nome)) {
        return;
    }
    int liberados = teatro_devolve(teatro, nome);
    if (liberados == 0) {
        printf("Nenhum ingresso encontrado em nome de %s.\n", nome);
        return;
    }
    printf("%d ingresso(s) devolvido(s).\n", liberados);
    teatro_imprime(teatro);
}

int main(void) {
    Teatro teatro = {0};
    int opcao;

    for (;;) {
        printf("\n1 - Criar novo teatro\n2 - Imprimir mapa\n3 - Venda\n4 - Devolução\n5 - Sair\n");
        if (!le_inteiro("> ", 1, 5, &opcao) || opcao == 5) {
            break;
        }
        if (opcao != 1 && !teatro_existe(&teatro)) {
            printf("Teatro não criado. Crie o teatro primeiro (opção 1).\n");
            continue;
        }
        switch (opcao) {
            case 1: criar(&teatro); break;
            case 2: teatro_imprime(&teatro); break;
            case 3: vender(&teatro); break;
            case 4: devolver(&teatro); break;
        }
    }

    teatro_libera(&teatro);
    return EXIT_SUCCESS;
}
