#include "../libs/leitor_header.h"
#include "../libs/operacoes_sat.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int main()
{
    formula *total = gerar_formula_cnf();

    printador(total);

    partial_interpretation I = inicializar_partial_interpretation(total);

    if (sat(total, I))
    {
        printf("\nSAT\n");
    }
    else
    {
        printf("\nUNSAT\n");
    }

    free(I.valores);

    free(total);
    return 0;
}