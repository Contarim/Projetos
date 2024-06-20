#include <stdio.h>
#include <stdlib.h>

struct Arv {
  int chave;
  struct Arv *esquerda;
  struct Arv *direita;
  int altura;
};

struct lista {
  int valor;
  struct lista *prox;
};
typedef struct lista Lista;

Lista *insereLista(Lista *l, int i);
void imprimeLista(Lista *l);

int altura(struct Arv *N) {
  if (N == NULL)
    return 0;
  return N->altura;
}

int max(int a, int b) { return (a > b) ? a : b; }

struct Arv *novaArv(int chave) {
  struct Arv *Arv = (struct Arv *)malloc(sizeof(struct Arv));
  Arv->chave = chave;
  Arv->esquerda = NULL;
  Arv->direita = NULL;
  Arv->altura = 1;
  return (Arv);
}

struct Arv *rotacaoDireita(struct Arv *y) {
  struct Arv *x = y->esquerda;
  struct Arv *T2 = x->direita;

  x->direita = y;
  y->esquerda = T2;

  y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;
  x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;

  return x;
}

struct Arv *rotacaoEsquerda(struct Arv *x) {
  struct Arv *y = x->direita;
  struct Arv *T2 = y->esquerda;

  // Executa a rotacao
  y->esquerda = x;
  x->direita = T2;

  // atualiza a altura
  x->altura = max(altura(x->esquerda), altura(x->direita)) + 1;
  y->altura = max(altura(y->esquerda), altura(y->direita)) + 1;

  return y;
}

int equilibrar(struct Arv *N) {
  if (N == NULL)
    return 0;
  return altura(N->esquerda) - altura(N->direita);
}

struct Arv *insere(struct Arv *Arv, int chave) {

  if (Arv == NULL)
    return (novaArv(chave));

  if (chave < Arv->chave)
    Arv->esquerda = insere(Arv->esquerda, chave);
  else if (chave > Arv->chave)
    Arv->direita = insere(Arv->direita, chave);
  else
    return Arv;

  Arv->altura = 1 + max(altura(Arv->esquerda), altura(Arv->direita));

  int balanco = equilibrar(Arv);

  if (balanco > 1 && chave < Arv->esquerda->chave)
    return rotacaoDireita(Arv);

  if (balanco < -1 && chave > Arv->direita->chave)
    return rotacaoEsquerda(Arv);

  if (balanco > 1 && chave > Arv->esquerda->chave) {
    Arv->esquerda = rotacaoEsquerda(Arv->esquerda);
    return rotacaoDireita(Arv);
  }

  if (balanco < -1 && chave < Arv->direita->chave) {
    Arv->direita = rotacaoDireita(Arv->direita);
    return rotacaoEsquerda(Arv);
  }

  return Arv;
}

struct Arv *valorMinArv(struct Arv *Arv) {
  struct Arv *atual = Arv;

  while (atual->esquerda != NULL)
    atual = atual->esquerda;

  return atual;
}

struct Arv *removeArv(struct Arv *raiz, int chave) {

  if (raiz == NULL)
    return raiz;

  if (chave < raiz->chave)
    raiz->esquerda = removeArv(raiz->esquerda, chave);

  else if (chave > raiz->chave)
    raiz->direita = removeArv(raiz->direita, chave);

  else {

    if ((raiz->esquerda == NULL) || (raiz->direita == NULL)) {
      struct Arv *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

      if (temp == NULL) {
        temp = raiz;
        raiz = NULL;
      } else
        *raiz = *temp;

      free(temp);
    } else {

      struct Arv *temp = valorMinArv(raiz->direita);

      raiz->chave = temp->chave;

      raiz->direita = removeArv(raiz->direita, temp->chave);
    }
  }

  if (raiz == NULL)
    return raiz;

  raiz->altura = 1 + max(altura(raiz->esquerda), altura(raiz->direita));

  int balanco = equilibrar(raiz);

  if (balanco > 1 && equilibrar(raiz->esquerda) >= 0)
    return rotacaoDireita(raiz);

  if (balanco > 1 && equilibrar(raiz->esquerda) < 0) {
    raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
    return rotacaoDireita(raiz);
  }

  if (balanco < -1 && equilibrar(raiz->direita) <= 0)
    return rotacaoEsquerda(raiz);

  if (balanco < -1 && equilibrar(raiz->direita) > 0) {
    raiz->direita = rotacaoDireita(raiz->direita);
    return rotacaoEsquerda(raiz);
  }

