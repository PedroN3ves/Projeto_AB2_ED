#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct NoAVL
{
  char *palavra;
  char *significado;
  int altura;
  struct NoAVL *direita, *esquerda;
} NoAVL;

NoAVL *novoNo(char *palavra, char *significado)
{ // cria o nó da árvore e retorna o endereço deste nó para a struct NOAVL
  NoAVL *novo = malloc(sizeof(NoAVL));

  if (novo)
  {
    novo->palavra = strdup(palavra);         // atribui a variavel palavra para o "membro" palavra (strdup ja aloca espaço na memoria para string e copia ela)
    novo->significado = strdup(significado); // atribui a variavel significado para o "membro" significado
    novo->altura = 1;                        // atribui 0 para o "membro" altura
    novo->direita = NULL;                    // atribui NULL para o "membro" direita
    novo->esquerda = NULL;                   // atribui NULL para o "membro" esquerda
  }
  else
  {
    printf("\nERRO!\n"); // caso não consiga armazenar o espaço na memória printa a mensagem
  }
  return novo; // retorna o o novo nó
}

int maior(int a, int b)
{                         // função que verifica qual maior dos 2 inteiros
  return (a > b) ? a : b; // se a > b retorne a, se não retorne b
}

int altura(NoAVL *no)
{ // função que calcula altura usada para rebalanceamento
  if (no == NULL)
  {
    return 0; // caso no seja vazio retornará 0
  }
  else
  {
    return no->altura; // caso contrário retorna nó apontando para altura, retornando a altura da arvore
  }
}

int fatordeBalanceamento(NoAVL *no)
{
  if (no == NULL)
  {
    return 0;
  }

  return (altura(no->esquerda) - altura(no->direita)); // calcula o fator de balanceamento que deve ser no máximo 1 e no mínimo -1
}

NoAVL *rotacaoDireita(NoAVL *r)
{ // r é o no desbalanceado
  NoAVL *y, *f;

  y = r->esquerda; // y ira receber o lado esquerdo de r e irá guardá-lo
  f = r->direita;  // f ira receber o lado direito de r e irá guardá-lo

  y->direita = r;  // a direita de y assumirá o valor de r
  r->esquerda = f; // a esquerda de r assumirá o valor de f

  r->altura = maior(altura(r->esquerda), altura(r->direita)) + 1; // atualiza a altura do no r
  y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1; // atualiza a altura do no y

  return y;
}

NoAVL *rotacaoEsquerda(NoAVL *r)
{
  NoAVL *y, *f;

  y = r->direita;  // y ira receber o lado direito de r e irá guardá-lo
  f = y->esquerda; // f ira receber o lado esquerdo de r e irá guardá-lo

  y->esquerda = r; // a esquerda de y assumirá o valor de r
  r->direita = f;  // a direita de r assumirá o valor de f

  y->altura = maior(altura(y->esquerda), altura(y->direita)) + 1; // atualiza a altura do no y
  r->altura = maior(altura(r->esquerda), altura(r->direita)) + 1; // atualiza a altura do no r

  return y; // return o no balanceado
}

NoAVL *rotacaoDireitaEsquerda(NoAVL *r)
{
  r->direita = rotacaoDireita(r->direita); // realiza uma rotação na direita de r
  return (rotacaoEsquerda(r));             // realiza uma rotação a esquerda de r
}

NoAVL *rotacaoEsquerdaDireita(NoAVL *r)
{
  r->esquerda = rotacaoEsquerda(r->esquerda); // realiza uma rotação na esquerda de r
  return (rotacaoDireita(r));                 // realiza uma rotação na direita de r
}

NoAVL *balancear(NoAVL *raiz)
{
  int fb = fatordeBalanceamento(raiz);

  // rotação a esquerda
  if (fb < -1 && fatordeBalanceamento(raiz->direita) <= 0)
  {
    raiz = rotacaoEsquerda(raiz);
  }
  // rotção a direita
  else if (fb > 1 && fatordeBalanceamento(raiz->esquerda) >= 0)
  {
    raiz = rotacaoDireita(raiz);
  }

  else if (fb > 1 && fatordeBalanceamento(raiz->esquerda) < 0)
  {
    raiz = rotacaoEsquerdaDireita(raiz);
  }

  else if (fb < -1 && fatordeBalanceamento(raiz->direita) > 0)
  {
    raiz = rotacaoDireitaEsquerda(raiz);
  }

  return raiz;
}

NoAVL *inserir(NoAVL *raiz, char *palavra, char *significado)
{
  if (raiz == NULL)
  {
    return novoNo(palavra, significado); // caso a raiz seja vazia retorna a função para criar um novoNo, inserindo na árvore
  }

  int compara = strcasecmp(palavra, raiz->palavra); // strcasecmp retornará um valor negativo(<0) caso a palavra a ser inserida venha antes de raiz->palavra e retorna um positivo(>0) caso venha depois de raiz->palavra EM ORDEM ALFABÉTICA e 0 caso sejam identicas

  if (compara < 0)
  {
    raiz->esquerda = inserir(raiz->esquerda, palavra, significado); // utilizara recursao para inserir a palvra á esquerda da raiz
  }
  else if (compara > 0)
  {
    raiz->direita = inserir(raiz->direita, palavra, significado); // utilizara recursao para inserir a palavra a direita da raiz
  }
  else
  {                                          // as palavras são iguais então atualiza o significado dela
    free(raiz->significado);                 // libera o significado da raiz
    raiz->significado = strdup(significado); // atualiza o significado da palavra
    printf("\nA palavra %s teve seu significado atualizado.\n", palavra);
    return raiz;
  }

  raiz->altura = maior(altura(raiz->esquerda), altura(raiz->direita)) + 1; // atualiza a altura da árvore
  return balancear(raiz);                                                  // balanceia a árvore após inserir ou modificar o significado da palavra
}

