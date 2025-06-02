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
    return (unsigned int)chave % capacidade;
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
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0 / 3.0)) {
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

void libera(Dicionario *dic) {
    free(dic->tabela);
    free(dic);
}

// Merge Sort
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l)/2;
        merge_sort(arr, l, m);
        merge_sort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

int* extrai_chaves(Dicionario *dic) {
    int *chaves = malloc(dic->contador * sizeof(int));
    int j = 0;
    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            chaves[j++] = dic->tabela[i].chave;
        }
    }
    return chaves;
}

int main() {
    const int passo = 20000;
    const int max_chaves = 1000000;

    for (int n = passo; n <= max_chaves; n += passo) {
        Dicionario *dic = inicializa(n);

        for (int i = 0; i < n; i++) {
            insere(dic, rand());
        }

        int *chaves = extrai_chaves(dic);

        clock_t inicio = clock();
        merge_sort(chaves, 0, dic->contador - 1);
        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        printf("N = %d, Tempo de ordenacao = %.6f segundos\n", n, tempo);

        free(chaves);
        libera(dic);
    }

    return 0;
}
