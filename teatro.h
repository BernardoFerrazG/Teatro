#ifndef TEATRO_H
#define TEATRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char nome[2];
    int vazio;
}Assento;

typedef struct{
    Assento *a;    
    int numassento;
}Fila;

typedef struct{
    Fila *fila;
    int numfila;
    int existe;
}Teatro;

void execMenu();
int exibeMenu();
Teatro criaTeatro();
void imprimeTeatro(Teatro teatro);
int vendaIngresso(Teatro teatro);
void devolucaoIngresso(Teatro teatro);
void liberaTeatro(Teatro t);

#endif // TEATRO_H