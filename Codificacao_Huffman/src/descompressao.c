#include "../libs/compressao.h"
#include "../libs/descompressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


noHuffman *desserializar_arvore(FILE *entrada, int *lidos)
{
    unsigned char c;
    fread(&c, 1, 1, entrada);
    (*lidos)++;

    void* byte_ptr = &c;

    if (c == '\\')
    {
        fread(&c, 1, 1, entrada);
        (*lidos)++;
        return criar_no(byte_ptr, 0);
    }
    if (c != '*')
    {
        return criar_no(byte_ptr, 0);
    }
    unsigned char asterisco = '*';
    noHuffman *no = criar_no(&asterisco, 0);
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
    unsigned char c;

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
                fwrite(atual->byte, 1, 1, saida);
                atual = raiz;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
    liberar_arvore(raiz);
}