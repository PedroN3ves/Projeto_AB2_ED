#include "../libs/leitor_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

formula *inicializar_formula()
{
    formula *nova_formula = malloc(sizeof(formula));
    nova_formula->head_clausula = NULL;
    nova_formula->clausula_qtd = 0;
    nova_formula->literal_tam = 0;
    return nova_formula;
}

clausula *add_clausula(formula *f)
{

    clausula *nova = malloc(sizeof(clausula));
    nova->tam = 0;
    nova->head_literal = NULL;
    nova->next = f->head_clausula;
    f->head_clausula = nova;
    return nova;
}

literal *add_literal(literal *head, int item, bool negado)
{
    literal *new_literal = (literal *)malloc(sizeof(literal));
    new_literal->item = item;
    new_literal->negado = negado;
    new_literal->next = head;
    return new_literal;
}

int ler_clausula(clausula *c, formula *f)
{
    while (1)
    {
        int aux;
        scanf("%d", &aux);

        if (aux > f->literal_tam || aux < f->literal_tam * -1)
        {
            return -1;
        }

        if (aux == 0)
            return 0;
        else if (aux > 0)
        {
            c->head_literal = add_literal(c->head_literal, aux, false);
        }
        else
        {
            c->head_literal = add_literal(c->head_literal, -aux, true);
        }

        c->tam++;
    }
}

int leitor(formula *f)
{
    for (int i = 0; i < f->clausula_qtd; i++)
    {
        clausula *nova = add_clausula(f);
        if (ler_clausula(nova, f) == -1)
            return -1;
    }
    return 0;
}

void printador(formula *f)
{
    clausula *c_atual = f->head_clausula;
    while (c_atual)
    {
        printf("(");
        literal *l_atual = c_atual->head_literal;

        while (l_atual)
        {
            if (l_atual->negado)
                printf("~%d", l_atual->item);
            else
                printf("%d", l_atual->item);

            if (l_atual->next)
                printf(" V ");

            l_atual = l_atual->next;
        }

        printf(")");
        if (c_atual->next)
            printf(" ^ ");

        c_atual = c_atual->next;
    }
    printf("\n");
}

formula *gerar_formula_cnf()
{
    char comando;
    char comentario[MAX];
    char formato[5];
    formula *total = inicializar_formula();

    while (scanf(" %c", &comando) != EOF)
    {
        if (comando == 'c')
        {
            fgets(comentario, MAX, stdin);
            printf("%s", comentario);
        }
        else if (comando == 'p')
        {
            scanf("%s %d %d", formato, &total->literal_tam, &total->clausula_qtd);

            if (strcmp(formato, "cnf") != 0)
            {
                printf("ERROR\n");
                return NULL;
            }

            if (leitor(total) == -1)
            {
                printf("ERRO DE LEITURA, VALOR FORA DO ESCOPO\n");
                return NULL;
            };
        }
        else
        {
            printf("ERROR\n");
            return NULL;
        }
    }
    return total;
}