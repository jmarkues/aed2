#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { LIVRE, OCUPADO } Estado;

typedef struct no {
    char *chave;
    char *valor;
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
        dic->tabela[i].valor = NULL;
    }
    return dic;
}

void redimensiona(Dicionario *dic) {
    int nova_capacidade = dic->capacidade * 2;
    No *nova_tabela = malloc(sizeof(No) * nova_capacidade);
    for (int i = 0; i < nova_capacidade; i++) {
        nova_tabela[i].estado = LIVRE;
        nova_tabela[i].chave = NULL;
        nova_tabela[i].valor = NULL;
    }

    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            unsigned int indice = hash_string(dic->tabela[i].chave, nova_capacidade);
            while (nova_tabela[indice].estado == OCUPADO) {
                indice = (indice + 1) % nova_capacidade;
            }
            nova_tabela[indice].chave = dic->tabela[i].chave;
            nova_tabela[indice].valor = dic->tabela[i].valor;
        }
    }

    free(dic->tabela);
    dic->tabela = nova_tabela;
    dic->capacidade = nova_capacidade;
}

void insere(Dicionario *dic, char *chave, char *valor) {
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0/3.0)) {
        redimensiona(dic);
    }

    unsigned int indice = hash_string(chave, dic->capacidade);

    for (int i = 0; i < dic->capacidade; i++) {
        int pos = (indice + i) % dic->capacidade;
        if (dic->tabela[pos].estado == LIVRE || strcmp(dic->tabela[pos].chave, chave) == 0) {
            if (dic->tabela[pos].estado == LIVRE) {
                dic->tabela[pos].chave = strdup(chave);
                dic->contador++;
            } else {
                free(dic->tabela[pos].valor);
            }
            dic->tabela[pos].valor = strdup(valor);
            dic->tabela[pos].estado = OCUPADO;
            return;
        }
    }
}

void imprime(Dicionario *dic) {
    printf("Dicionario:\n");
    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            printf("Pos %d: Chave = %s, Valor = %s\n", i, dic->tabela[i].chave, dic->tabela[i].valor);
        }
    }
}

void libera(Dicionario *dic) {
    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            free(dic->tabela[i].chave);
            free(dic->tabela[i].valor);
        }
    }
    free(dic->tabela);
    free(dic);
}

int main() {
    Dicionario *dic = inicializa(8);

    while (1) {
        int opcao;
        printf("Deseja inserir uma chave? (1 = sim, 0 = nao): ");
        scanf("%d", &opcao);
        if (!opcao) break;

        char chave[100], valor[100];
        printf("Digite a chave (string): ");
        scanf("%s", chave);
        printf("Digite o valor (string): ");
        scanf("%s", valor);

        insere(dic, chave, valor);
        imprime(dic);
    }

    libera(dic);
    return 0;
}
