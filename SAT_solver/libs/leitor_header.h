#ifndef leitor_header
#define leitor_header

#include <stdio.h>
#include <stdbool.h>

#define MAX 10000

typedef struct literal
{
    int item;
    bool negado;
    struct literal *next;
} literal;

typedef struct clausula
{
    struct clausula *next;
    literal *head_literal;
    int tam;
} clausula;

typedef struct formula
{
    clausula *head_clausula;
    int clausula_qtd;
    int literal_tam;
} formula;

formula *inicializar_formula();
clausula *add_clausula(formula *f);
literal *add_literal(literal *head, int item, bool negado);
int ler_clausula(clausula *c, formula *f);
int leitor(formula *f);
void printador(formula *f);
formula *gerar_formula_cnf();

#endif