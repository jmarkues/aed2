#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sentinela = -10000000;

typedef enum { LIVRE, OCUPADO, REMOVIDO } Estado;

typedef struct no {
  int chave_inteiro;
  char *chave_string;
  char *string;
  int inteiro;
  Estado estado;
} No;

typedef struct dicionario {
  No *tabela;
  int contador;
  int capacidade;
} Dicionario;

Dicionario *inicializa(int capacidade) {
  Dicionario *dic = malloc(sizeof(Dicionario));
  dic->contador = 0;
  dic->capacidade = capacidade;
  dic->tabela = malloc(capacidade * sizeof(No));
  for (int i = 0; i < capacidade; i++) {
    dic->tabela[i].estado = LIVRE;
    dic->tabela[i].chave_string = NULL;
    dic->tabela[i].string = NULL;
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
  for (int i = 0; i < dic->capacidade; i++) {
    int pos = (indice + i) % dic->capacidade;
    No *no = &dic->tabela[pos];
    
    if (no->estado == LIVRE || no->estado == REMOVIDO) {
      no->chave_string = chave_string;
      no->chave_inteiro = sentinela;

      if (eh_string_valida(valor_string)) {
        no->string = valor_string;
        no->inteiro = sentinela;
      } else {
        no->inteiro = valor_inteiro;
        no->string = NULL;
      }
      no->estado = OCUPADO;
      dic->contador++;
      return;
    } else if (no->chave_string && strcmp(no->chave_string, chave_string) == 0) {
      if (eh_string_valida(valor_string)) {
        no->string = valor_string;
        no->inteiro = sentinela;
      } else {
        no->inteiro = valor_inteiro;
        no->string = NULL;
      }
      return;
    }
  }
}

void insere_atualiza_inteiro(Dicionario *dic, int chave_inteira, char *valor_string, int valor_inteiro) {
  unsigned int indice = funcao_hash_inteiro(chave_inteira, dic->capacidade);
  for (int i = 0; i < dic->capacidade; i++) {
    int pos = (indice + i) % dic->capacidade;
    No *no = &dic->tabela[pos];
    
    if (no->estado == LIVRE || no->estado == REMOVIDO) {
      no->chave_inteiro = chave_inteira;
      no->chave_string = NULL;

      if (eh_string_valida(valor_string)) {
        no->string = valor_string;
        no->inteiro = sentinela;
      } else {
        no->inteiro = valor_inteiro;
        no->string = NULL;
      }
      no->estado = OCUPADO;
      dic->contador++;
      return;
    } else if (no->chave_inteiro == chave_inteira && no->estado == OCUPADO) {
      if (eh_string_valida(valor_string)) {
        no->string = valor_string;
        no->inteiro = sentinela;
      } else {
        no->inteiro = valor_inteiro;
        no->string = NULL;
      }
      return;
    }
  }
}

void remover(Dicionario *dic, int chave_int, char *chave_string) {
  int indice;
  if (chave_int != sentinela) {
    indice = funcao_hash_inteiro(chave_int, dic->capacidade);
    for (int i = 0; i < dic->capacidade; i++) {
      int pos = (indice + i) % dic->capacidade;
      No *no = &dic->tabela[pos];
      if (no->estado == LIVRE) break;
      if (no->estado == OCUPADO && no->chave_inteiro == chave_int) {
        if (no->chave_string) free(no->chave_string);
        if (no->string) free(no->string);
        no->estado = REMOVIDO;
        dic->contador--;
        return;
      }
    }
  } else {
    indice = funcao_hash_string(chave_string, dic->capacidade);
    for (int i = 0; i < dic->capacidade; i++) {
      int pos = (indice + i) % dic->capacidade;
      No *no = &dic->tabela[pos];
      if (no->estado == LIVRE) break;
      if (no->estado == OCUPADO && no->chave_string && strcmp(no->chave_string, chave_string) == 0) {
        if (no->chave_string) free(no->chave_string);
        if (no->string) free(no->string);
        no->estado = REMOVIDO;
        dic->contador--;
        return;
      }
    }
  }
  printf("chave nao existe\n");
}

int busca(Dicionario *dic, int chave_int, char *chave_string) {
  int indice;
  if (chave_int != sentinela) {
    indice = funcao_hash_inteiro(chave_int, dic->capacidade);
    for (int i = 0; i < dic->capacidade; i++) {
      int pos = (indice + i) % dic->capacidade;
      No *no = &dic->tabela[pos];
      if (no->estado == LIVRE) break;
      if (no->estado == OCUPADO && no->chave_inteiro == chave_int) return 1;
    }
  } else {
    indice = funcao_hash_string(chave_string, dic->capacidade);
    for (int i = 0; i < dic->capacidade; i++) {
      int pos = (indice + i) % dic->capacidade;
      No *no = &dic->tabela[pos];
      if (no->estado == LIVRE) break;
      if (no->estado == OCUPADO && no->chave_string && strcmp(no->chave_string, chave_string) == 0) return 1;
    }
  }
  return 0;
}

void imprime(Dicionario *dic) {
  for (int i = 0; i < dic->capacidade; i++) {
    No *no = &dic->tabela[i];
    if (no->estado == OCUPADO) {
      printf("Posição: %d ", i);
      if (!no->chave_string) {
        printf("Chave: %d -> ", no->chave_inteiro);
        if (no->string) {
          printf("Valor: %s", no->string);
        } else {
          printf("Valor: %d", no->inteiro);
        }
      } else {
        printf("Chave: %s -> ", no->chave_string);
        if (no->string) {
          printf("Valor: %s", no->string);
        } else {
          printf("Valor: %d", no->inteiro);
        }
      }
      printf("\n");
    }
  }
}

void libera_dicionario(Dicionario *dic) {
  for (int i = 0; i < dic->capacidade; i++) {
    No *no = &dic->tabela[i];
    if (no->estado == OCUPADO) {
      if (no->chave_string) free(no->chave_string);
      if (no->string) free(no->string);
    }
  }
  free(dic->tabela);
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
      printf("chave encontrada\n");
    } else printf("chave não encontrada\n");
  } else {
    char valor_str[100];
    printf("Digite a string: ");
    scanf("%s", valor_str);
    char *chave_buscada = strdup(valor_str);
    if (busca(dic, sentinela, chave_buscada)) {
      printf("chave encontrada\n");
    } else printf("chave não encontrada\n");
    free(chave_buscada);
  }

  imprime(dic);
  libera_dicionario(dic);

  return 0;
}
