#ifndef TEATRO_H
#define TEATRO_H

#include <stddef.h>

#define TAM_NOME 32

typedef struct {
    char nome[TAM_NOME]; /* nome do comprador; vazio quando o assento está livre */
    int ocupado;
} Assento;

typedef struct {
    Assento *assentos;
    int num_assentos;
} Fila;

typedef struct {
    Fila *filas;
    int num_filas;
} Teatro;

/* Aloca um teatro com todas as poltronas livres. Retorna 0 em caso de sucesso. */
int teatro_cria(Teatro *teatro, int num_filas, int assentos_por_fila);

/* Libera a memória do teatro e o deixa vazio (pode ser chamada mais de uma vez). */
void teatro_libera(Teatro *teatro);

int teatro_existe(const Teatro *teatro);
int teatro_sem_vendas(const Teatro *teatro);
int teatro_livres_na_fila(const Teatro *teatro, int fila);

/*
 * Vende `quantidade` ingressos na fila `fila` (1-based) para `nome`.
 * Se os lugares livres estiverem espalhados, os compradores já sentados na fila
 * são reacomodados para o início, para que o grupo fique junto.
 * Retorna 0 em caso de sucesso, ou -1 se não houver lugares suficientes / parâmetros inválidos.
 */
int teatro_vende(Teatro *teatro, int fila, int quantidade, const char *nome);

/* Devolve todos os ingressos em nome de `nome`. Retorna quantos assentos foram liberados. */
int teatro_devolve(Teatro *teatro, const char *nome);

/* Mapa do teatro: cada poltrona mostra a inicial do comprador, ou '-' se estiver livre. */
void teatro_imprime(const Teatro *teatro);

#endif
