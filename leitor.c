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
} clausula;

typedef struct formula
{
    clausula clausulas[MAX]; //Implementar lista encadeada aqui
    int clausula_qtd;
    int literal_tam;
} formula;

void leitor(formula *f)
{
    int aux;

    for (int i = 0; i < f->clausula_qtd; i++)
    {
        while (1) // Ler literais
        {
            scanf("%d", &aux);
            if (aux == 0) break;
            else if (aux > 0)
            {
                f->clausulas[i]->literais[aux-1].item = 1;
                f->clausular[i]->literais[aux-1].negado = false;
            }
            else
            {
                aux *= -1;
                f->clausulas[i]->literais[aux-1].item = 1;
                f->clausular[i]->literais[aux-1].negado = true;
            }
        }
    }
}

int main()
{
    char comando;

    int n;
    int i = 0;

    char comentario[MAX];
    char formato[5];
    formula total;

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
            scanf("%s %d %d", formato, &total.literal_tam, &total.clausula_qtd);

            if (strcmp(formato, "cnf") != 0)
            {
                printf("ERROR\n");
                break;
            }
            
            // FUNÇÃO AQUI
        }
        else
        {
            printf("ERROR\n");
            break;
        }
    }
}