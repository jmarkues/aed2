#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void countingSort(int *v, int maior, int n) {
    int *contador = calloc(maior + 1, sizeof(int));
    int *saida = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        contador[v[i]]++;
    }

    int idx = 0;
    for (int i = 0; i <= maior; i++) {
        while (contador[i] > 0) {
            saida[idx++] = i;
            contador[i]--;
        }
    }

    for (int i = 0; i < n; i++) {
        v[i] = saida[i];
    }

    free(contador);
    free(saida);
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
    int n = 100000000;
    int *v = geraVetor(0, 1000000, n);
    
    t = clock();
    countingSort(v, 1000000, n-1);
    t = clock() - t;
    
    printf("Tempo de execucao: %lf segundos\n", (double)t / CLOCKS_PER_SEC);
    
    free(v);
    return 0;
}