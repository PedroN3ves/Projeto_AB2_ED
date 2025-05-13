#include "../libs/operacoes_sat.h"
#include <stdlib.h>

partial_interpretation inicializar_partial_interpretation(formula *F)
{
    partial_interpretation new_pi;
    new_pi.valores = malloc(sizeof(short) * F->literal_tam + 1);
    for (int i = 0; i < F->literal_tam + 1; i++)
    {
        new_pi.valores[i] = UNDEFINED;
    }
    return new_pi;
}

bool implica_F(formula *F, partial_interpretation I)
{
    clausula *clausula_atual = F->head_clausula;
    while (clausula_atual != NULL)
    {
        bool satisfeita = false;
        literal *literal_atual = clausula_atual->head_literal;

        while (literal_atual != NULL)
        {
            short val = I.valores[literal_atual->item];
            if (val != UNDEFINED)
            {
                bool v = val ^ literal_atual->negado;
                if (v)
                {
                    satisfeita = true;
                    break;
                }
            }
            literal_atual = literal_atual->next;
        }

        if (!satisfeita)
        {
            return false;
        }
        clausula_atual = clausula_atual->next;
    }
    return true;
}

bool implica_negF(formula *F, partial_interpretation I)
{
    clausula *clausula_atual = F->head_clausula;
    while (clausula_atual != NULL)
    {
        bool satisfaz = false;
        literal *literal_atual = clausula_atual->head_literal;

        while (literal_atual != NULL)
        {
            short val = I.valores[literal_atual->item];
            if (val == UNDEFINED)
            {
                satisfaz = true;
                break;
            }
            if (val ^ literal_atual->negado)
            {
                satisfaz = true;
            }
            literal_atual = literal_atual->next;
        }

        if (!satisfaz)
        {
            return true;
        }
        clausula_atual = clausula_atual->next;
    }
    return false;
}

partial_interpretation uniao(partial_interpretation I, int literal_tam, int xi, short valor)
{
    partial_interpretation novo;
    novo.valores = malloc(sizeof(short) * literal_tam + 1);
    for (int i = 1; i < literal_tam + 1; i++)
    {
        novo.valores[i] = I.valores[i];
    }
    novo.valores[xi] = valor;
    return novo;
}

no_arvore_binaria *sat(formula *F, partial_interpretation I)
{
    no_arvore_binaria *n = (no_arvore_binaria *)malloc(sizeof(no_arvore_binaria));
    n->interpretacao = I;
    n->left = NULL;
    n->right = NULL;

    if (implica_F(F, I))
    {
        n->resultado = true;
        return n;
    }
    if (implica_negF(F, I))
    {
        n->resultado = false;
        return n;
    }

    int xi = -1;
    for (int i = 1; i < F->literal_tam + 1; i++)
    {
        if (I.valores[i] == UNDEFINED)
        {
            xi = i;
            break;
        }
    }

    n->valor = xi;

    partial_interpretation I_true = uniao(I, F->literal_tam, xi, true);
    n->left = sat(F, I_true);

    partial_interpretation I_false = uniao(I, F->literal_tam, xi, false);
    n->right = sat(F, I_false);

    n->resultado = n->left->resultado || n->right->resultado;
    return n;
}