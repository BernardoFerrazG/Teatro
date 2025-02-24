#include "teatro.h"

int exibeMenu(){
    printf("\n");
    printf("1 - Criar novo teatro\n");
    printf("2 - Imprimir mapa\n");
    printf("3 - Venda\n");
    printf("4 - Devolução\n");
    printf("5 - Sair\n");
    printf("\n");
    int opção;
    scanf("%d", &opção);

    return opção;
}


void execMenu(){           // Executa as funções do menu
    int opcao = exibeMenu();
    Teatro t;
    t.existe = 0;  

    while (opcao != 5){
        int cont = 0;
        if (opcao == 1) {
            if (t.existe){
                for(int i = 0; i < t.numfila; i++){
                    for(int j = 0; j < t.fila[i].numassento; j++){
                        if(t.fila[i].a[j].vazio == 0){
                            cont++;
                        }
                    }
                }
                if(cont == (t.numfila * t.fila[0].numassento)){
                    t.existe = 0;
                    t = criaTeatro();
                }
                else{
                    printf("Teatro não vazio\n");
                }
            }
            else{
                t = criaTeatro();
            }
        } else if (opcao == 2){
            if (t.existe){
                imprimeTeatro(t);
            } else{
                printf("Teatro não criado. Por favor, crie o teatro primeiro.\n");
            }
        } else if (opcao == 3){
            if (t.existe){
                if (vendaIngresso(t)){
                    imprimeTeatro(t);
                }
            } else {
                printf("Teatro não criado. Por favor, crie o teatro primeiro.\n");
            }
        } else if (opcao == 4){
            if (t.existe){
                devolucaoIngresso(t);
                imprimeTeatro(t);
            } else{
                printf("Teatro não criado. Por favor, crie o teatro primeiro.\n");
            }
        } else{
            printf("Opção inválida\n");
        }
        opcao = exibeMenu();
    }

    if (t.existe){
        liberaTeatro(t);
    }
}


Teatro criaTeatro(){        // Cria o teatro e realiza as alocações
    int numfila, numassento;

    while(1){
        printf("Digite o número de filas: ");
        if(scanf("%d", &numfila) == 1 && numfila > 0){
            break;
        } else {
            printf("Número inválido. Por favor, tente novamente.\n");
        }
    }

    while(1){
        printf("Digite o número de assento por fila: ");
        if(scanf("%d", &numassento) == 1 && numassento > 0){
            break;
        } else {
            printf("Número inválido. Por favor, tente novamente.\n");
        }
    }

    Teatro teatro;
    teatro.numfila = numfila;
    teatro.fila = (Fila*)malloc(numfila * sizeof(Fila));
    
    for(int i = 0; i < teatro.numfila; i++){
        teatro.fila[i].numassento = numassento;
        teatro.fila[i].a = (Assento*)malloc(numassento * sizeof(Assento));
        for(int j = 0; j < teatro.fila[i].numassento; j++){
            strcpy(teatro.fila[i].a[j].nome, "-");
            teatro.fila[i].a[j].vazio = 0;
        }
    }
    teatro.existe = 1;
    return teatro;
}

void imprimeTeatro(Teatro teatro){          // Imprime o teatro
    for(int i = 0; i < teatro.numfila; i++){
        for(int j = 0; j < teatro.fila[i].numassento; j++){
            printf("%s ", teatro.fila[i].a[j].nome);
        }
        printf("\n");
    }
}

int vendaIngresso(Teatro teatro){     // Realiza a venda, registrando o nome, a quantidade de ingressos e a fileira desejada
    char nome[2];
    int numentradas, fileira;
    printf("Nome do comprador: ");
    getchar();
    fgets(nome, 2, stdin);

    

    while(1){
        printf("\nQuantidade de entradas: ");
        if(scanf("%d", &numentradas) == 1 && numentradas > 0 && numentradas <= teatro.fila->numassento){
            break;
        } else {
            printf("Número inválido. Por favor, tente novamente.\n");
        }
    }

    while(1){
        printf("\nFileira desejada: ");
        if(scanf("%d", &fileira) == 1 && fileira > 0 && fileira <= teatro.numfila){
            break;
        }else{
            printf("Número inválido ou fileira inexistente. Por favor, tente novamente.\n");
        }
    }
    
    
    for(int i = 0; i < teatro.fila[fileira - 1].numassento; i++){
        if(teatro.fila[fileira - 1].a[i].vazio == 1){
            for(int j = i - 1; j >= 0; j--){
                if(teatro.fila[fileira - 1].a[j].vazio == 0){
                    teatro.fila[fileira - 1].a[j].vazio = 1;
                    strcpy(teatro.fila[fileira - 1].a[j].nome, teatro.fila[fileira - 1].a[j+1].nome);

                    teatro.fila[fileira - 1].a[j+1].vazio = 0;
                    strcpy(teatro.fila[fileira - 1].a[j+1].nome, "-");
                }
            }
        }
    }
    

    int cont = 0;
    for(int i = 0; i < teatro.fila[fileira - 1].numassento; i++){
        if(teatro.fila[fileira - 1].a[i].vazio == 0){
            cont++;
        }
    }

    if(cont >= numentradas){
        for(int i = 0; i < teatro.fila[fileira - 1].numassento && numentradas > 0; i++){
            if(teatro.fila[fileira - 1].a[i].vazio == 0){
                strcpy(teatro.fila[fileira - 1].a[i].nome, nome);
                teatro.fila[fileira - 1].a[i].vazio = 1;
                printf("Assento %d reservado\n", i + 1);
                numentradas--;
            }
        }
        printf("Venda Concluída\n\n");
        return 1;
    }
    else{
        printf("Nao ha espaco na fila solicitada. Por favor refaca a compra com outra fila.\n");
        return 0;
    }
}

void devolucaoIngresso(Teatro teatro){      // Realiza a devolução do ingresso pelo nome do cliente
    char nome[2];
    printf("Nome do cliente: ");
    getchar();
    fgets(nome, 2, stdin);
    printf("\n");

    for(int i = 0; i < teatro.numfila; i++){
        for(int j = 0; j < teatro.fila[i].numassento; j++){
            if(strcmp(teatro.fila[i].a[j].nome, nome) == 0){
                strcpy(teatro.fila[i].a[j].nome, "-");
                teatro.fila[i].a[j].vazio = 0;
            }
        }
    }
    
    printf("Cancelamento concluido\n\n");
}

void liberaTeatro(Teatro t){            // desaloca o teatro criado ao encerrar o programa
    for(int i = 0; i < t.numfila; i++) {
        free(t.fila[i].a);
    }
    free(t.fila);
}