NoAVL *remover(NoAVL *raiz, char *palavra)
{
  if (raiz == NULL)
  {
    printf("A palavra %s não foi adicionada ao dicionário.\n", palavra);
    return NULL;
  }

  int compara = strcasecmp(palavra, raiz->palavra);

  if (compara < 0)
  {
    raiz->esquerda = remover(raiz->esquerda, palavra);
  }
  else if (compara > 0)
  {
    raiz->direita = remover(raiz->direita, palavra);
  }
  else
  { // palavra foi encontrada
    // caso 1: nó folha (sem filhos)
    if (raiz->esquerda == NULL && raiz->direita == NULL)
    {
      free(raiz->palavra);
      free(raiz->significado);
      free(raiz);
      printf("Palavra '%s' removida.\n", palavra);
      return NULL;
    }
    // caso 2: Nó com apenas um filho
    else if (raiz->esquerda == NULL || raiz->direita == NULL)
    {
      NoAVL *filho = (raiz->esquerda != NULL) ? raiz->esquerda : raiz->direita;
      free(raiz->palavra);
      free(raiz->significado);
      free(raiz);
      printf("Palavra '%s' removida.\n", palavra);
      return filho; // filho torna-se nova raiz
    } // caso 3: nó com dois filhos
    else
    {
      // encontra o maior nó da subárvore esquerda (antecessor)
      NoAVL *antecessor = raiz->esquerda;
      while (antecessor->direita != NULL) // vai acessando para pegar o nó que estiver mais a direita (o maior valor)
      {
        antecessor = antecessor->direita;
      }
      // copia os dados do antecessor para o nó atual
      free(raiz->palavra);
      free(raiz->significado);
      raiz->palavra = strdup(antecessor->palavra); // atualiza a raiz->palavra para receber o antecessor que seria o maior valor dentro da subarvore
      raiz->significado = strdup(antecessor->significado);
      // remove o antecessor (que agora está duplicado)
      raiz->esquerda = remover(raiz->esquerda, antecessor->palavra); // remove o antecessor(que agora é a raiz) que estava duplicado na arvore
      printf("Palavra '%s' substituída e removida.\n", palavra);
    }
  }

  // atualiza a altura do nó atual
  raiz->altura = 1 + maior(altura(raiz->esquerda), altura(raiz->direita));

  // balanceia a árvore
  return balancear(raiz);
};

void buscar(NoAVL *raiz, char *palavra)
{
  if (raiz == NULL)
  {
    printf("Palavra '%s' não encontrada no dicionário.\n", palavra);
    return;
  }

  int comparacao = strcasecmp(palavra, raiz->palavra);

  if (comparacao < 0) // semlhante as funções remover e inserir
  {
    buscar(raiz->esquerda, palavra);
  }
  else if (comparacao > 0)
  {
    buscar(raiz->direita, palavra);
  }
  else
  {
    printf("%s: %s\n", raiz->palavra, raiz->significado);
  }
}

void listarEmOrdem(NoAVL *raiz) // função que lista a arvore(dicionario) em ordem
{
  if (raiz != NULL)
  {
    listarEmOrdem(raiz->esquerda);
    printf("%s: %s\n", raiz->palavra, raiz->significado);
    listarEmOrdem(raiz->direita);
  }
}

void liberarArvore(NoAVL *raiz) // função para evitar memory leak libera a arvore
{
  if (raiz != NULL)
  {
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz->palavra);
    free(raiz->significado);
    free(raiz);
  }
}

void exibirMenu() // função que exibe o menu
{
  printf("\n--- Dicionário com AVL ---\n");
  printf("1. Inserir palavra\n");
  printf("2. Buscar palavra\n");
  printf("3. Listar todas as palavras\n");
  printf("4. Remover palavra\n");
  printf("5. Sair\n");
  printf("Escolha uma opção: ");
}

int main()
{
  NoAVL *raiz = NULL;
  int opcao;
  char palavra[50];
  char significado[200];

  do
  {
    exibirMenu();
    scanf("%d", &opcao);
    getchar(); // limpa o buffer do teclado (remove o \n) para não atrapalhar o fgets

    switch (opcao)
    {
    case 1:
      printf("Digite a palavra: ");
      fgets(palavra, sizeof(palavra), stdin);
      palavra[strcspn(palavra, "\n")] = '\0'; // remove o \n que o fgets grava e substitui por \0 encerrando a string

      printf("Digite o significado: ");
      fgets(significado, sizeof(significado), stdin);
      significado[strcspn(significado, "\n")] = '\0';

      raiz = inserir(raiz, palavra, significado);
      break;

    case 2:
      printf("Digite a palavra a ser buscada: ");
      fgets(palavra, sizeof(palavra), stdin);
      palavra[strcspn(palavra, "\n")] = '\0';

      buscar(raiz, palavra);
      break;

    case 3:
      printf("\n--- Palavras no dicionário (Palavra: Significado) ---\n");
      listarEmOrdem(raiz);
      break;

    case 4:
      printf("Digite a palavra a ser removida: ");
      fgets(palavra, sizeof(palavra), stdin);
      palavra[strcspn(palavra, "\n")] = '\0';

      raiz = remover(raiz, palavra);
      break;

    case 5:
      printf("Saindo...\n");
      break;

    default:
      printf("Opção inválida!\n");
    }
  } while (opcao != 5);

  liberarArvore(raiz); // libera a memoria alocada para a arvore

  return 0;
}