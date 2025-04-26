#ifndef operacoes_sat
#define operacoes_sat

#include "leitor_header.h"
#include <stdbool.h>

#define UNDEFINED 2

typedef struct partial_interpretation
{
    short *valores;
} partial_interpretation;

typedef struct no_arvore_binaria
{
    int literal;
    short valor;
    partial_interpretation interpretacao;
    struct no_arvore_binaria *left;
    struct no_arvore_binaria *right;
    bool resultado;
} no_arvore_binaria;

partial_interpretation inicializar_partial_interpretation(formula *F);
bool implica_F(formula *F, partial_interpretation *I);
bool implica_negF(formula *F, partial_interpretation *I);
partial_interpretation uniao(partial_interpretation I, int literal_tam, int xi, short valor);
no_arvore_binaria *sat(formula *F, partial_interpretation I);

#endif