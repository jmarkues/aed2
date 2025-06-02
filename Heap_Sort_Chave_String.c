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
        dic->tabela[i].chave = NULL;
    }
    return dic;
}

void redimensiona(Dicionario *dic) {
    int nova_capacidade = dic->capacidade * 2;
    No *nova_tabela = malloc(sizeof(No) * nova_capacidade);
    for (int i = 0; i < nova_capacidade; i++) {
        nova_tabela[i].estado = LIVRE;
        nova_tabela[i].chave = NULL;
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

void insere(Dicionario *dic, char *chave) {
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0 / 3.0)) {
        redimensiona(dic);
    }

    unsigned int indice = hash_string(chave, dic->capacidade);

    for (int i = 0; i < dic->capacidade; i++) {
        int pos = (indice + i) % dic->capacidade;
        if (dic->tabela[pos].estado == LIVRE) {
            dic->tabela[pos].chave = strdup(chave);
            dic->tabela[pos].estado = OCUPADO;
            dic->contador++;
            return;
        }
    }
}

// Função auxiliar para o Heapify (strings)
void heapify(char **arr, int n, int i) {
    int maior = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && strcmp(arr[esq], arr[maior]) > 0)
        maior = esq;

    if (dir < n && strcmp(arr[dir], arr[maior]) > 0)
        maior = dir;

    if (maior != i) {
        char *temp = arr[i];
        arr[i] = arr[maior];
        arr[maior] = temp;

        heapify(arr, n, maior);
    }
}

void heap_sort(char **arr, int n) {
    // Construção do heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // Extração
    for (int i = n - 1; i >= 0; i--) {
        char *temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
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

// Gera uma string aleatória
void gera_string_aleatoria(char *str, int tamanho) {
    static const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < tamanho - 1; i++) {
        str[i] = alfabeto[rand() % (sizeof(alfabeto) - 1)];
    }
    str[tamanho - 1] = '\0';
}

int main() {
    srand(time(NULL));

    for (int N = 20000; N <= 1000000; N += 20000) {
        Dicionario *dic = inicializa(8);

        // Inserção de N strings aleatórias
        for (int i = 0; i < N; i++) {
            char str[10];
            gera_string_aleatoria(str, 10);
            insere(dic, str);
        }

        // Coleta das chaves
        char **chaves = malloc(dic->contador * sizeof(char*));
        int idx = 0;
        for (int i = 0; i < dic->capacidade; i++) {
            if (dic->tabela[i].estado == OCUPADO) {
                chaves[idx++] = dic->tabela[i].chave;
            }
        }

        // Medição do tempo de ordenação
        clock_t inicio = clock();
        heap_sort(chaves, dic->contador);
        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

        printf("N = %d, Tempo de ordenacao (heap, string) = %.6f segundos\n", N, tempo);

        free(chaves);
        libera(dic);
    }

    return 0;
}
