#include <stdio.h>   // Biblioteca para operações de entrada/saída (ex: fopen, fprintf)
#include <stdlib.h>  // Biblioteca para funções gerais (ex: rand, srand)
#include <time.h>    // Biblioteca para funções de tempo (ex: time)

// Função para gerar números aleatórios e salvar em um arquivo
void gerarNumerosAleatorios(int quantidade)
{
  // Abre o arquivo "entrada.txt" para escrita ("w" = write)
  FILE *arquivo = fopen("entrada.txt", "w");
  
  // Verifica se o arquivo foi aberto corretamente
  if (arquivo == NULL)
  {
    // Imprime mensagem de erro no console de erro (stderr)
    fprintf(stderr, "Erro ao abrir o arquivo para escrita.\n");
    return;  // Encerra a função em caso de erro
  }

  // Inicializa o gerador de números aleatórios com uma "semente" baseada no tempo atual
  // Isso garante que a sequência de números seja diferente a cada execução
  srand(time(NULL));

  // Loop para gerar a quantidade especificada de números
  for (int i = 0; i < quantidade; i++)
  {
    // Gera um número aleatório entre 0 e 10000 (rand() % 10001)
    int numero = rand() % 10001;
    
    // Escreve o número no arquivo, seguido de uma quebra de linha
    fprintf(arquivo, "%d\n", numero);
  }

  // Fecha o arquivo após a escrita
  fclose(arquivo);
  
  // Mensagem de confirmação no console
  printf("Arquivo 'entrada.txt' gerado com sucesso.\n");
}

// Função principal
int main()
{
  // Chama a função para gerar 500 números aleatórios
  gerarNumerosAleatorios(500);
  
  return 0;  // Retorna 0 indicando execução bem-sucedida
}