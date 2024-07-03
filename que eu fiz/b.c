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
    no->filhos[i] = NULL;
    contador++;
  }

  return no;
}

void percorreArvore(No* no) {
  contador++;
  if (no != NULL) {
    contador++;
    for (int i = 0; i < no->total; i++) {
      percorreArvore(no->filhos[i]);  // visita o filho a esquerda

      printf("%d ", no->chaves[i]);
      contador++;
    }

    percorreArvore(no->filhos[no->total]);  // visita ultimo filho (direita)
  }
}

int pesquisaBinaria(No* no, int chave) {
  int inicio = 0, fim = no->total - 1, meio;

  contador++;
  while (inicio <= fim) {
    meio = (inicio + fim) / 2;

    contador++;
    if (no->chaves[meio] == chave) {
      contador++;
      return meio;  // encontrou
    } else if (no->chaves[meio] > chave) {
      fim = meio - 1;
    } else {
      contador++;
      inicio = meio + 1;
    }
    contador++;
  }
  return inicio;  // não encontrou
}

int localizaChave(ArvoreB* arvore, int chave) {
  No* no = arvore->raiz;

  contador++;
  while (no != NULL) {
    int i = pesquisaBinaria(no, chave);

    contador++;
    if (i < no->total && no->chaves[i] == chave) {
      return 1;  // encontrou
    } else {
      no = no->filhos[i];
    }
    contador++;
  }

  printf("Valor %d não encontrado\n", chave);
  return 0;  // não encontrou
}

