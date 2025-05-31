#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sentinela = -10000000;

typedef struct no {
  char *chave;
  char *string;
  int inteiro;
  struct no* prox;
} No;

typedef struct dicionario {
  No **lista;
  int contador;
  int capacidade;
} Dicionario;

Dicionario *inicializa(int capacidade) {
  Dicionario *dic = malloc(sizeof(Dicionario));
  dic->contador = 0;
  dic->capacidade = capacidade;
  
  dic->lista = malloc(capacidade * sizeof(No*));
  
  for (int i = 0; i < capacidade; i++) {
    dic->lista[i] = NULL;
  }
  
  return dic;
}

unsigned int funcao_hash(const char* string, int capacidade) {
    unsigned long hash = 5381;
    int c;

    while ((c = *string++)) {
        hash = ((hash << 5) + hash) + c; 
    }

    return hash % capacidade;
}

int eh_caractere(char *string) {
  int eh_caractere = 1;
  if (!string) {
    eh_caractere = 0;
  }
  return eh_caractere;
}

void insere_atualiza(Dicionario *dic, char *chave, char *string, int inteiro) {
  unsigned int indice = funcao_hash(chave, dic->capacidade);
  No *aux = dic->lista[indice];
  
  while (aux) {
    if (strcmp(chave, aux->chave) == 0) {
      if (eh_caractere(string)) {
      aux->string = string;
      aux->inteiro = sentinela;
      } else {
      aux->inteiro = inteiro;
      aux->string = NULL;
      }
      return;
    }
    aux = aux->prox;
  }
  
  No *novo = malloc(sizeof(No));
  novo->chave = chave;
  
  if (eh_caractere(string)) {
    novo->string = string;
    novo->inteiro = sentinela;
  } else {
    novo->inteiro = inteiro;
    novo->string = NULL;
  }
  
  novo->prox = dic->lista[indice];
  dic->lista[indice] = novo;
  
  dic->contador++;
}

void imprime(Dicionario *dic) {
  for (int i = 0; i < dic->capacidade; i++) {
    No *aux = dic->lista[i];
    while (aux) {
      printf("Posição: %d ", i);
      if (eh_caractere(aux->string)) {
        printf("%s", aux->chave);
        printf("%s ", aux->string);
      } else {
        printf("%s", aux->chave);
        printf("%d ", aux->inteiro);
      }
      printf(" - ");
      aux = aux->prox;
    }
  }
}

int main() {
  int N; scanf("%d", &N);
  int quant; scanf("%d", &quant);

  Dicionario *dic = inicializa(N);

  for (int i = 0; i < quant; i++) {
    char chave[100];
    printf("Digite a chave: ");
    scanf("%s", chave);
    char* chave_copia = strdup(chave);

    printf("Digite 1 se o valor for número: ");
    int eh_numero; scanf("%d", &eh_numero);

    if (eh_numero) {
      int inteiro;
      printf("Digite o número: ");
      scanf("%d", &inteiro);
      insere_atualiza(dic, chave_copia, NULL, inteiro);
    } else {
      char valor_str[100];
      printf("Digite a string: ");
      scanf("%s", valor_str);
      char* valor_copia = strdup(valor_str);
      insere_atualiza(dic, chave_copia, valor_copia, sentinela);
    }
  }

  imprime(dic);

  return 0;
}