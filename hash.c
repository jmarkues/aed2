#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sentinela = -10000000;

typedef struct no {
  int chave_inteiro;
  char *chave_string;
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

unsigned int funcao_hash_string(const char* string, int capacidade) {
  unsigned long hash = 5381;
  int c;
  while ((c = *string++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % capacidade;
}

unsigned int funcao_hash_inteiro(int inteiro, int capacidade) {
  return (unsigned int)inteiro % capacidade;
}

int eh_string_valida(char *string) {
  return string != NULL;
}

void insere_atualiza_string(Dicionario *dic, char *chave_string, char *valor_string, int valor_inteiro) {
  unsigned int indice = funcao_hash_string(chave_string, dic->capacidade);
  No *aux = dic->lista[indice];

  while (aux) {
    if (strcmp(chave_string, aux->chave_string) == 0) {
      if (eh_string_valida(valor_string)) {
        aux->string = valor_string;
        aux->inteiro = sentinela;
      } else {
        aux->inteiro = valor_inteiro;
        aux->string = NULL;
      }
      return;
    }
    aux = aux->prox;
  }

  No *novo = malloc(sizeof(No));
  novo->chave_string = chave_string;
  novo->chave_inteiro = sentinela;

  if (eh_string_valida(valor_string)) {
    novo->string = valor_string;
    novo->inteiro = sentinela;
  } else {
    novo->inteiro = valor_inteiro;
    novo->string = NULL;
  }

  novo->prox = dic->lista[indice];
  dic->lista[indice] = novo;
  dic->contador++;
}

void insere_atualiza_inteiro(Dicionario *dic, int chave_inteira, char *valor_string, int valor_inteiro) {
  unsigned int indice = funcao_hash_inteiro(chave_inteira, dic->capacidade);
  No *aux = dic->lista[indice];

  while (aux) {
    if (aux->chave_inteiro == chave_inteira) {
      if (eh_string_valida(valor_string)) {
        aux->string = valor_string;
        aux->inteiro = sentinela;
      } else {
        aux->inteiro = valor_inteiro;
        aux->string = NULL;
      }
      return;
    }
    aux = aux->prox;
  }

  No *novo = malloc(sizeof(No));
  novo->chave_inteiro = chave_inteira;
  novo->chave_string = NULL;

  if (eh_string_valida(valor_string)) {
    novo->string = valor_string;
    novo->inteiro = sentinela;
  } else {
    novo->inteiro = valor_inteiro;
    novo->string = NULL;
  }

  novo->prox = dic->lista[indice];
  dic->lista[indice] = novo;
  dic->contador++;
}

void remover(Dicionario *dic, int chave_int, char *chave_string) {
  int indice;
  No *aux, *anterior = NULL;

  if (chave_int != sentinela) {
    indice = funcao_hash_inteiro(chave_int, dic->capacidade);
    aux = dic->lista[indice];
    while (aux && aux->chave_inteiro != chave_int)
      anterior = aux, aux = aux->prox;
  } else {
    indice = funcao_hash_string(chave_string, dic->capacidade);
    aux = dic->lista[indice];
    while (aux && strcmp(aux->chave_string, chave_string) != 0)
      anterior = aux, aux = aux->prox;
  }

  if (!aux) {
    printf("chave nao existe\n");
    return;
  }

  if (anterior) anterior->prox = aux->prox;
  else dic->lista[indice] = aux->prox;

  if (aux->chave_string) free(aux->chave_string);
  if (aux->string) free(aux->string);
  free(aux);
  dic->contador--;
}

int busca(Dicionario *dic, int chave_int, char *chave_string) {
  // CHAVE INT
  if (chave_int != sentinela) {
    int indice_inteiro = funcao_hash_inteiro(chave_int, dic->capacidade);
    No *aux_inteiro = dic->lista[indice_inteiro];
    while (aux_inteiro != NULL && aux_inteiro->chave_inteiro != chave_int) aux_inteiro = aux_inteiro->prox;
    if (aux_inteiro) return 1;
    else return 0;
  } else { // CHAVE STRING
      int indice_string = funcao_hash_string(chave_string, dic->capacidade);
      No *aux_string = dic->lista[indice_string];
      while (aux_string != NULL && strcmp(aux_string->chave_string, chave_string) != 0) aux_string = aux_string->prox;
      if (aux_string) return 1;
      else return 0;
  }
}

void imprime(Dicionario *dic) {
  for (int i = 0; i < dic->capacidade; i++) {
    No *aux = dic->lista[i];
    while (aux) {
      printf("Posição: %d ", i);
      if (!aux->chave_string) {
        printf("Chave: %d -> ", aux->chave_inteiro);
        if (aux->string) {
          printf("Valor: %s", aux->string);
        } else {
          printf("Valor: %d", aux->inteiro);
        }
      } else {
        printf("Chave: %s -> ", aux->chave_string);
        if (aux->string) {
          printf("Valor: %s", aux->string);
        } else {
          printf("Valor: %d", aux->inteiro);
        }
      }
      printf("\n");
      aux = aux->prox;
    }
  }
}

void libera_dicionario(Dicionario *dic) {
  for (int i = 0; i < dic->capacidade; i++) {
    No *aux = dic->lista[i];
    while (aux) {
      No *prox = aux->prox;
      if (aux->chave_string) free(aux->chave_string);
      if (aux->string) free(aux->string);
      free(aux);
      aux = prox;
    }
  }
  free(dic->lista);
  free(dic);
}

int main() {
  int N, quant;
  scanf("%d", &N);
  scanf("%d", &quant);

  Dicionario *dic = inicializa(N);

  for (int i = 0; i < quant; i++) {
    printf("Digite 1 se a chave for número: ");
    int chave_tipo;
    scanf("%d", &chave_tipo);

    if (chave_tipo) {
      int chave_int;
      printf("Digite a chave_int: ");
      scanf("%d", &chave_int);

      printf("Digite 1 se o valor for número: ");
      int valor_tipo;
      scanf("%d", &valor_tipo);

      if (valor_tipo) {
        int valor_int;
        printf("Digite o número: ");
        scanf("%d", &valor_int);
        insere_atualiza_inteiro(dic, chave_int, NULL, valor_int);
      } else {
        char valor_str[100];
        printf("Digite a string: ");
        scanf("%s", valor_str);
        char *valor_copia = strdup(valor_str);
        insere_atualiza_inteiro(dic, chave_int, valor_copia, sentinela);
      }
    } else {
      char chave_str[100];
      printf("Digite a chave_string: ");
      scanf("%s", chave_str);
      char *chave_copia = strdup(chave_str);

      printf("Digite 1 se o valor for número: ");
      int valor_tipo;
      scanf("%d", &valor_tipo);

      if (valor_tipo) {
        int valor_int;
        printf("Digite o número: ");
        scanf("%d", &valor_int);
        insere_atualiza_string(dic, chave_copia, NULL, valor_int);
      } else {
        char valor_str[100];
        printf("Digite a string: ");
        scanf("%s", valor_str);
        char *valor_copia = strdup(valor_str);
        insere_atualiza_string(dic, chave_copia, valor_copia, sentinela);
      }
    }
  }
  
  printf("Digite 1 se chave for um numero inteiro: ");
  int tipo_chave_buscada; scanf("%d", &tipo_chave_buscada);
  
  if (tipo_chave_buscada) {
      printf("Digite o inteiro: ");
      int chave_buscada; scanf("%d", &chave_buscada);
      if (busca(dic, chave_buscada, NULL)) {
          printf("chave encontrada");
      } else printf("chave não encontrada");
  } else {
      char valor_str[100];
      printf("Digite a string: ");
      scanf("%s", valor_str);
      char *chave_buscada = strdup(valor_str);
      if (busca(dic, sentinela, chave_buscada)) {
          printf("chave encontrada");
      } else printf("chave não encontrada");
  }
  imprime(dic);
  libera_dicionario(dic);

  return 0;
}