#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct no {
  struct no* pai;
  struct no* esquerda;
  struct no* direita;
  int valor;
  int altura;
} No;

typedef struct arvore {
  struct no* raiz;
} Arvore;

int contador = 0;

void balanceamento(Arvore*, No*);
int altura(No*);
int fb(No*);
No* rsd(Arvore*, No*);
No* rse(Arvore*, No*);
No* rdd(Arvore*, No*);
No* rde(Arvore*, No*);

int max(int a, int b) {
  contador++;
  return a > b ? a : b;
}

Arvore* criar() {
  Arvore* arvore = malloc(sizeof(Arvore));
  arvore->raiz = NULL;

  return arvore;
}

int vazia(Arvore* arvore) {
  contador++;
  return arvore->raiz == NULL;
}

void adicionar(Arvore* arvore, int valor) {
  No* no = arvore->raiz;

  while (no != NULL) {
    contador++;

    if (valor > no->valor) {
      contador++;

      if (no->direita != NULL) {
        contador++;
        no = no->direita;
      } else {
        contador++;
        break;
      }
    } else {
      contador++;
      if (no->esquerda != NULL) {
        contador++;
        no = no->esquerda;
      } else {
        contador++;
        break;
      }
    }
  }
  contador++;

  No* novo = malloc(sizeof(No));
  novo->valor = valor;
  novo->pai = no;
  novo->esquerda = NULL;
  novo->direita = NULL;
  novo->altura = 1;

  if (no == NULL) {
    contador++;
    arvore->raiz = novo;
  } else {
    contador++;
    if (valor > no->valor) {
      contador++;
      no->direita = novo;
    } else {
      contador++;
      no->esquerda = novo;
    }

    balanceamento(arvore, no);
  }
}

No* localizar(No* no, int valor) {
  while (no != NULL) {
    contador++;
    if (no->valor == valor) {
      contador++;
      return no;
    } else {
      contador++;
    }

    no = valor < no->valor ? no->esquerda : no->direita;
    contador++;
  }
  contador++;

  return NULL;
}

void percorrer(No* no, void (*callback)(int)) {
  if (no != NULL) {
    contador++;
    percorrer(no->esquerda, callback);
    callback(no->valor);
    percorrer(no->direita, callback);
  } else {
    contador++;
  }
}

void visitar(int valor) {
    printf("%d ", valor);
}

void balanceamento(Arvore* arvore, No* no) {
  while (no != NULL) {
    contador++;

    no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
    contador++;
    int fator = fb(no);

    if (fator > 1) {  // árvore mais pesada para esquerda
      contador++;
      // rotação para a direita
      if (fb(no->esquerda) > 0) {
        contador++;
        // printf("RSD %d -> ", no->valor);
        rsd(arvore, no);  // rotação simples a direita, pois o FB do filho
                          // tem sinal igual
      } else {
        contador++;
        // printf("RDD %d -> ", no->valor);
        rdd(arvore, no);  // rotação dupla a direita, pois o FB do filho tem
                          // sinal diferente
      }
    } else if (fator < -1) {  // árvore mais pesada para a direita
      contador++;
      // rotação para a esquerda
      if (fb(no->direita) < 0) {
        contador++;
        // printf("RSE %d -> ", no->valor);
        rse(arvore, no);  // rotação simples a esquerda, pois o FB do
                          // filho tem sinal igual
      } else {
        contador++;
        // printf("RDE %d -> ", no->valor);
        rde(arvore, no);  // rotação dupla a esquerda, pois o FB do filho tem
                          // sinal diferente
      }
    } else {
      contador++;
    }

    no = no->pai;
  }
  contador++;
}

int altura(No* no) {
  contador++;
  return no != NULL ? no->altura : 0;
}

int fb(No* no) {
  int esquerda = 0, direita = 0;

  if (no->esquerda != NULL) {
    contador++;
    esquerda = no->esquerda->altura;
  } else {
    contador++;
  }

  if (no->direita != NULL) {
    contador++;
    direita = no->direita->altura;
  } else {
    contador++;
  }

  return esquerda - direita;
}