  return raiz;
}

// Função para buscar um elemento na árvore AVL
struct Arv *buscaAVL(struct Arv *raiz, int chave) {
  if (raiz == NULL) {
    return NULL;
  }

  if (chave < raiz->chave) {
    buscaAVL(raiz->esquerda, chave);
  } else if (chave > raiz->chave) {
    buscaAVL(raiz->direita, chave);
  } else {
    return raiz;
  }
}

Lista* preOrdem(struct Arv *raiz, Lista *list) {
  if (raiz != NULL) {
    list = insereLista(list, raiz->chave);
    list = preOrdem(raiz->esquerda, list);
    list = preOrdem(raiz->direita, list);

  }

  return list;
}

Lista* inOrdem(struct Arv *raiz, Lista* list) {
  if (raiz != NULL) {
    list = inOrdem(raiz->esquerda, list);
    list = insereLista(list, raiz->chave);
    list = inOrdem(raiz->direita, list);

  }

  return list;
}

Lista* posOrdem(struct Arv *raiz, Lista *list) {
  if (raiz != NULL) {
    list = posOrdem(raiz->esquerda, list);
    list = posOrdem(raiz->direita, list);
    list = insereLista(list, raiz->chave);

  }

  return list;
}

Lista *inicializaLista();
// Lista* insereLista(Lista* l, int i);
void imprimeLista(Lista *l);

/* Função de inicialização: retorna uma lista vazia */
Lista *inicializaLista(void) { return NULL; }

/* Função de inserção no início: retorna a lista atualizada */
Lista *insereLista(Lista *l, int i) {
  Lista *novo = (Lista *)malloc(sizeof(Lista));
  novo->valor = i;
  novo->prox = l;
  return novo;
}

/* Função para imprimir os valores dos elementos da lista */
void imprimeLista(Lista *l) {
  Lista *p;
  printf("Lista: ");
  for (p = l; p != NULL; p = p->prox)
    printf("%d ", p->valor);
  printf("\n");
}

int main() {
  struct Arv *raiz = NULL;
  int opc, numero, result;
  Lista *list = NULL;

  do {
    printf("::: SELECIONE UMA OPCAO :::\n\n");
    printf("( 1 ) INSERIR\n");
    printf("( 2 ) REMOVER\n");
    printf("( 3 ) BUSCA\n");
    printf("( 4 ) COPIAR PARA LISTA PRE-ORDEM\n");
    printf("( 5 ) COPIAR PARA LISTA IN-ORDEM\n");
    printf("( 6 ) COPIAR PARA LISTA POS-ORDEM\n");
    printf("( 7 ) IMPRIMIR LISTA\n");
    printf("( 8 ) SAIR\n\n");
    printf("==> ");
    scanf("%d", &opc);
    //system("cls");

    switch (opc) {

    case 1:
      printf("Digite o numero a ser armazenado: ");
      scanf(" %d", &numero);
      raiz = insere(raiz, numero);
      system("cls");
      printf("Numero armazenado com sucesso!\n\n");
      break;

    case 2:
      printf("Digite o numero a ser removido: ");
      scanf(" %d", &numero);
      raiz = removeArv(raiz, numero);
      system("cls");
      printf("Numero removido com sucesso!\n\n");
      break;

    case 3:
      printf("Digite o numero a ser buscado: ");
      scanf("%d", &numero);
      result = buscaAVL(raiz, numero);
      system ("cls");
      if (result != NULL ){
        printf ("Numero encontrado na lista!\n\n");
      } else {
        printf("Numero nao encontrado na lista\n\n");

      }
      printf("\n");
      break;

    case 4:
      system ("cls");
      printf("Copiado para a lista Pre-Ordem!\n");

      list = NULL;

      list = preOrdem(raiz,list);
      printf("\n\n");
      break;

    case 5:
      system ("cls");
      printf("Copiado para a lista In-Ordem!\n");

      list = NULL;

      list = inOrdem(raiz, list);
      printf("\n\n");
      break;

    case 6:
      system ("cls");
      printf("Copiado para a lista Pos-Ordem!\n");

      list = NULL;

      list = posOrdem(raiz, list);
      printf("\n\n");
      break;

    case 7:
      system ("cls");
      printf("\n");
      imprimeLista(list);
      printf("\n\n");
      break;
    }
  } while (opc != 8);

  return 0;
}
