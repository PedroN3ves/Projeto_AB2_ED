#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct NoAVL
{
  char *palavra;
  char *significado;
  int altura;
  struct NoAVL *direita, *esquerda;
} NoAVL;

NoAVL *novoNo(char *palavra, char *significado)
{ // cria o nó da árvore e retorna o endereço deste nó para a struct NOAVL
  NoAVL *novo = malloc(sizeof(NoAVL));

  if (novo)
  {
    novo->palavra = strdup(palavra);         // atribui a variavel palavra para o "membro" palavra (strdup ja aloca espaço na memoria para string e copia ela)
    novo->significado = strdup(significado); // atribui a variavel significado para o "membro" significado
    novo->altura = 1;                        // atribui 0 para o "membro" altura
    novo->direita = NULL;                    // atribui NULL para o "membro" direita
    novo->esquerda = NULL;                   // atribui NULL para o "membro" esquerda
  }
  else
  {
    printf("\nERRO!\n"); // caso não consiga armazenar o espaço na memória printa a mensagem
  }
  return novo; // retorna o o novo nó
}

int maior(int a, int b)
{                         // função que verifica qual maior dos 2 inteiros
  return (a > b) ? a : b; // se a > b retorne a, se não retorne b
}

int altura(NoAVL *no)
{ // função que calcula altura usada para rebalanceamento
  if (no == NULL)
  {
    return 0; // caso no seja vazio retornará 0
  }
  else
  {
    return no->altura; // caso contrário retorna nó apontando para altura, retornando a altura da arvore
  }
}

int fatordeBalanceamento(NoAVL *no)
{
  if (no == NULL)
  {
    return 0;
  }

  return (altura(no->esquerda) - altura(no->direita)); // calcula o fator de balanceamento que deve ser no máximo 1 e no mínimo -1
}

NoAVL *rotacaoDireita(NoAVL *r)
{ // r é o no desbalanceado
  NoAVL *y, *f;

  y = r->esquerda; // y ira receber o lado esquerdo de r e irá guardá-lo
  f = r->direita;  // f ira receber o lado direito de r e irá guardá-lo

  y->direita = r;  // a direita de y assumirá o valor de r
  r->esquerda = f; // a esquerda de r assumirá o valor de f

  r->altura = maior(altura(r->esquerda), altura(r->direita)) + 1; // atualiza a altura do no r
  y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1; // atualiza a altura do no y

  return y;
}

NoAVL *rotacaoEsquerda(NoAVL *r)
{
  NoAVL *y, *f;

  y = r->direita;  // y ira receber o lado direito de r e irá guardá-lo
  f = y->esquerda; // f ira receber o lado esquerdo de r e irá guardá-lo

  y->esquerda = r; // a esquerda de y assumirá o valor de r
  r->direita = f;  // a direita de r assumirá o valor de f

  y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1; // atualiza a altura do no y
  r->altura = maior(altura(r->esquerda), altura(r->direita)) + 1; // atualiza a altura do no r

  return y; // return o no balanceado
}

NoAVL *rotacaoDireitaEsquerda(NoAVL *r)
{
  r->direita = rotacaoDireita(r->direita); // realiza uma rotação na direita de r
  return (rotacaoEsquerda(r));             // realiza uma rotação a esquerda de r
}

NoAVL *rotacaoEsquerdaDireita(NoAVL *r)
{
  r->esquerda = rotacaoEsquerda(r->esquerda); // realiza uma rotação na esquerda de r
  return (rotacaoDireita(r));                 // realiza uma rotação na direita de r
}

NoAVL *balancear(NoAVL *raiz)
{
  int fb = fatordeBalanceamento(raiz);

  // rotação a esquerda
  if (fb < -1 && fatordeBalanceamento(raiz->direita) <= 0)
  {
    raiz = rotacaoEsquerda(raiz);
  }
  // rotção a direita
  else if (fb > 1 && fatordeBalanceamento(raiz->esquerda) >= 0)
  {
    raiz = rotacaoDireita(raiz);
  }
  // rotação dupla a esquerda
  else if (fb > 1 && fatordeBalanceamento(raiz->esquerda) < 0)
  {
    raiz = rotacaoEsquerdaDireita(raiz);
  }

  else if (fb < -1 && fatordeBalanceamento(raiz->direita) > 0)
  {
    raiz = rotacaoDireitaEsquerda(raiz);
  }

  return raiz;
}