#ifndef compressao
#define compressao

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII 256

// Estrutura que representa um nó da árvore de Huffman
typedef struct noHuffman {
    char caracter;
    int frequencia;
    struct noHuffman *next;
    struct noHuffman *left;
    struct noHuffman *right;
} noHuffman;

// Estrutura para armazenar códigos de Huffman
typedef struct tabelaHuffman {
    char caracter;
    char codigo[ASCII];
} tabelaHuffman;

// Estrutura de Min Heap
typedef struct heap {
    noHuffman **dados;
    int tamanho;
    int capacidade;
} heap;

// Funções para manipulação de nós e lista encadeada
noHuffman *criar_no(char caracter, int frequencia);
void add_atualizar(noHuffman **head, char caracter);
void liberar_lista(noHuffman *head);

// Funções para manipulação de heap
heap *criar_heap(int capacidade);
int get_parent_index(int i);
int get_left_index(int i);
int get_right_index(int i);
int comparar(noHuffman *a, noHuffman *b);
void swap(noHuffman **a, noHuffman **b);
void enqueue(heap *h, noHuffman *no);
void min_heapify(heap *h, int i);
noHuffman *dequeue_min(heap *h);

// Funções para a árvore de Huffman
noHuffman *criar_arvore(heap *h);
void liberar_arvore(noHuffman *raiz);

// Funções para serialização e compressão
void serializar_arvore(noHuffman *raiz, FILE *saida, int *tam_arvore);
void construir_tabela(noHuffman *no, char *codigo_atual, int profundidade, tabelaHuffman *tabela, int *i);
void escrever_bits(FILE *saida, tabelaHuffman *tabela, int tam_tabela, const char *original, int *lixo);
void escrever_cabecalho(FILE *saida, int lixo, int tam_arvore);

#endif // HUFFMAN_H