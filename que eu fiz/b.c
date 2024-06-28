#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct no {
  int total;
  int* chaves;
  struct no** filhos;
  struct no* pai;
} No;

typedef struct arvoreB {
  No* raiz;
  int ordem;
} ArvoreB;

ArvoreB* criaArvore(int);
No* criaNo(ArvoreB*);
void percorreArvore(No*);
int pesquisaBinaria(No*, int);
No* localizaNo(ArvoreB*, int);
void adicionaChaveNo(No*, No*, int);
int transbordo(ArvoreB*, No*);
No* divideNo(ArvoreB*, No*);
void adicionaChaveRecursivo(ArvoreB*, No*, No*, int);
void adicionaChave(ArvoreB*, int);
void removerChave(ArvoreB*, int);
void removerChaveNo(ArvoreB*, No*, int);
void balancearNo(ArvoreB*, No*);

ArvoreB* criaArvore(int ordem) {
  ArvoreB* a = malloc(sizeof(ArvoreB));
  a->ordem = ordem;
  a->raiz = criaNo(a);

  return a;
}

int contador = 0;

No* criaNo(ArvoreB* arvore) {
  int max = arvore->ordem * 2;
  No* no = malloc(sizeof(No));

  no->pai = NULL;

  no->chaves = malloc(sizeof(int) * (max + 1));
  no->filhos = malloc(sizeof(No) * (max + 2));
  no->total = 0;

  contador++;
  for (int i = 0; i < max + 2; i++) {
    contador++;
    no->filhos[i] = NULL;
  }

  return no;
}

void percorreArvore(No* no) {
  if (no != NULL) {
    for (int i = 0; i < no->total; i++) {
      percorreArvore(no->filhos[i]);  // visita o filho a esquerda

      printf("%d ", no->chaves[i]);
    }

    percorreArvore(no->filhos[no->total]);  // visita ultimo filho (direita)
  }
}

int pesquisaBinaria(No* no, int chave) {
  int inicio = 0, fim = no->total - 1, meio;

  contador++;
  while (inicio <= fim) {
    contador++;

    meio = (inicio + fim) / 2;

    if (no->chaves[meio] == chave) {
      contador++;
      return meio;  // encontrou
    } else if (no->chaves[meio] > chave) {
      contador += 2;
      fim = meio - 1;
    } else {
      contador += 2;
      inicio = meio + 1;
    }
  }
  return inicio;  // não encontrou
}

No* localizaNo(ArvoreB* arvore, int chave) {
  No* no = arvore->raiz;

  contador++;
  while (no != NULL) {
    contador++;

    int i = pesquisaBinaria(no, chave);

    contador++;
    if (no->filhos[i] == NULL)
      return no;  // encontrou nó
    else
      no = no->filhos[i];
  }

  return NULL;  // não encontrou nenhum nó
}

void adicionaChaveNo(No* no, No* novo, int chave) {
  int i = pesquisaBinaria(no, chave);

  contador++;

  for (int j = no->total - 1; j >= i; j--) {
    contador++;
    no->chaves[j + 1] = no->chaves[j];
    no->filhos[j + 2] = no->filhos[j + 1];
  }

  no->chaves[i] = chave;
  no->filhos[i + 1] = novo;

  no->total++;
}

int transbordo(ArvoreB* arvore, No* no) {
  contador++;

  return no->total > arvore->ordem * 2;
}

No* divideNo(ArvoreB* arvore, No* no) {
  int meio = no->total / 2;
  No* novo = criaNo(arvore);
  novo->pai = no->pai;

  contador++;

  for (int i = meio + 1; i < no->total; i++) {
    novo->filhos[novo->total] = no->filhos[i];
    novo->chaves[novo->total] = no->chaves[i];
    if (novo->filhos[novo->total] != NULL)
      novo->filhos[novo->total]->pai = novo;

    novo->total++;
  }

  novo->filhos[novo->total] = no->filhos[no->total];
  if (novo->filhos[novo->total] != NULL) novo->filhos[novo->total]->pai = novo;
  no->total = meio;
  return novo;
}

void adicionaChaveRecursivo(ArvoreB* arvore, No* no, No* novo, int chave) {
  adicionaChaveNo(no, novo, chave);

  contador++;
  if (transbordo(arvore, no)) {
    int promovido = no->chaves[arvore->ordem];
    No* novo = divideNo(arvore, no);

    if (no->pai == NULL) {
      No* pai = criaNo(arvore);
      pai->filhos[0] = no;
      adicionaChaveNo(pai, novo, promovido);

      no->pai = pai;
      novo->pai = pai;
      arvore->raiz = pai;
    } else {
      adicionaChaveRecursivo(arvore, no->pai, novo, promovido);
    }
    contador++;
  }
}

void adicionaChave(ArvoreB* arvore, int chave) {
  No* no = localizaNo(arvore, chave);

  adicionaChaveRecursivo(arvore, no, NULL, chave);
}

void removerChave(ArvoreB* arvore, int chave) {
  No* no = localizaNo(arvore, chave);

  removerChaveNo(arvore, no, chave);
}

void removerChaveNo(ArvoreB* arvore, No* no, int chave) {
  contador++;
  if (no == NULL) return;

  int i = pesquisaBinaria(no, chave);

  contador++;
  if (i < no->total && no->chaves[i] == chave) {
    contador++;
    for (int j = i; j < no->total - 1; j++) {
      contador++;
      no->chaves[j] = no->chaves[j + 1];
      no->filhos[j + 1] = no->filhos[j + 2];
    }
    no->total--;

    // Verificar se é necessário realizar balanceamento
    if (no->total < arvore->ordem && no->pai != NULL) {
      balancearNo(arvore, no);
    }
    contador++;
  } else {
    removerChaveNo(arvore, no->filhos[i], chave);
  }
}

