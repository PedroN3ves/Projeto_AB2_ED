#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ASCII 256
#define MAX_NOME 50

typedef struct noHuffman
{
    char caracter;
    int frequencia;
    struct noHuffman *next;
    struct noHuffman *left;
    struct noHuffman *right;
} noHuffman;

noHuffman *criar_no(char caracter, int frequencia)
{
    noHuffman *novo = (noHuffman *)malloc(sizeof(noHuffman));
    novo->caracter = caracter;
    novo->frequencia = frequencia;
    novo->left = NULL;
    novo->right = NULL;
    novo->next = NULL;
    return novo;
}

void liberar_arvore(noHuffman *raiz)
{
    if (raiz == NULL)
        return;
    liberar_arvore(raiz->left);
    liberar_arvore(raiz->right);
    free(raiz);
}

noHuffman *desserializar_arvore(FILE *entrada, int *lidos)
{
    int c = fgetc(entrada);
    (*lidos)++;
    if (c == '\\')
    {
        c = fgetc(entrada);
        (*lidos)++;
        return criar_no((char)c, 0);
    }
    if (c != '*')
    {
        return criar_no((char)c, 0);
    }
    noHuffman *no = criar_no('*', 0);
    no->left = desserializar_arvore(entrada, lidos);
    no->right = desserializar_arvore(entrada, lidos);
    return no;
}

void descomprimir(const char *entrada_nome, const char *saida_nome)
{
    FILE *entrada = fopen(entrada_nome, "rb");
    FILE *saida = fopen(saida_nome, "wb");
    if (!entrada || !saida)
    {
        perror("Erro ao abrir arquivos");
        return;
    }

    int byte1 = fgetc(entrada);
    int byte2 = fgetc(entrada);
    int lixo = byte1 >> 5;
    int tam_arvore = ((byte1 & 0b00011111) << 8) | byte2;

    int lidos = 0;
    noHuffman *raiz = desserializar_arvore(entrada, &lidos);
    if (lidos != tam_arvore)
    {
        printf("Erro! Tamanho da arvore nao corresponde aos bytes lidos!\n");
        return;
    }

    noHuffman *atual = raiz; // percorrer a arvore
    int c;

    fseek(entrada, 2 + tam_arvore, SEEK_SET); // pula os 2 bytes do cabeçalho e a árvore

    long atual_pos = ftell(entrada); // retorna a byte logo após o cabecalho + árvore
    fseek(entrada, 0, SEEK_END);
    long tam_arquivo = ftell(entrada); // retorna o tamanho do arquivo
    fseek(entrada, atual_pos, SEEK_SET);

    if ((tam_arquivo - atual_pos) > INT_MAX)
    {
        fprintf(stderr, "Erro: Tamanho muito grande para int!\n");
        exit(1);
    }

    int total_bytes = tam_arquivo - atual_pos;
    for (int i = 0; i < total_bytes; i++)
    {
        c = fgetc(entrada);

        int limite; // qtd de bits relevante dentro do byte
        if (i == total_bytes - 1)
        {
            limite = 8 - lixo;
        }
        else
        {
            limite = 8;
        }

        for (int b = 7; b >= 8 - limite; b--)
        {
            int bit = (c >> b) & 0b00000001;
            if (bit == 1)
            {
                atual = atual->right;
            }
            else
            {
                atual = atual->left;
            }

            if (atual->left == NULL && atual->right == NULL)
            {
                fputc(atual->caracter, saida);
                atual = raiz;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}

int main()
{
    char entrada_nome[MAX_NOME];
    char extensao[MAX_NOME];

    printf("Digite o nome do arquivo comprimido: ");
    scanf("%s", entrada_nome);

    printf("Digite a extensao desejada para o arquivo descomprimido (ex: .txt, .bin, .pdf): ");
    scanf("%s", extensao);

    char saida_nome[MAX_NOME];
    strcpy(saida_nome, entrada_nome);

    char *ponto = strrchr(saida_nome, '.');
    if (ponto != NULL)
    {
        *ponto = '\0'; // Remove a extensão
    }

    strcat(saida_nome, "(1)");
    strcat(saida_nome, extensao); // Nova extensão

    descomprimir(entrada_nome, saida_nome);

    printf("Arquivo descomprimido salvo como: %s\n", saida_nome);
    return 0;
}
