#include <stdio.h>   // Biblioteca para entrada/saída (ex: printf)
#include <stdlib.h>  // Biblioteca para alocação de memória (ex: malloc, free)

// Define a estrutura de um nó da fila de prioridade
typedef struct Node
{
  int chave;           // Valor armazenado no nó (prioridade)
  struct Node *next;   // Ponteiro para o próximo nó na fila
} Node;

// Define a estrutura da fila de prioridade
typedef struct
{
  Node *head;          // Ponteiro para o primeiro nó da fila
} FilaPrioridade;

// Cria uma nova fila de prioridade vazia
FilaPrioridade *criarFilaPrioridade()
{
  // Aloca memória para a fila
  FilaPrioridade *f = malloc(sizeof(FilaPrioridade));
  // Inicializa a cabeça da fila como NULL (vazia)
  f->head = NULL;
  return f;  // Retorna a fila criada
}

// Insere um elemento na fila de prioridade (ordem decrescente)
int enqueue(FilaPrioridade *f, int chave)
{
  // Cria um novo nó
  Node *nova = malloc(sizeof(Node));
  nova->chave = chave;  // Atribui o valor ao nó
  nova->next = NULL;    // Inicializa o próximo como NULL

  int comps = 0;  // Contador de comparações (para análise)

  // Caso 1: Insere no início (se a fila está vazia ou a chave é maior que a cabeça)
  if (!f->head || chave > f->head->chave)
  {
    nova->next = f->head;  // O novo nó aponta para a antiga cabeça
    f->head = nova;        // A cabeça agora é o novo nó
    return comps;          // Retorna 0 comparações (inserção direta)
  }

  // Caso 2: Procura a posição correta para inserir no meio/fim
  Node *p = f->head;  // Ponteiro auxiliar para percorrer a fila
  while (p->next && p->next->chave > chave)  // Enquanto houver próximo e a prioridade for maior
  {
    p = p->next;      // Avança para o próximo nó
    comps++;          // Incrementa o contador de comparações
  }

  // Insere o novo nó na posição encontrada
  nova->next = p->next;  // O novo nó aponta para o próximo de p
  p->next = nova;        // p agora aponta para o novo nó
  return comps;          // Retorna o número de comparações
}

// Remove e retorna o elemento de maior prioridade (cabeça da fila)
int dequeue(FilaPrioridade *f)
{
  // Se a fila estiver vazia, retorna -1 (erro)
  if (!f->head)
  {
    return -1;
  }

  // Remove o nó da cabeça
  Node *removido = f->head;      // Salva o nó a ser removido
  int chave = removido->chave;   // Armazena o valor do nó
  f->head = removido->next;      // Atualiza a cabeça para o próximo nó
  free(removido);                // Libera a memória do nó removido

  return chave;  // Retorna o valor removido
}

// Libera toda a memória alocada para a fila
void liberarFila(FilaPrioridade *f)
{
  // Percorre a fila liberando cada nó
  while (f->head)
  {
    Node *aux = f->head;    // Salva o nó atual
    f->head = aux->next;    // Avança para o próximo
    free(aux);              // Libera o nó salvo
  }
  free(f);  // Libera a estrutura da fila
}