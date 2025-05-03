#include <stdio.h>
#include <stdlib.h>
#include "fila_com_heap.c"
#include "fila_sem_heap.c"

int main()
{
  FILE *in = fopen("entrada.txt", "r");
  FILE *out = fopen("comparacoes.csv", "w");

  if (!in || !out)
  {
    fprintf(stderr, "Erro ao acessar arquivos.\n");
    return 1;
  }

  FilaPrioridade *fila = criarFilaPrioridade();
  EstruturaHeap *filaHeap = criarHeap();

  int valor;
  fprintf(out, "Comparação com Heap / Comparação sem heap / Tamanho\n");

  while (fscanf(in, "%d", &valor) == 1)
  {
    int c1 = enqueue(fila, valor);
    int c2 = enqueueHeap(filaHeap, valor);
    fprintf(out, "%d %d %d\n", c2, c1, filaHeap->tamanho);
  }

  liberarFila(fila);
  liberarHeap(filaHeap);

  fclose(in);
  fclose(out);

  puts("Arquivo de saída gerado com sucesso.");
  return 0;
}