No* localizaNo(ArvoreB* arvore, int chave) {
  No* no = arvore->raiz;

  contador++;
  while (no != NULL) {
    int i = pesquisaBinaria(no, chave);

    contador++;
    if (no->filhos[i] == NULL)
      return no;  // encontrou nó
    else
      no = no->filhos[i];
    contador++;
  }

  printf("Valor %d não encontrado\n", chave);
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

    contador++;
    if (novo->filhos[novo->total] != NULL)
      novo->filhos[novo->total]->pai = novo;

    novo->total++;
    contador++;
  }

  novo->filhos[novo->total] = no->filhos[no->total];
  contador++;
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

    contador++;
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

  // Localiza a posição do no atual no pai
  while (pai->filhos[posicaoNo] != no && posicaoNo <= pai->total) {
    posicaoNo++;
  }

  No* irmaoEsquerdo = posicaoNo > 0 ? pai->filhos[posicaoNo - 1] : NULL;
  No* irmaoDireito = posicaoNo < pai->total ? pai->filhos[posicaoNo + 1] : NULL;

  if (irmaoEsquerdo != NULL && irmaoEsquerdo->total > arvore->ordem) {
    // Redistribui um elemento do irmão esquerdo
    for (int i = no->total; i > 0; i--) {
      no->chaves[i] = no->chaves[i - 1];
      no->filhos[i + 1] = no->filhos[i];
    }
    no->filhos[1] = no->filhos[0];
    no->chaves[0] = pai->chaves[posicaoNo - 1];
    no->filhos[0] = irmaoEsquerdo->filhos[irmaoEsquerdo->total];

    if (no->filhos[0] != NULL) {
      no->filhos[0]->pai = no;
    }

    pai->chaves[posicaoNo - 1] =
        irmaoEsquerdo->chaves[irmaoEsquerdo->total - 1];
    irmaoEsquerdo->total--;
    no->total++;
  } else if (irmaoDireito != NULL && irmaoDireito->total > arvore->ordem) {
    // Redistribui um elemento do irmão direito
    no->chaves[no->total] = pai->chaves[posicaoNo];
    no->filhos[no->total + 1] = irmaoDireito->filhos[0];

    if (no->filhos[no->total + 1] != NULL) {
      no->filhos[no->total + 1]->pai = no;
    }

    pai->chaves[posicaoNo] = irmaoDireito->chaves[0];

    for (int i = 0; i < irmaoDireito->total - 1; i++) {
      irmaoDireito->chaves[i] = irmaoDireito->chaves[i + 1];
      irmaoDireito->filhos[i] = irmaoDireito->filhos[i + 1];
    }
    irmaoDireito->filhos[irmaoDireito->total - 1] =
        irmaoDireito->filhos[irmaoDireito->total];
    irmaoDireito->total--;
    no->total++;
  } else {
    // Fusão com o irmão, se não for possível redistribuir
    if (irmaoEsquerdo != NULL) {
      // Fundir com o irmão esquerdo
      irmaoEsquerdo->chaves[irmaoEsquerdo->total] = pai->chaves[posicaoNo - 1];
      irmaoEsquerdo->total++;

      for (int i = 0; i < no->total; i++) {
        irmaoEsquerdo->chaves[irmaoEsquerdo->total] = no->chaves[i];
        irmaoEsquerdo->filhos[irmaoEsquerdo->total] = no->filhos[i];
        if (irmaoEsquerdo->filhos[irmaoEsquerdo->total] != NULL) {
          irmaoEsquerdo->filhos[irmaoEsquerdo->total]->pai = irmaoEsquerdo;
        }
        irmaoEsquerdo->total++;
      }
      irmaoEsquerdo->filhos[irmaoEsquerdo->total] = no->filhos[no->total];
      if (irmaoEsquerdo->filhos[irmaoEsquerdo->total] != NULL) {
        irmaoEsquerdo->filhos[irmaoEsquerdo->total]->pai = irmaoEsquerdo;
      }

      for (int i = posicaoNo - 1; i < pai->total - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
      }
      pai->total--;

      free(no->chaves);
      free(no->filhos);
      free(no);

      if (pai->total == 0) {
        if (pai->pai == NULL) {
          arvore->raiz = irmaoEsquerdo;
          irmaoEsquerdo->pai = NULL;
          free(pai->chaves);
          free(pai->filhos);
          free(pai);
        } else {
          balancearNo(arvore, pai);
        }
      }
    } else if (irmaoDireito != NULL) {
      // Fundir com o irmão direito
      no->chaves[no->total] = pai->chaves[posicaoNo];
      no->total++;

      for (int i = 0; i < irmaoDireito->total; i++) {
        no->chaves[no->total] = irmaoDireito->chaves[i];
        no->filhos[no->total] = irmaoDireito->filhos[i];
        if (no->filhos[no->total] != NULL) {
          no->filhos[no->total]->pai = no;
        }
        no->total++;
      }
      no->filhos[no->total] = irmaoDireito->filhos[irmaoDireito->total];
      if (no->filhos[no->total] != NULL) {
        no->filhos[no->total]->pai = no;
      }

      for (int i = posicaoNo; i < pai->total - 1; i++) {
        pai->chaves[i] = pai->chaves[i + 1];
        pai->filhos[i + 1] = pai->filhos[i + 2];
      }
      pai->total--;

      free(irmaoDireito->chaves);
      free(irmaoDireito->filhos);
      free(irmaoDireito);

      if (pai->total == 0) {
        if (pai->pai == NULL) {
          arvore->raiz = no;
          no->pai = NULL;
          free(pai->chaves);
          free(pai->filhos);
          free(pai);
        } else {
          balancearNo(arvore, pai);
        }
      }
    }
  }
}

int main() {
  int loops = 30;
  printf("\n[");
  for (int j = 0; j < loops; j++) {
    ArvoreB* arvore = criaArvore(5);
    sleep(1);
    srand(time(0));

    int tamAmostra = 10000;
    int valores[tamAmostra];
    contador = 0;

    for (int i = 0; i < tamAmostra; i++) {
      int valor = rand() % 100000;
      valores[i] = valor;

      adicionaChave(arvore, valor);
    }

    for (int i = 0; i < tamAmostra; i++) {
      removerChave(arvore, valores[i]);
    }  // esse lixo não funciona em arvores de outras ordens alem de 1

    if (j == loops - 1) {
      printf("%d", contador);
    } else {
      printf("%d, ", contador);
    }

    // percorreArvore(arvore->raiz);
  }
  printf("]\n");
}
