#include "teatro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int teatro_cria(Teatro *teatro, int num_filas, int assentos_por_fila) {
    if (num_filas <= 0 || assentos_por_fila <= 0) {
        return -1;
    }

    teatro->filas = calloc((size_t)num_filas, sizeof *teatro->filas);
    if (teatro->filas == NULL) {
        return -1;
    }
    teatro->num_filas = num_filas;

    for (int i = 0; i < num_filas; i++) {
        teatro->filas[i].assentos = calloc((size_t)assentos_por_fila, sizeof(Assento));
        if (teatro->filas[i].assentos == NULL) {
            teatro_libera(teatro);
            return -1;
        }
        teatro->filas[i].num_assentos = assentos_por_fila;
    }
    return 0;
}

void teatro_libera(Teatro *teatro) {
    if (teatro->filas != NULL) {
        for (int i = 0; i < teatro->num_filas; i++) {
            free(teatro->filas[i].assentos);
        }
        free(teatro->filas);
    }
    teatro->filas = NULL;
    teatro->num_filas = 0;
}

int teatro_existe(const Teatro *teatro) {
    return teatro->filas != NULL;
}

int teatro_sem_vendas(const Teatro *teatro) {
    for (int i = 0; i < teatro->num_filas; i++) {
        if (teatro_livres_na_fila(teatro, i + 1) != teatro->filas[i].num_assentos) {
            return 0;
        }
    }
    return 1;
}

int teatro_livres_na_fila(const Teatro *teatro, int fila) {
    const Fila *f = &teatro->filas[fila - 1];
    int livres = 0;
    for (int j = 0; j < f->num_assentos; j++) {
        livres += !f->assentos[j].ocupado;
    }
    return livres;
}

/* Move os assentos ocupados para o início da fila, mantendo a ordem dos compradores. */
static void compacta_fila(Fila *fila) {
    int destino = 0;
    for (int j = 0; j < fila->num_assentos; j++) {
        if (fila->assentos[j].ocupado) {
            if (j != destino) {
                fila->assentos[destino] = fila->assentos[j];
                fila->assentos[j] = (Assento){0};
            }
            destino++;
        }
    }
}

int teatro_vende(Teatro *teatro, int fila, int quantidade, const char *nome) {
    if (!teatro_existe(teatro) || fila < 1 || fila > teatro->num_filas || quantidade <= 0 ||
        nome == NULL || nome[0] == '\0') {
        return -1;
    }
    if (teatro_livres_na_fila(teatro, fila) < quantidade) {
        return -1;
    }

    Fila *f = &teatro->filas[fila - 1];
    compacta_fila(f);

    for (int j = 0; j < f->num_assentos && quantidade > 0; j++) {
        if (!f->assentos[j].ocupado) {
            f->assentos[j].ocupado = 1;
            snprintf(f->assentos[j].nome, TAM_NOME, "%s", nome);
            quantidade--;
        }
    }
    return 0;
}

int teatro_devolve(Teatro *teatro, const char *nome) {
    int liberados = 0;
    for (int i = 0; i < teatro->num_filas; i++) {
        Fila *f = &teatro->filas[i];
        for (int j = 0; j < f->num_assentos; j++) {
            if (f->assentos[j].ocupado && strcmp(f->assentos[j].nome, nome) == 0) {
                f->assentos[j] = (Assento){0};
                liberados++;
            }
        }
    }
    return liberados;
}

void teatro_imprime(const Teatro *teatro) {
    for (int i = 0; i < teatro->num_filas; i++) {
        printf("Fila %2d | ", i + 1);
        for (int j = 0; j < teatro->filas[i].num_assentos; j++) {
            const Assento *a = &teatro->filas[i].assentos[j];
            printf("%c ", a->ocupado ? a->nome[0] : '-');
        }
        printf("\n");
    }
}
