#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX 10000

typedef struct literal
{
    int item;
    bool negado;
} literal;

typedef struct clausula
{
    literal literais[MAX]; // Implementar lista encadeada aqui
    int tam;
} clausula;

typedef struct formula
{
    clausula clausulas[MAX]; //Implementar lista encadeada aqui
    int clausula_qtd;
    int literal_tam;
} formula;

formula *inicializar_formula()
{
    formula *nova_formula = (formula*) malloc(sizeof(formula));
    for (int i = 0; i < MAX; i++)
    {
        nova_formula->clausulas[i].tam = 0;
    }
    return nova_formula;
}

void ler_clausula(clausula *c)
{
    int j = 0;
    while (1) // Ler literais
    {
        int aux;
        scanf("%d", &aux);
        if (aux == 0) return;

        else if (aux > 0)
        {
            c->literais[j].item = aux;
            c->literais[j].negado = false;
        }
        
        else
        {
            aux *= -1;
            c->literais[j].item = aux;
            c->literais[j].negado = true;
        }

        c->tam++;
        j++;
    }
}

void leitor(formula *f)
{
    for (int i = 0; i < f->clausula_qtd; i++)
    {
        ler_clausula(&f->clausulas[i]);
    }
}

void printador(formula *f)
{
    for (int i = 0; i < f->clausula_qtd ; i++)
    {
        printf("(");
        for (int j = 0; j < f->clausulas[i].tam; j++)
        {
            if(f->clausulas[i].literais[j].negado)
            {
                printf("~%d", f->clausulas[i].literais[j].item);
            }
            else
            {
                printf("%d", f->clausulas[i].literais[j].item);
            }
            if (j < f->clausulas[i].tam - 1)
            {
                printf(" V ");
            }
        }
        printf(")");
        if (i < f->clausula_qtd-1)
        {
            printf(" ^ ");
        }
    }
}

int main()
{
    char comando;

    int n;

    char comentario[MAX];
    char formato[5];
    formula *total = inicializar_formula();

    while (1)
    {

        scanf("%c ", &comando);

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
                break;
            }
            
            leitor(total);

            printador(total);

            return 0;
        }
        else
        {
            printf("ERROR\n");
            break;
        }
    }
}