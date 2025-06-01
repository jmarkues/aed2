#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>

void merge(int *v, int inicio, int meio, int fim) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    int *esq = malloc((n1 + 1) * sizeof(int));
    int *dir = malloc((n2 + 1) * sizeof(int));

    for (int i = 0; i < n1; i++) esq[i] = v[inicio + i];
    for (int j = 0; j < n2; j++) dir[j] = v[meio + 1 + j];

    esq[n1] = INT_MAX;
    dir[n2] = INT_MAX;

    int i = 0, j = 0;
    for (int k = inicio; k <= fim; k++) {
        if (esq[i] <= dir[j]) {
            v[k] = esq[i++];
        } else {
            v[k] = dir[j++];
        }
    }

    free(esq);
    free(dir);
}

void mergeSort(int *v, int inicio, int fim) {
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSort(v, inicio, meio);
        mergeSort(v, meio + 1, fim);
        merge(v, inicio, meio, fim);
    }
}

int *geraVetor(int menor, int maior, int n) {
    int *v = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        v[i] = rand() % (maior - menor + 1) + menor;
    }
    return v;
}

int main() {
    clock_t t;
    int n = 10000000;
    int *v = geraVetor(0, 1000000, n);
    
    t = clock();
    mergeSort(v, 0, n-1);
    t = clock() - t;
    
    printf("Tempo de execucao: %lf segundos\n", (double)t / CLOCKS_PER_SEC);
    
    free(v);
    return 0;
}
