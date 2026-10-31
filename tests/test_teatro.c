#include <stdio.h>
#include <string.h>

#include "../src/teatro.h"

static int falhas = 0;

#define CHECA(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);           \
            falhas++;                                                        \
        }                                                                    \
    } while (0)

static void test_cria_teatro_vazio(void) {
    Teatro t = {0};
    CHECA(!teatro_existe(&t));
    CHECA(teatro_cria(&t, 3, 4) == 0);
    CHECA(teatro_existe(&t));
    CHECA(teatro_sem_vendas(&t));
    CHECA(teatro_livres_na_fila(&t, 2) == 4);
    teatro_libera(&t);
    CHECA(!teatro_existe(&t));
    teatro_libera(&t); /* liberar duas vezes é seguro */
}

static void test_rejeita_parametros_invalidos(void) {
    Teatro t = {0};
    CHECA(teatro_cria(&t, 0, 5) != 0);
    CHECA(teatro_cria(&t, 2, -1) != 0);
    CHECA(teatro_cria(&t, 2, 3) == 0);
    CHECA(teatro_vende(&t, 0, 1, "Ana") != 0);
    CHECA(teatro_vende(&t, 3, 1, "Ana") != 0);
    CHECA(teatro_vende(&t, 1, 0, "Ana") != 0);
    CHECA(teatro_vende(&t, 1, 1, "") != 0);
    CHECA(teatro_vende(&t, 1, 4, "Ana") != 0); /* mais ingressos que assentos */
    CHECA(teatro_sem_vendas(&t));
    teatro_libera(&t);
}

static void test_venda_e_devolucao(void) {
    Teatro t = {0};
    teatro_cria(&t, 2, 5);
    CHECA(teatro_vende(&t, 1, 2, "Ana") == 0);
    CHECA(teatro_vende(&t, 1, 3, "Bruno") == 0);
    CHECA(teatro_livres_na_fila(&t, 1) == 0);
    CHECA(teatro_vende(&t, 1, 1, "Carla") != 0); /* fila cheia */
    CHECA(!teatro_sem_vendas(&t));

    CHECA(teatro_devolve(&t, "Ana") == 2);
    CHECA(teatro_devolve(&t, "Ana") == 0);
    CHECA(teatro_livres_na_fila(&t, 1) == 2);
    CHECA(teatro_devolve(&t, "Bruno") == 3);
    CHECA(teatro_sem_vendas(&t));
    teatro_libera(&t);
}

static void test_reacomoda_para_manter_grupo_junto(void) {
    Teatro t = {0};
    teatro_cria(&t, 1, 5);
    teatro_vende(&t, 1, 1, "Ana");    /* A - - - - */
    teatro_vende(&t, 1, 1, "Bruno");  /* A B - - - */
    teatro_vende(&t, 1, 1, "Carla");  /* A B C - - */
    teatro_devolve(&t, "Bruno");      /* A - C - - : lugares livres espalhados */

    CHECA(teatro_vende(&t, 1, 3, "Davi") == 0);
    /* Carla foi reacomodada para o lado da Ana e Davi ficou com 3 lugares juntos. */
    const Assento *a = t.filas[0].assentos;
    CHECA(strcmp(a[0].nome, "Ana") == 0);
    CHECA(strcmp(a[1].nome, "Carla") == 0);
    CHECA(strcmp(a[2].nome, "Davi") == 0 && strcmp(a[3].nome, "Davi") == 0 && strcmp(a[4].nome, "Davi") == 0);
    teatro_libera(&t);
}

static void test_nome_longo_e_truncado_com_seguranca(void) {
    Teatro t = {0};
    teatro_cria(&t, 1, 1);
    char nome[100];
    memset(nome, 'x', sizeof nome - 1);
    nome[sizeof nome - 1] = '\0';
    CHECA(teatro_vende(&t, 1, 1, nome) == 0);
    CHECA(strlen(t.filas[0].assentos[0].nome) == TAM_NOME - 1);
    teatro_libera(&t);
}

int main(void) {
    test_cria_teatro_vazio();
    test_rejeita_parametros_invalidos();
    test_venda_e_devolucao();
    test_reacomoda_para_manter_grupo_junto();
    test_nome_longo_e_truncado_com_seguranca();

    if (falhas == 0) {
        printf("Todos os testes passaram.\n");
        return 0;
    }
    printf("%d verificação(ões) falharam.\n", falhas);
    return 1;
}