void balancearNo(ArvoreB* arvore, No* no) {
  No* pai = no->pai;
  int posicaoNo = 0;

  contador++;
  while (pai->filhos[posicaoNo] != no) {
    posicaoNo++;
    contador++;
  }

  No* irmaoEsquerdo = posicaoNo > 0 ? pai->filhos[posicaoNo - 1] : NULL;
  contador++;
  No* irmaoDireito = posicaoNo < pai->total ? pai->filhos[posicaoNo + 1] : NULL;
  contador++;

  // Tenta a redistribuição
  if (irmaoEsquerdo != NULL && irmaoEsquerdo->total > arvore->ordem) {
    contador++;
    for (int i = no->total; i > 0; i--) {
      no->chaves[i] = no->chaves[i - 1];
      no->filhos[i + 1] = no->filhos[i];
      contador++;
    }
    contador++;
    no->filhos[1] = no->filhos[0];
    no->chaves[0] = pai->chaves[posicaoNo - 1];
    no->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->total];

    contador++;
    if (no->filhos[0] != NULL) {
      no->filhos[0]->pai = no;
    }

    pai->chaves[posicaoNo - 1] =
        irmaoEsquerdo->chaves[irmaoEsquerdo->total - 1];
    irmaoEsquerdo->total--;
    no->total++;
  } else if (irmaoDireito != NULL && irmaoDireito->total > arvore->ordem) {
    contador++;
    no->chaves[no->total] = pai->chaves[posicaoNo];
    no->filhos[no->total + 1] = irmaoDireito->filhos[0];

    contador++;
    if (no->filhos[no->total + 1] != NULL) {
      no->filhos[no->total + 1]->pai = no;
    }

    pai->chaves[posicaoNo] = irmaoDireito->chaves[0];

    for (int i = 0; i < irmaoDireito->total - 1; i++) {
      irmaoDireito->chaves[i] = irmaoDireito->chaves[i + 1];
      irmaoDireito->filhos[i] = irmaoDireito->filhos[i + 1];
      contador++;
    }
    contador++;

    irmaoDireito->filhos[irmaoDireito->total - 1] =
        irmaoDireito->filhos[irmaoDireito->total];
    irmaoDireito->total--;
    no->total++;
  } else {
    // Fusão
    contador++;
    if (irmaoEsquerdo != NULL) {
      irmaoEsquerdo->chaves[irmaoEsquerdo->total] = pai->chaves[posicaoNo - 1];
      irmaoEsquerdo->total++;
      contador++;

      for (int i = 0; i < no->total; i++) {
        contador++;
        irmaoEsquerdo->chaves[irmaoEsquerdo->total] = no->chaves[i];
        irmaoEsquerdo->filhos[irmaoEsquerdo->total] = no->filhos[i];
        if (irmaoEsquerdo->filhos[irmaoEsquerdo->total] != NULL) {
          irmaoEsquerdo->filhos[irmaoEsquerdo->total]->pai = irmaoEsquerdo;
        }
        contador++;
        irmaoEsquerdo->total++;
      }
      contador++;

      irmaoEsquerdo->filhos[irmaoEsquerdo->total] = no->filhos[no->total];
      if (irmaoEsquerdo->filhos[irmaoEsquerdo->total] != NULL) {
        irmaoEsquerdo->filhos[irmaoEsquerdo->total]->pai = irmaoEsquerdo;
      }
      contador++;

      for (int i = posicaoNo - 1; i < pai->total - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
        contador++;
      }
      contador++;

      pai->total--;

      contador++;
      if (pai->total < arvore->ordem && pai->pai != NULL) {
        balancearNo(arvore, pai);
      }
    } else if (irmaoDireito != NULL) {
      contador++;  // Incrementando contador para a comparação
      no->chaves[no->total] = pai->chaves[posicaoNo];
      no->total++;
      contador++;  // Incrementando contador

      for (int i = 0; i < irmaoDireito->total; i++) {
        contador++;
        no->chaves[no->total] = irmaoDireito->chaves[i];
        no->filhos[no->total] = irmaoDireito->filhos[i];
        if (no->filhos[no->total] != NULL) {
          no->filhos[no->total]->pai = no;
        }
        no->total++;
        contador++;
      }
      contador++;

      no->filhos[no->total] = irmaoDireito->filhos[irmaoDireito->total];
      if (no->filhos[no->total] != NULL) {
        no->filhos[no->total]->pai = no;
      }
      contador++;

      for (int i = posicaoNo; i < pai->total - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
        contador++;  // Incrementando contador a cada iteração
      }
      contador++;  // Incrementando contador para a comparação final do for

      pai->total--;

      if (pai->total < arvore->ordem && pai->pai != NULL) {
        balancearNo(arvore, pai);
      }
      contador++;
    }
    contador++;
  }

  if (pai->total == 0) {
    arvore->raiz = no;
    no->pai = NULL;
    free(pai->chaves);
    free(pai->filhos);
    free(pai);
  }
  contador++;
}

int main() {
  for (int j = 0; j < 1; j++) {
    ArvoreB* arvore = criaArvore(1);
    sleep(1);
    srand(time(0));

    int tamAmostra = 10000;
    int valores[tamAmostra];
    contador = 0;

    for (int i = 0; i < tamAmostra; i++) {
      int operacao = 0;
      int valor = rand() % 100000;
      valores[i] = valor;

      adicionaChave(arvore, valor);
    }

    // for (int i = 0; i < tamAmostra; i++) {
    //   removerChave(arvore, valores[i]);
    // }

    // percorreArvore(arvore->raiz);
    printf("\nNumero de operacoes Arvore B: %d\n", contador);
  }
}
