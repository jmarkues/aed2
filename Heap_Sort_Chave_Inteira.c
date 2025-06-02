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
    return (unsigned int)(chave) % capacidade;
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
                dic->contador++;
            }
            dic->tabela[pos].chave = chave;
            dic->tabela[pos].estado = OCUPADO;
            return;
        }
    }
}

// Função auxiliar para o Heapify
void heapify(int arr[], int n, int i) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && arr[esq] > arr[maior])
        maior = esq;

    if (dir < n && arr[dir] > arr[maior])
        maior = dir;

    if (maior != i) {
        int temp = arr[i];
        arr[i] = arr[maior];
        arr[maior] = temp;

        heapify(arr, n, maior);
    }
}

void heap_sort(int arr[], int n) {
    // Construção do heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extração dos elementos do heap
    for (int i = n - 1; i >= 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

void libera(Dicionario *dic) {
    free(dic->tabela);
    free(dic);
}

int main() {
    srand(time(NULL));

    for (int N = 20000; N <= 1000000; N += 20000) {
        Dicionario *dic = inicializa(8);

        // Inserção de N inteiros aleatórios
        for (int i = 0; i < N; i++) {
            int chave = rand();
            insere(dic, chave);
        }

        // Coleta das chaves para ordenar
        int *chaves = malloc(dic->contador * sizeof(int));
        int idx = 0;
        for (int i = 0; i < dic->capacidade; i++) {
            if (dic->tabela[i].estado == OCUPADO) {
                chaves[idx++] = dic->tabela[i].chave;
            }
        }

        // Medição do tempo de ordenação com Heap Sort
        clock_t inicio = clock();
        heap_sort(chaves, dic->contador);
        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("N = %d, Tempo de ordenacao (heap) = %.6f segundos\n", N, tempo);

        free(chaves);
        libera(dic);
    }

    return 0;
}