No* rse(Arvore* arvore, No* no) {
  No* pai = no->pai;
  No* direita = no->direita;

  contador++;
  if (direita->esquerda != NULL) {
    direita->esquerda->pai = no;
  } else {
    contador++;
  }

  no->direita = direita->esquerda;
  no->pai = direita;

  direita->esquerda = no;
  direita->pai = pai;

  contador++;
  if (pai == NULL) {
    arvore->raiz = direita;
  } else {
    if (pai->esquerda == no) {
      pai->esquerda = direita;
    } else {
      pai->direita = direita;
    }
    contador++;
  }

  no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
  direita->altura =
      max(altura(direita->esquerda), altura(direita->direita)) + 1;

  contador++;

  return direita;
}

No* rsd(Arvore* arvore, No* no) {
  No* pai = no->pai;
  No* esquerda = no->esquerda;

  if (esquerda->direita != NULL) {
    esquerda->direita->pai = no;
  } else {
    contador++;
  }
  contador++;

  no->esquerda = esquerda->direita;
  no->pai = esquerda;

  esquerda->direita = no;
  esquerda->pai = pai;

  contador++;
  if (pai == NULL) {
    arvore->raiz = esquerda;
  } else {
    contador++;
    if (pai->esquerda == no) {
      pai->esquerda = esquerda;
    } else {
      pai->direita = esquerda;
    }
    contador++;
  }

  no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
  esquerda->altura =
      max(altura(esquerda->esquerda), altura(esquerda->direita)) + 1;

  contador++;

  return esquerda;
}

No* rde(Arvore* arvore, No* no) {
  no->direita = rsd(arvore, no->direita);
  return rse(arvore, no);
}

No* rdd(Arvore* arvore, No* no) {
  no->esquerda = rse(arvore, no->esquerda);
  return rsd(arvore, no);
}

No* minValorNo(No* no) {
  No* atual = no;
  while (atual->esquerda != NULL) {
    contador++;
    atual = atual->esquerda;
  }
  contador++;
  return atual;
}

void remover(Arvore* arvore, int valor) {
    No* no = localizar(arvore->raiz, valor);

    contador++;
    if (no == NULL) return;

    // printf("Removendo o valor %d\n", valor);

    while (no != NULL) {
        No* substituto = NULL;
        No* pai = no->pai;

        if (no->esquerda == NULL || no->direita == NULL) {
            substituto = no->esquerda ? no->esquerda : no->direita;

            if (substituto != NULL) {
                substituto->pai = pai;
            }

            if (pai == NULL) {
                arvore->raiz = substituto;
            } else {
                if (no == pai->esquerda) {
                    pai->esquerda = substituto;
                } else {
                    pai->direita = substituto;
                }
            }

            free(no);
            no = NULL;  // End the loop
        } else {
            No* sucessor = minValorNo(no->direita);
            no->valor = sucessor->valor;
            no = sucessor;  // Continue to remove the successor
        }

        balanceamento(arvore, pai);
    }
}

int main() {
  int loops = 30;
  printf("\n[");
  for (int j = 0; j < loops; j++) {
    Arvore* a = criar();
    sleep(1);
    srand(time(0));

    int tamAmostra = 10000;
    int valores[tamAmostra];
    contador = 0;

    for (int i = 0; i < tamAmostra; i++) {
      int operacao = 0;
      int valor = rand() % 10000;
      valores[i] = valor;

      adicionar(a, valor);
    }

    int insertCounter = contador;
    for (int i = 0; i < tamAmostra; i++) {
      remover(a, valores[i]);
    } // essa bosta de remover ta cagando

    if (j == loops - 1) {
      printf("[%d, %d],", insertCounter, contador - insertCounter);
    } else {
      printf("[%d, %d],", insertCounter, contador - insertCounter);
    }
    percorrer(a->raiz,visitar);
  }
  printf("]\n");

  return 0;
}
