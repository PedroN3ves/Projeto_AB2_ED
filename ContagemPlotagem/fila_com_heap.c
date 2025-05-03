#include <stdio.h>    // biblioteca padrão para entrada e saída (ex: printf, fopen)
#include <stdlib.h>   // biblioteca para alocação de memória (ex: malloc, calloc, free)

// define uma estrutura que representa uma fila de prioridade implementada como um heap
typedef struct
{
  int *dados;       // ponteiro para o vetor que armazena os elementos do heap
  int tamanho;      // número atual de elementos no heap
  int capacidade;   // tamanho máximo que o vetor suporta antes de crescer
} EstruturaHeap;

// função que cria e inicializa uma nova estrutura de heap
EstruturaHeap *criarHeap()
{
  EstruturaHeap *h = malloc(sizeof(EstruturaHeap));         // aloca memória para a estrutura do heap
  h->capacidade = 8;                                        // define uma capacidade inicial (pode crescer depois)
  h->tamanho = 0;                                           // começa vazio
  h->dados = calloc(h->capacidade + 1, sizeof(int));        // aloca o vetor de dados, com índice 0 ignorado
  return h;                                                 // retorna o ponteiro para o heap recém-criado
}

// função auxiliar que retorna o índice do pai de um nó (posição i)
static inline int parentIndex(int i)
{
  return i / 2;  // em um heap binário, o pai do nó i está na posição i/2
}

// função que dobra o tamanho do vetor de dados quando a capacidade é atingida
void expandirHeap(EstruturaHeap *h)
{
  h->capacidade *= 2;  // dobra a capacidade
  h->dados = realloc(h->dados, (h->capacidade + 1) * sizeof(int)); // realoca memória com o novo tamanho
}

// insere um novo valor no heap, mantendo a propriedade de heap máximo
// também retorna o número de trocas feitas (para contar comparações)
int enqueueHeap(EstruturaHeap *h, int num)
{
  // se o vetor estiver cheio, expande
  if (h->tamanho + 1 > h->capacidade)
    expandirHeap(h);

  int pos = ++h->tamanho;   // incrementa o tamanho e guarda a posição onde o novo valor vai entrar
  h->dados[pos] = num;      // insere o número no final do heap

  int trocas = 0;           // contador de trocas/comparações
  // sobe o elemento até ele ficar na posição correta (heapify-up)
  while (pos > 1 && h->dados[pos] > h->dados[parentIndex(pos)])
  {
    // troca o elemento com seu pai
    int temp = h->dados[pos];
    h->dados[pos] = h->dados[parentIndex(pos)];
    h->dados[parentIndex(pos)] = temp;

    pos = parentIndex(pos);  // move para a posição do pai
    trocas++;                // conta a troca feita
  }
  return trocas;  // retorna o número de comparações/trocas feitas
}

// mantém a propriedade de heap a partir de uma posição (heapify-down)
void heapify(EstruturaHeap *h, int i)
{
  int maior = i;                // começa assumindo que o maior é o próprio nó
  int esq = 2 * i;              // índice do filho da esquerda
  int dir = 2 * i + 1;          // índice do filho da direita

  // verifica se o filho da esquerda é maior
  if (esq <= h->tamanho && h->dados[esq] > h->dados[maior])
    maior = esq;

  // verifica se o filho da direita é maior
  if (dir <= h->tamanho && h->dados[dir] > h->dados[maior])
    maior = dir;

  // se o maior não for o nó atual, faz a troca e continua a ajustar
  if (maior != i)
  {
    int temp = h->dados[i];
    h->dados[i] = h->dados[maior];
    h->dados[maior] = temp;

    heapify(h, maior);  // recursivamente ajusta a subárvore
  }
}

// remove o elemento do topo do heap (maior elemento)
void dequeueHeap(EstruturaHeap *h)
{
  if (h->tamanho > 0)
  {
    // move o último elemento para o topo
    h->dados[1] = h->dados[h->tamanho];
    h->tamanho--;           // reduz o tamanho do heap
    heapify(h, 1);          // ajusta o heap a partir da raiz
  }
}

// libera toda a memória usada pelo heap
void liberarHeap(EstruturaHeap *h)
{
  free(h->dados);   // libera o vetor de dados
  free(h);          // libera a estrutura do heap
}
