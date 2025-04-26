#include "../libs/leitor_header.h"
#include "../libs/operacoes_sat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

bool imprimir_primeiro_caminho_sat(no_arvore_binaria *raiz, int literal_tam)
{
    if (raiz == NULL)
        return false;

    if (raiz->resultado && raiz->left == NULL && raiz->right == NULL)
    {
        for (int i = 1; i <= literal_tam; i++)
        {
            if (raiz->interpretacao.valores[i] == 1)
                printf("%d = 1\n", i);
            else if (raiz->interpretacao.valores[i] == 0)
                printf("%d = 0\n", i);
            else
                printf("%d = indefinido\n", i);
        }
        return true;
    }

    if (imprimir_primeiro_caminho_sat(raiz->left, literal_tam))
        return true;
    if (imprimir_primeiro_caminho_sat(raiz->right, literal_tam))
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