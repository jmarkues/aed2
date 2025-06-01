#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void insertionSort(int *v, int n) {
  for (int i = 1; i < n; i++) {
    int chave = v[i]; int j = i - 1;
    while (j >= 0 && chave < v[j]) {
      v[j + 1] = v[j];
      j--;
    }
    v[j + 1] = chave;
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
    int n = 1000000;
    int *v = geraVetor(0, 1000000, n);
    
    t = clock();
    insertionSort(v, n);
    t = clock() - t;
    
    printf("Tempo de execucao: %lf segundos\n", (double)t / CLOCKS_PER_SEC);
    
    free(v);
    return 0;
}