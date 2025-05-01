#include <stdio.h>
#include <stdlib.h>

// 29 bytes
typedef struct noHuffman
{
    char caracter;
    int frequencia;
    struct noHuffman *next;
    struct noHuffman *left;
    struct noHuffman *right;
} noHuffman;

typedef struct heap
{
    noHuffman **dados;
    int tamanho;
    int capacidade;
} heap;

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

void imprimir_pre_ordem(noHuffman *raiz)
{
    if (raiz == NULL)
    {
        return;
    }

    if (raiz->left != NULL || raiz->right != NULL)
    {
        // É um nó interno
        printf("*");
    }
    else
    {
        // É uma folha
        printf("%c", raiz->caracter);
    }

    imprimir_pre_ordem(raiz->left);
    imprimir_pre_ordem(raiz->right);
}

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

int main(int argc, char **argv)
{
    // ex: argv[0] = "./sample.exe" | argv[1] = "filename.txt"
    if (argc != 2)
    {
        printf("Erro! Entrada Incompleta\n");
        return 1;
    }

    // "rb" = "read binary"
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    noHuffman *lista_frequencia = NULL;
    int total_bytes = 0;

    int byte;
    while ((byte = fgetc(file)) != EOF)
    {
        add_atualizar(&lista_frequencia, (char)byte);
        total_bytes++;
    }

    fclose(file);

    heap *h = criar_heap(256);
    noHuffman *atual = lista_frequencia;
    while (atual != NULL)
    {
        enqueue(h, atual);
        atual = atual->next;
    }

    noHuffman *raiz = criar_arvore(h);

    printf("Arvore de Huffman em pre-ordem:\n");
    imprimir_pre_ordem(raiz);

    liberar_lista(lista_frequencia);

    liberar_arvore(raiz);

    free(h->dados);
    free(h);

    return 0;
}
