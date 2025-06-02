#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum { LIVRE, OCUPADO } Estado;

typedef struct no {
    char *chave;
    Estado estado;
} No;

typedef struct dicionario {
    No *tabela;
    int capacidade;
    int contador;
} Dicionario;

unsigned int hash_string(const char *str, int capacidade) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % capacidade;
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
            unsigned int indice = hash_string(dic->tabela[i].chave, nova_capacidade);
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

void insere(Dicionario *dic, const char *chave) {
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0 / 3.0)) {
        redimensiona(dic);
    }

    unsigned int indice = hash_string(chave, dic->capacidade);

    for (int i = 0; i < dic->capacidade; i++) {
        int pos = (indice + i) % dic->capacidade;
        if (dic->tabela[pos].estado == LIVRE || strcmp(dic->tabela[pos].chave, chave) == 0) {
            if (dic->tabela[pos].estado == LIVRE) {
                dic->tabela[pos].chave = strdup(chave);
                dic->contador++;
            }
            dic->tabela[pos].estado = OCUPADO;
            return;
        }
    }
}

void insertion_sort(char **arr, int n) {
    for (int i = 1; i < n; i++) {
        char *key = arr[i];
        int j = i - 1;
        while (j >= 0 && strcmp(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void libera(Dicionario *dic) {
    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            free(dic->tabela[i].chave);
        }
    }
    free(dic->tabela);
    free(dic);
}

char *gera_string_aleatoria(int tamanho) {
    char *str = malloc((tamanho + 1) * sizeof(char));
    const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < tamanho; i++) {
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    str[tamanho] = '\0';
    return str;
}

int main() {
    srand(time(NULL));
    for (int N = 20000; N <= 200000; N += 20000) {
        Dicionario *dic = inicializa(8);

        // Inserção de N strings aleatórias
        for (int i = 0; i < N; i++) {
            char *s = gera_string_aleatoria(10);
            insere(dic, s);
            free(s);  // liberar a string temporária
        }

        // Coletar chaves para ordenar
        char **chaves = malloc(dic->contador * sizeof(char *));
        int idx = 0;
        for (int i = 0; i < dic->capacidade; i++) {
            if (dic->tabela[i].estado == OCUPADO) {
                chaves[idx++] = dic->tabela[i].chave;
            }
        }

        // Medir tempo de ordenação com Insertion Sort
        clock_t inicio = clock();
        insertion_sort(chaves, dic->contador);
        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("N = %d, Tempo de ordenacao (insertion) = %.6f segundos\n", N, tempo);

        free(chaves);
        libera(dic);
    }
    return 0;
}
