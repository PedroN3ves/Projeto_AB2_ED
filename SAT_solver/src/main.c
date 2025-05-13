#include "../libs/leitor_header.h"
#include "../libs/operacoes_sat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool imprimir_primeiro_caminho_sat(no_arvore_binaria *no, int literal_tam)
{
    if (no == NULL)
        return false;

    if (no->resultado && no->left == NULL && no->right == NULL)
    {
        for (int i = 1; i <= literal_tam; i++)
        {
            if (no->interpretacao.valores[i] == 1)
                printf("%d = 1\n", i);
            else if (no->interpretacao.valores[i] == 0)
                printf("%d = 0\n", i);
            else
                printf("%d = indefinido\n", i);
        }
        return true;
    }

    if (imprimir_primeiro_caminho_sat(no->left, literal_tam))
        return true;
    if (imprimir_primeiro_caminho_sat(no->right, literal_tam))
        return true;

    return false;
}

int main()
{
    formula *total = gerar_formula_cnf();

    printador(total);

    partial_interpretation I = inicializar_partial_interpretation(total);

    no_arvore_binaria *n = sat(total, I);

    if (n->resultado)
    {
        printf("\nSAT\n");
        imprimir_primeiro_caminho_sat(n, total->literal_tam);
    }
    else
    {
        printf("\nUNSAT\n");
    }

    free(I.valores);

    free(total);
    return 0;
}