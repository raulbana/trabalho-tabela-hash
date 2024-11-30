//Estrutura programa Tabela Hash - Professor Lucas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

int maxsize = 30;

typedef struct Contato{
    char nome[51];
    char numero[11];
} Contato;

struct hashmap{
    Contato **map;
    int len;
} hashTable;

void inicializarTabela() {
    hashTable.map = (Contato **) calloc(sizeof(Contato), maxsize);
    for (int i = 0; i < maxsize; i++) {
        hashTable.map[i] = NULL;
    }
    hashTable.len = 0;
}

void rehash(){
    maxsize++;
    Contato** oldmap = hashTable.map;
    hashTable.map = (Contato **) calloc(sizeof(Contato), maxsize);
    hashTable.len = 0;
    for (int i = 0; i < maxsize-1; i++) {
        if (oldmap[i] != NULL) {
            adicionarContato(oldmap[i]->nome, oldmap[i]->numero);
        }
    }
    free(oldmap);
}

unsigned int hash(char *nome){
    int len = (int) strlen(nome);
    double somatorio = 0;
    for (int i=0; i < len; i++){
        somatorio += pow(nome[i], (len-i));
    }
    int pos = ((int) sqrt(somatorio)) % maxsize;
    return pos >= 0 ? pos : -pos;
}

void adicionarContato(char *nome, char *numero) {
    int index = hash(nome);
    Contato *novoCtt = (Contato *) malloc(sizeof(Contato));
    strcpy(novoCtt->nome, nome);
    strcpy(novoCtt->numero, numero);

    if (hashTable.map[index] == NULL) {
        hashTable.map[index] = novoCtt;
        hashTable.len++;
        printf("Contato adicionado!\n");
    } else {
        if (strcmp(hashTable.map[index]->nome, nome) == 0){
            printf("Não é permitido inserir nomes iguais.\n");
        } else{
            rehash();
            adicionarContato(nome, numero);
        }
    }
    return;
}

void buscarContato(char *nome) {
    
    clock_t start, end;
    start = clock();
    
    int index = hash(nome);
    if (hashTable.map[index] != NULL) {
        end = clock();
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        printf("Contato encontrado: (%d) %s - %s ", index, hashTable.map[index]->nome, hashTable.map[index]->numero);
        printf("(Tempo de busca: %.4f ms)\n", cpu_time_used);
    } else {
        printf("Contato nao encontrado.\n");
    }
    return;
}

void removerContato(char *nome) {
    clock_t start, end;
    start = clock();
    
    int index = hash(nome);
    if (hashTable.map[index] != NULL) {
        Contato *c = hashTable.map[index];
        hashTable.map[index] = NULL;
        hashTable.len--;
        free(c);
        end = clock();
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        printf("Contato removido!\n");
        printf("(Tempo de remoção: %.4f ms)\n", cpu_time_used);
    } else {
        printf("Contato nao encontrado.\n");
    }
    return;
}

void exibirContatos() {
    printf("Contatos: %d/%d\n", hashTable.len, maxsize);
    for (int i = 0; i < maxsize; i++) {
        if (hashTable.map[i] != NULL) {
            printf("(%2d) %s - %s\n", i, hashTable.map[i]->nome, hashTable.map[i]->numero);
        }
    }
}



int main() {
    int opcao;
    
    char nome[51];
    char numero[11];

    clock_t start, end;
    double cpu_time_used;

    inicializarTabela();

    do {
        printf("\nEscolha uma opcao:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opcao: ");
        
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome do contato (sem espaços!): ");
                scanf("%s", nome);
                printf("Numero do contato: ");
                scanf("%s", numero);
                start = clock();
                adicionarContato(nome, numero);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
                printf("(Tempo de insercao: %.4f ms)\n", cpu_time_used);
                break;
            case 2:
                printf("Nome do contato: ");
                scanf("%s", nome);
                buscarContato(nome);
                break;
            case 3:
                printf("Nome do contato: ");
                scanf("%s", nome);
                removerContato(nome);
                break;
            case 4:
                exibirContatos();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}