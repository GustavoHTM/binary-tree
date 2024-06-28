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
void balancearRemocao(Arvore*, No*);
void rotacionarEsquerda(Arvore*, No*);
void rotacionarDireita(Arvore*, No*);
Arvore* criar();
int vazia(Arvore*);
No* adicionar(Arvore*, int);
No* localizar(Arvore* arvore, int valor);
No* substituir(Arvore*, No*);
No* minimo(Arvore*, No*);
void remover(Arvore*, int);
void removerNo(Arvore*, No*);
void transplante(Arvore*, No*, No*);

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

void balancearRemocao(Arvore* arvore, No* no) {
  while (no != arvore->raiz && no->cor == Preto) {
    if (no == no->pai->esquerda) {
      No* irmao = no->pai->direita;
      if (irmao->cor == Vermelho) {
        irmao->cor = Preto;
        no->pai->cor = Vermelho;
        rotacionarEsquerda(arvore, no->pai);
        irmao = no->pai->direita;
      }
      if (irmao->esquerda->cor == Preto && irmao->direita->cor == Preto) {
        irmao->cor = Vermelho;
        no = no->pai;
      } else {
        if (irmao->direita->cor == Preto) {
          irmao->esquerda->cor = Preto;
          irmao->cor = Vermelho;
          rotacionarDireita(arvore, irmao);
          irmao = no->pai->direita;
        }
        irmao->cor = no->pai->cor;
        no->pai->cor = Preto;
        irmao->direita->cor = Preto;
        rotacionarEsquerda(arvore, no->pai);
        no = arvore->raiz;
      }
    } else {
      No* irmao = no->pai->esquerda;
      if (irmao->cor == Vermelho) {
        irmao->cor = Preto;
        no->pai->cor = Vermelho;
        rotacionarDireita(arvore, no->pai);
        irmao = no->pai->esquerda;
      }
      if (irmao->direita->cor == Preto && irmao->esquerda->cor == Preto) {
        irmao->cor = Vermelho;
        no = no->pai;
      } else {
        if (irmao->esquerda->cor == Preto) {
          irmao->direita->cor = Preto;
          irmao->cor = Vermelho;
          rotacionarEsquerda(arvore, irmao);
          irmao = no->pai->esquerda;
        }
        irmao->cor = no->pai->cor;
        no->pai->cor = Preto;
        irmao->esquerda->cor = Preto;
        rotacionarDireita(arvore, no->pai);
        no = arvore->raiz;
      }
    }
  }
  no->cor = Preto;
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

No* minimo(Arvore* arvore, No* no) {
  contador++;
  while (no->esquerda != arvore->nulo) {
    contador++;
    no = no->esquerda;
  }
  return no;
}

No* substituir(Arvore* arvore, No* no) {
  contador++;
  if (no->esquerda != arvore->nulo && no->direita != arvore->nulo) {
    return minimo(arvore, no->direita);
  }
  contador++;
  if (no->esquerda == arvore->nulo && no->direita == arvore->nulo) {
    return arvore->nulo;
  }
  contador++;
  if (no->esquerda != arvore->nulo) {
    return no->esquerda;
  }
  return no->direita;
}

void transplante(Arvore* arvore, No* u, No* v) {
  if (u->pai == arvore->nulo) {
    contador++;
    arvore->raiz = v;
  } else if (u == u->pai->esquerda) {
    contador += 2;
    u->pai->esquerda = v;
  } else {
    contador += 2;
    u->pai->direita = v;
  }
  v->pai = u->pai;
}

void remover(Arvore* arvore, int valor) {
  No* no = localizar(arvore, valor);
  contador++;
  if (no == arvore->nulo) {
    return;
  }
  removerNo(arvore, no);
}

void removerNo(Arvore* arvore, No* no) {
  No* y = no;
  No* x;
  Cor corOriginal = y->cor;
  if (no->esquerda == arvore->nulo) {
    contador++;
    x = no->direita;
    transplante(arvore, no, no->direita);
  } else if (no->direita == arvore->nulo) {
    contador += 2;
    x = no->esquerda;
    transplante(arvore, no, no->esquerda);
  } else {
    contador += 2;
    y = minimo(arvore, no->direita);
    corOriginal = y->cor;
    x = y->direita;
    contador++;
    if (y->pai == no) {
      x->pai = y;
    } else {
      transplante(arvore, y, y->direita);
      y->direita = no->direita;
      y->direita->pai = y;
    }
    transplante(arvore, no, y);
    y->esquerda = no->esquerda;
    y->esquerda->pai = y;
    y->cor = no->cor;
  }
  free(no);
  contador++;
  if (corOriginal == Preto) {
    balancearRemocao(arvore, x);
  }
}

int main() {
  for (int j = 0; j < 1; j++) {
    Arvore* a = criar();
    sleep(1);
    srand(time(0));

    int tamAmostra = 10000;
    int valores[tamAmostra];
    contador = 0;

    for (int i = 0; i < tamAmostra; i++) {
      int operacao = 0;
      int valor = rand() % 100000;
      valores[i] = valor;

      adicionar(a, valor);
    }

    // for (int i = 0; i < tamAmostra; i++) {
    //   remover(a, valores[i]);
    // }

    printf("\nNumero de operacoes Arvore Rubro-negra: %d\n", contador);
  }

  return 0;
}
