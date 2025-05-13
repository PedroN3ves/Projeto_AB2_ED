#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/compressao.h"
 
/**
 * Aloca e inicializa um novo nó Huffman
 * 
 * @param byte O caracter do novo nó
 * @param frequencia Frequencia do caracter a ser adicionado
 * @return Ponteiro para o novo nó criado
 */
noHuffman *criar_no(void* byte, int frequencia)
{
    noHuffman *novo_no = (noHuffman *)malloc(sizeof(noHuffman));
    if (novo_no == NULL)
    {
        printf("Erro na alocação de memória");
        exit(1);
    }

    // Por ser ponteiro, se usa memcpy
    novo_no->byte = malloc(1);
    memcpy(novo_no->byte, byte, 1);
    novo_no->frequencia = frequencia;
    novo_no->next = NULL;
    novo_no->left = NULL;
    novo_no->right = NULL;

    return novo_no;
}

/**
 * Adiciona um nó huffman ou atualiza sua frequencia
 * Se o caracter já existir na lista, aumenta sua frequencia em 1
 * Se não, cria um novo nó com frequencia 1
 * 
 * @param head Ponteiro para o inicio da lista de nós
 * @param byte Byte a ser adicionado/atualizado
 */
void add_atualizar(noHuffman **head, void* byte)
{
    noHuffman *atual = *head;
    noHuffman *anterior = NULL;

    while (atual != NULL && memcmp(atual->byte, byte, 1) != 0)
    {
        anterior = atual;
        atual = atual->next;
    }

    if (atual != NULL)
    {
        atual->frequencia++;
    }
    else
    {
        noHuffman *novo_no = criar_no(byte, 1);

        // caso lista vazia
        if (anterior == NULL)
        {
            novo_no->next = *head;
            *head = novo_no;
        }
        else
        {
            novo_no->next = anterior->next;
            anterior->next = novo_no;
        }
    }
}

heap *criar_heap(int capacidade)
{
    heap *h = (heap *)malloc(sizeof(heap));
    h->dados = (noHuffman **)malloc(capacidade * sizeof(noHuffman *));
    h->tamanho = 0;
    h->capacidade = capacidade;

    return h;
}

int get_parent_index(int i)
{
    return (i - 1) / 2;
}

int get_left_index(int i)
{
    return 2 * i + 1;
}

int get_right_index(int i)
{
    return 2 * i + 2;
}

/**
 * Critério de comparação para heap, qual dos nós tem maior frequência (Desempata baseado no valor do byte)
 * 
 * @param a Primeiro valor a ser comparado
 * @param b Segundo valor a ser comparado
 * @return Valor inteiro para se usar na heap mínima
 */
int comparar(noHuffman *a, noHuffman *b)
{
    if (a->frequencia != b->frequencia)
    {
        return a->frequencia - b->frequencia;
    }

    // desempate por valor do byte
    unsigned char byte_a = *(unsigned char*)a->byte;
    unsigned char byte_b = *(unsigned char*)b->byte;
    return byte_a - byte_b;
}

void swap(noHuffman **a, noHuffman **b)
{
    noHuffman *temp = *a;
    *a = *b;
    *b = temp;
}

void enqueue(heap *h, noHuffman *no)
{
    if (h->tamanho >= h->capacidade)
    {
        h->capacidade *= 2;
        h->dados = (noHuffman **)realloc(h->dados, h->capacidade * sizeof(noHuffman *));
    }

    h->dados[h->tamanho] = no;
    int i = h->tamanho;
    h->tamanho++;

    while (i > 0 && comparar(h->dados[i], h->dados[get_parent_index(i)]) < 0)
    {
        swap(&h->dados[i], &h->dados[get_parent_index(i)]);
        i = get_parent_index(i);
    }
}

void min_heapify(heap *h, int i)
{
    int menor = i;
    int left = get_left_index(i);
    int right = get_right_index(i);

    if (left < h->tamanho && comparar(h->dados[left], h->dados[menor]) < 0)
    {
        menor = left;
    }
    if (right < h->tamanho && comparar(h->dados[right], h->dados[menor]) < 0)
    {
        menor = right;
    }

    if (menor != i)
    {
        swap(&h->dados[i], &h->dados[menor]);
        min_heapify(h, menor);
    }
}

noHuffman *dequeue_min(heap *h)
{
    if (h->tamanho == 0)
    {
        return NULL;
    }

    noHuffman *min = h->dados[0];
    h->dados[0] = h->dados[h->tamanho - 1];
    h->tamanho--;

    min_heapify(h, 0);
    return min;
}

/**
 * Constrói a árvore de Huffman a partir de um heap de frequências.
 * 
 * @param h Heap contendo os nós com suas frequências
 * @return  Raiz da árvore de Huffman construída
 */
noHuffman *criar_arvore(heap *h)
{
    while (h->tamanho > 1)
    {
        noHuffman *esq = dequeue_min(h);
        noHuffman *dir = dequeue_min(h);

        unsigned char asterisco = '*';
        noHuffman *pai = criar_no(&asterisco, esq->frequencia + dir->frequencia);
        pai->left = esq;
        pai->right = dir;

        enqueue(h, pai);
    }
    return dequeue_min(h);
}

/**
 * Libera a memória da lista encadeada de nós
 * 
 * @param head Ponteiro pra o primeiro nó da lista
 */
