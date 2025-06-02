#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { LIVRE, OCUPADO } Estado;

typedef struct no {
    int chave;
    Estado estado;
} No;

typedef struct dicionario {
    No *tabela;
    int capacidade;
    int contador;
} Dicionario;

unsigned int hash_int(int chave, int capacidade) {
    return chave % capacidade;
}

Dicionario *inicializa(int capacidade) {
    Dicionario *dic = malloc(sizeof(Dicionario));
    dic->capacidade = capacidade;
    dic->contador = 0;
    dic->tabela = malloc(sizeof(No) * capacidade);
    for (int i = 0; i < capacidade; i++) {
        dic->tabela[i].estado = LIVRE;
    }
    return dic;
}

void redimensiona(Dicionario *dic) {
    int nova_capacidade = dic->capacidade * 2;
    No *nova_tabela = malloc(sizeof(No) * nova_capacidade);
    for (int i = 0; i < nova_capacidade; i++) {
        nova_tabela[i].estado = LIVRE;
    }

    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            unsigned int indice = hash_int(dic->tabela[i].chave, nova_capacidade);
            while (nova_tabela[indice].estado == OCUPADO) {
                indice = (indice + 1) % nova_capacidade;
            }
            nova_tabela[indice].chave = dic->tabela[i].chave;
            nova_tabela[indice].estado = OCUPADO;
        }
    }

    free(dic->tabela);
    dic->tabela = nova_tabela;
    dic->capacidade = nova_capacidade;
}

void insere(Dicionario *dic, int chave) {
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0/3.0)) {
        redimensiona(dic);
    }

    unsigned int indice = hash_int(chave, dic->capacidade);

    for (int i = 0; i < dic->capacidade; i++) {
        int pos = (indice + i) % dic->capacidade;
        if (dic->tabela[pos].estado == LIVRE || dic->tabela[pos].chave == chave) {
            if (dic->tabela[pos].estado == LIVRE) {
                dic->tabela[pos].chave = chave;
                dic->contador++;
            }
            dic->tabela[pos].estado = OCUPADO;
            return;
        }
    }
}

void insertion_sort(int *arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void libera(Dicionario *dic) {
    free(dic->tabela);
    free(dic);
}

int main() {
    for (int N = 20000; N <= 200000; N += 20000) {
        Dicionario *dic = inicializa(8);

        for (int i = 0; i < N; i++) {
            insere(dic, rand() % 1000000);
        }

        // Coletar chaves em vetor auxiliar
        int *chaves = malloc(dic->contador * sizeof(int));
        int idx = 0;
        for (int i = 0; i < dic->capacidade; i++) {
            if (dic->tabela[i].estado == OCUPADO) {
                chaves[idx++] = dic->tabela[i].chave;
            }
        }

        clock_t inicio = clock();

        insertion_sort(chaves, dic->contador);

        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("N = %d, Tempo de ordenacao = %.6f segundos\n", N, tempo);

        free(chaves);
        libera(dic);
    }

    return 0;
}
