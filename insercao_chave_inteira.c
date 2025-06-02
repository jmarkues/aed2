#include <stdio.h>
#include <stdlib.h>

typedef enum { LIVRE, OCUPADO } Estado;

typedef struct no {
    int chave;
    int valor;
    Estado estado;
} No;

typedef struct dicionario {
    No *tabela;
    int capacidade;
    int contador;
} Dicionario;

unsigned int hash_inteiro(int chave, int capacidade) {
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
            int chave = dic->tabela[i].chave;
            int valor = dic->tabela[i].valor;
            unsigned int indice = hash_inteiro(chave, nova_capacidade);
            while (nova_tabela[indice].estado == OCUPADO) {
                indice = (indice + 1) % nova_capacidade;
            }
            nova_tabela[indice].chave = chave;
            nova_tabela[indice].valor = valor;
            nova_tabela[indice].estado = OCUPADO;
        }
    }

    free(dic->tabela);
    dic->tabela = nova_tabela;
    dic->capacidade = nova_capacidade;
}

void insere(Dicionario *dic, int chave, int valor) {
    if ((float)(dic->contador + 1) / dic->capacidade >= (2.0/3.0)) {
        redimensiona(dic);
    }

    unsigned int indice = hash_inteiro(chave, dic->capacidade);

    for (int i = 0; i < dic->capacidade; i++) {
        int pos = (indice + i) % dic->capacidade;
        if (dic->tabela[pos].estado == LIVRE || dic->tabela[pos].chave == chave) {
            dic->tabela[pos].chave = chave;
            dic->tabela[pos].valor = valor;
            if (dic->tabela[pos].estado == LIVRE) {
                dic->contador++;
                dic->tabela[pos].estado = OCUPADO;
            }
            return;
        }
    }
}

void imprime(Dicionario *dic) {
    printf("Dicionario:\n");
    for (int i = 0; i < dic->capacidade; i++) {
        if (dic->tabela[i].estado == OCUPADO) {
            printf("Pos %d: Chave = %d, Valor = %d\n", i, dic->tabela[i].chave, dic->tabela[i].valor);
        }
    }
}

void libera(Dicionario *dic) {
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

        int chave, valor;
        printf("Digite a chave (int): ");
        scanf("%d", &chave);
        printf("Digite o valor (int): ");
        scanf("%d", &valor);

        insere(dic, chave, valor);
        imprime(dic);
    }

    libera(dic);
    return 0;
}