void liberar_lista(noHuffman *head)
{
    noHuffman *atual = head;
    while (atual != NULL)
    {
        noHuffman *temp = atual;
        atual = atual->next;
        free(temp);
    }
}

/**
 * Serializa a árvore de Huffman em pré-ordem para o arquivo de saída.
 * 
 * @param raiz       Raiz da árvore a ser serializada
 * @param saida      Arquivo de saída onde a árvore será escrita
 * @param tam_arvore Ponteiro para contador do tamanho da árvore (será atualizado)
 */
void serializar_arvore(noHuffman *raiz, FILE *saida, int *tam_arvore)
{
    if (raiz == NULL)
    {
        return;
    }

    // for folha
    if (raiz->left == NULL && raiz->right == NULL)
    {
        unsigned char byte = *(unsigned char*)raiz->byte;

        if (byte == '*' || byte == '\\')
        {
            fputc('\\', saida);
            (*tam_arvore)++;
        }
        fputc(byte, saida);
        (*tam_arvore)++;
    }
    
    else
    {
        fputc('*', saida);
        (*tam_arvore)++;
        serializar_arvore(raiz->left, saida, tam_arvore);
        serializar_arvore(raiz->right, saida, tam_arvore);
    }
}

/**
 * Constrói a tabela de codificação percorrendo a árvore de Huffman.
 * 
 * @param no Nó atual sendo processado
 * @param codigo_atual Buffer para armazenar o código binário sendo construído
 * @param profundidade Profundidade atual na árvore
 * @param tabela Tabela de codificação sendo preenchida
 * @param i Ponteiro para índice atual na tabela
 */
void construir_tabela(noHuffman *no, char *codigo_atual, int profundidade, tabelaHuffman *tabela, int *i)
{
    // i* para manter seu valor após a recursão
    if (no == NULL)
    {
        return;
    }

    // for folha
    if (no->left == NULL && no->right == NULL)
    {
        tabela[*i].byte = malloc(1);
        memcpy(tabela[*i].byte, no->byte, 1);
        strncpy(tabela[*i].codigo, codigo_atual, profundidade);
        tabela[*i].codigo[profundidade] = '\0';
        (*i)++;
    }

    if (no->left != NULL)
    {
        codigo_atual[profundidade] = '0';
        construir_tabela(no->left, codigo_atual, profundidade + 1, tabela, i);
    }

    if (no->right != NULL)
    {
        codigo_atual[profundidade] = '1';
        construir_tabela(no->right, codigo_atual, profundidade + 1, tabela, i);
    }
}

/**
 * Escreve os bits no arquivo comprimido pelo Huffman
 * 
 * @param saida Arquivo a ser escrito
 * @param tabela Tabela que mapeia valores e caracteres
 * @param tam_tabela Tamanho da tabela
 * @param original Arquivo a ser comprimido
 * @param lixo Ponteiro para a variável que vai armazenar quantos bits de lixo o ultimo byte possuí
 */
void escrever_bits(FILE *saida, tabelaHuffman *tabela, int tam_tabela, const char *original, int *lixo)
{
    FILE *file = fopen(original, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo original");
        exit(1);
    }

    unsigned char byte = 0;
    int bit_index = 0;
    unsigned char c;
    while ((fread(&c, 1, 1, file) == 1))
    {
        for (int i = 0; i < tam_tabela; i++)
        {
            if (memcmp(tabela[i].byte, &c, 1) == 0)
            {
                char *codigo_temp = tabela[i].codigo;
                for (int j = 0; codigo_temp[j] != '\0'; j++)
                {
                    byte = byte << 1;
                    if (codigo_temp[j] == '1')
                    {
                        byte = byte | 1;
                    }
                    bit_index++;

                    if (bit_index == 8)
                    {
                        fputc(byte, saida);
                        byte = 0;
                        bit_index = 0;
                    }
                }
                break;
            }
        }
    }

    if (bit_index > 0)
    {
        byte = byte << (8 - bit_index);
        fputc(byte, saida);
        *lixo = 8 - bit_index;
    }
    else
    {
        *lixo = 0;
    }

    fclose(file);
}

/**
 * Escreve o cabeçalho no arquivo compactado
 * 
 * @param saida O arquivo onde vai ser escrito o cabeçalho
 * @param lixo Numero de bits lixo no ultimo byte
 * @param tam_arvore Tamanho da árvore a ser impressa no cabeçalho
 */
void escrever_cabecalho(FILE *saida, int lixo, int tam_arvore)
{
    unsigned short cabecalho = 0;
    cabecalho = cabecalho | (lixo << 13);
    cabecalho = cabecalho | (tam_arvore & 0b0001111111111111); // impedir que tam_arvore "vaze" para o lixo;
    fputc(cabecalho >> 8, saida);
    fputc(cabecalho & 0b11111111, saida); // garantir que o primeiro byte seja 00000000
}

/**
 * Libera a memória da árvore de Huffman
 * 
 * @param raiz A raiz da árvore
 */
void liberar_arvore(noHuffman *raiz)
{
    if (raiz == NULL)
    {
        return;
    }
    liberar_arvore(raiz->left);
    liberar_arvore(raiz->right);
    free(raiz->byte);
    free(raiz);
}
