#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII 256

// 29 bytes
/**
 * Estrutura que representa um nó da arvore de Huffman e também da lista encadeada
 * 
 * @field caracter O caracter ou byte do nó para nós folha, para nós pai guarda um '*'
 * @field frequencia Frequência de ocorrência do caracter ou byte no arquivo
 * @field next Ponteiro para o próximo nó da lista encadeada
 * @field left Ponteiro para o filho esquerdo da árvore
 * @field right Ponteiro para o filho direito da árvore
 */
typedef struct noHuffman
{
    char caracter;
    int frequencia;
    struct noHuffman *next;
    struct noHuffman *left;
    struct noHuffman *right;
} noHuffman;

// 29 bytes
/**
 * Estrutura de Min Heap para construção da árvore de Huffman
 * 
 * @field dados Array de ponteiros para nós do Huffman
 * @field tamanho Número atual de elementos na heap
 * @field capacidade Total de elementos que a heap suporta
 */
typedef struct heap
{
    noHuffman **dados;
    int tamanho;
    int capacidade;
} heap;


typedef struct tabelaHuffman
{
    char caracter;
    char codigo[ASCII];
} tabelaHuffman;

/**
 * Aloca e inicializa um novo nó Huffman
 * 
 * @param caracter O caracter do novo nó
 * @param frequencia Frequencia do caracter a ser adicionado
 * @return Ponteiro para o novo nó criado
 */
noHuffman *criar_no(char caracter, int frequencia)
{
    noHuffman *novo_no = (noHuffman *)malloc(sizeof(noHuffman));
    if (novo_no == NULL)
    {
        printf("Erro na alocação de memória");
        exit(1);
    }

    novo_no->caracter = caracter;
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
 * @param caracter Caracter a ser adicionado/atualizado
 */
void add_atualizar(noHuffman **head, char caracter)
{
    noHuffman *atual = *head;
    noHuffman *anterior = NULL;

    while (atual != NULL && atual->caracter != caracter)
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
        noHuffman *novo_no = criar_no(caracter, 1);

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
    return (unsigned char)a->caracter - (unsigned char)b->caracter;
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

        noHuffman *pai = criar_no('*', esq->frequencia + dir->frequencia);
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
        if (raiz->caracter == '*' || raiz->caracter == '\\')
        {
            fputc('\\', saida);
            (*tam_arvore)++;
        }
        fputc(raiz->caracter, saida);
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
        tabela[*i].caracter = no->caracter;
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
    int c;
    while ((c = fgetc(file)) != EOF)
    {
        for (int i = 0; i < tam_tabela; i++)
        {
            if (tabela[i].caracter == (char)c)
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
    free(raiz);
}

int main()
{
    char nome_arquivo[256];

    printf("Digite o nome do arquivo a ser comprimido: ");
    scanf("%255s", nome_arquivo);

    FILE *file = fopen(nome_arquivo, "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    noHuffman *lista_frequencia = NULL;

    int byte;
    while ((byte = fgetc(file)) != EOF)
    {
        add_atualizar(&lista_frequencia, (char)byte);
    }
    fclose(file);

    heap *h = criar_heap(ASCII);
    noHuffman *atual = lista_frequencia;
    while (atual != NULL)
    {
        enqueue(h, atual);
        atual = atual->next;
    }

    noHuffman *raiz = criar_arvore(h);

    tabelaHuffman tabela[ASCII];
    int tam_tabela = 0;
    char codigo_atual[ASCII];
    construir_tabela(raiz, codigo_atual, 0, tabela, &tam_tabela);

    // Gerar nome do arquivo de saída com extensão .huff
    char nome_saida[300];
    strcpy(nome_saida, nome_arquivo);
    char *ponto = strrchr(nome_saida, '.');
    if (ponto != NULL)
    {
        *ponto = '\0'; // remove a extensão original
    }
    strcat(nome_saida, ".huff");

    FILE *saida = fopen(nome_saida, "wb");
    if (saida == NULL)
    {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    // reserva 2 bytes para o cabeçalho (lixo + tamanho da árvore)
    fputc(0, saida);
    fputc(0, saida);

    int tam_arvore = 0;
    serializar_arvore(raiz, saida, &tam_arvore);

    int lixo = 0;
    escrever_bits(saida, tabela, tam_tabela, nome_arquivo, &lixo);

    fseek(saida, 0, SEEK_SET);
    escrever_cabecalho(saida, lixo, tam_arvore);

    fclose(saida);
    printf("Compressao feita! Arquivo salvo como: %s\n", nome_saida);

    liberar_lista(lista_frequencia);
    liberar_arvore(raiz);
    free(h->dados);
    free(h);

    return 0;
}