#ifndef descompressao
#define descompressao

#include "compressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

noHuffman *desserializar_arvore(FILE *entrada, int *lidos);
void descomprimir(const char *entrada_nome, const char *saida_nome);

#endif