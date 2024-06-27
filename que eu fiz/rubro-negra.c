#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum coloracao { Vermelho, Preto };
typedef enum coloracao Cor;

typedef struct no {
  struct no* pai;
  struct no* esquerda;
  struct no* direita;
  Cor cor;
  int valor;
} No;

typedef struct arvore {
  struct no* raiz;
  struct no* nulo;
} Arvore;

No* criarNo(Arvore*, No*, int);
void balancear(Arvore*, No*);
void rotacionarEsquerda(Arvore*, No*);
void rotacionarDireita(Arvore*, No*);

Arvore* criar();
int vazia(Arvore*);
No* adicionar(Arvore*, int);
No* localizar(Arvore* arvore, int valor);

int contador = 0;

Arvore* criar() {
  Arvore* arvore = malloc(sizeof(Arvore));
  arvore->nulo = NULL;
  arvore->raiz = NULL;

  arvore->nulo = criarNo(arvore, NULL, 0);
  arvore->nulo->cor = Preto;

  return arvore;
}

int vazia(Arvore* arvore) {
  contador++;
  return arvore->raiz == NULL;
}

No* criarNo(Arvore* arvore, No* pai, int valor) {
  No* no = malloc(sizeof(No));

  no->pai = pai;
  no->valor = valor;
  no->direita = arvore->nulo;
  no->esquerda = arvore->nulo;

  return no;
}

No* adicionarNo(Arvore* arvore, No* no, int valor) {
  if (valor > no->valor) {
    contador++;
    if (no->direita == arvore->nulo) {
      no->direita = criarNo(arvore, no, valor);
      no->direita->cor = Vermelho;

      contador++;
      return no->direita;
    } else {
      contador++;
      return adicionarNo(arvore, no->direita, valor);
    }
  } else {
    contador++;
    if (no->esquerda == arvore->nulo) {
      no->esquerda = criarNo(arvore, no, valor);
      no->esquerda->cor = Vermelho;

      contador++;
      return no->esquerda;
    } else {
      contador++;
      return adicionarNo(arvore, no->esquerda, valor);
    }
  }
}

No* adicionar(Arvore* arvore, int valor) {
  if (vazia(arvore)) {
    arvore->raiz = criarNo(arvore, arvore->nulo, valor);
    arvore->raiz->cor = Preto;

    return arvore->raiz;
  } else {
    No* no = adicionarNo(arvore, arvore->raiz, valor);
    balancear(arvore, no);

    return no;
  }
}

No* localizar(Arvore* arvore, int valor) {
  if (!vazia(arvore)) {
    No* no = arvore->raiz;

    contador++;
    while (no != arvore->nulo) {
      contador++;
      if (no->valor == valor) {
        contador++;
        return no;
      } else {
        contador++;
        no = valor < no->valor ? no->esquerda : no->direita;
      }
    }
  }

  return NULL;
}

void percorrerProfundidadeInOrder(Arvore* arvore, No* no,
                                  void (*callback)(int)) {
  if (no != arvore->nulo) {
    percorrerProfundidadeInOrder(arvore, no->esquerda, callback);
    callback(no->valor);
    percorrerProfundidadeInOrder(arvore, no->direita, callback);
  }
}

void percorrerProfundidadePreOrder(Arvore* arvore, No* no,
                                   void (*callback)(int)) {
  if (no != arvore->nulo) {
    callback(no->valor);
    percorrerProfundidadePreOrder(arvore, no->esquerda, callback);
    percorrerProfundidadePreOrder(arvore, no->direita, callback);
  }
}

void percorrerProfundidadePosOrder(Arvore* arvore, No* no,
                                   void(callback)(int)) {
  if (no != arvore->nulo) {
    percorrerProfundidadePosOrder(arvore, no->esquerda, callback);
    percorrerProfundidadePosOrder(arvore, no->direita, callback);
    callback(no->valor);
  }
}

void visitar(int valor) { printf("%d ", valor); }

void balancear(Arvore* arvore, No* no) {
  contador++;
  while (no->pai->cor == Vermelho) {
    contador++;
    if (no->pai == no->pai->pai->esquerda) {
      No* tio = no->pai->pai->direita;

      if (tio->cor == Vermelho) {
        tio->cor = Preto;  // Caso 1
        no->pai->cor = Preto;

        no->pai->pai->cor = Vermelho;  // Caso 1
        no = no->pai->pai;             // Caso 1
      } else {
        if (no == no->pai->direita) {
          no = no->pai;                    // Caso 2
          rotacionarEsquerda(arvore, no);  // Caso 2
        } else {
          no->pai->cor = Preto;
          no->pai->pai->cor = Vermelho;             // Caso 3
          rotacionarDireita(arvore, no->pai->pai);  // Caso 3
        }
        contador++;
      }
      contador++;
    } else {
      No* tio = no->pai->pai->esquerda;

      if (tio->cor == Vermelho) {
        tio->cor = Preto;  // Caso 1
        no->pai->cor = Preto;

        no->pai->pai->cor = Vermelho;  // Caso 1
        no = no->pai->pai;             // Caso 1
      } else {
        if (no == no->pai->esquerda) {
          no = no->pai;                   // Caso 2
          rotacionarDireita(arvore, no);  // Caso 2
        } else {
          no->pai->cor = Preto;
          no->pai->pai->cor = Vermelho;              // Caso 3
          rotacionarEsquerda(arvore, no->pai->pai);  // Caso 3
        }
        contador++;
      }
      contador++;
    }
    contador++;
  }
  arvore->raiz->cor = Preto;  // Conserta possível quebra regra 2
}

void rotacionarEsquerda(Arvore* arvore, No* no) {
  No* direita = no->direita;
  no->direita = direita->esquerda;

  if (direita->esquerda != arvore->nulo) {
    direita->esquerda->pai = no;
  }
  contador++;

  direita->pai = no->pai;

  if (no->pai == arvore->nulo) {
    contador++;
    arvore->raiz = direita;
  } else if (no == no->pai->esquerda) {
    contador += 2;
    no->pai->esquerda = direita;
  } else {
    contador += 2;
    no->pai->direita = direita;
  }

  direita->esquerda = no;
  no->pai = direita;
}

void rotacionarDireita(Arvore* arvore, No* no) {
  No* esquerda = no->esquerda;
  no->esquerda = esquerda->direita;

  if (esquerda->direita != arvore->nulo) {
    esquerda->direita->pai = no;
  }
  contador++;

  esquerda->pai = no->pai;

  if (no->pai == arvore->nulo) {
    contador++;
    arvore->raiz = esquerda;
  } else if (no == no->pai->esquerda) {
    contador += 2;
    no->pai->esquerda = esquerda;
  } else {
    contador += 2;
    no->pai->direita = esquerda;
  }

  esquerda->direita = no;
  no->pai = esquerda;
}

int main() {
  for (int j = 0; j < 30; j++) {
    Arvore* a = criar();
    sleep(1);
    srand(time(0));

    contador = 0;

    for (int i = 0; i < 10000; i++) {
      int operacao = 0;
      int valor = rand() % 100000;

      switch (operacao) {
        case 0:
          adicionar(a, valor);
          break;
      }
    }

    printf("\nNumero de operacoes: %d\n", contador);
  }

  return 0;
}
