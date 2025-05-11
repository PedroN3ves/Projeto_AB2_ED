#include "../libs/compressao.h"
#include "../libs/descompressao.h"
#include "../libs/utilitarios.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int menu()
{
    int opcao;
    while (1)
    {
    printf("\n--------------------------\n");
    printf("ESCOLHA UMA OPÇÃO:\n[1] - COMPACTAR ARQUIVO\n[2] - DESCOMPACTAR ARQUIVO\n[3] - SAIR");
    printf("\n--------------------------\n");
    scanf("%d", &opcao);
        switch (opcao)
        {
            case 1: 
                comp();
                break;
            case 2:
                decomp();
                break;
            case 3:
                printf("PROGRAMA FINALIZADO COM SUCESSO.");
                return 0;
            default:
                printf("ESSA NÃO É UMA OPÇÃO VÁLIDA, TENTE NOVAMENTE!");
                break;
        }
    }
}

int comp()
{
    char nome_arquivo[256];

    printf("--------------------------\n");

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
    while (atual != NULL) {
        noHuffman *copia = criar_no(atual->caracter, atual->frequencia);
        enqueue(h, copia);  
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

    printf("--------------------------\n");
    printf("Compressao feita! Arquivo salvo como: %s\n", nome_saida);

    liberar_lista(lista_frequencia);
    liberar_arvore(raiz);
    free(h->dados);
    free(h);

    return 0;
}

int decomp()
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