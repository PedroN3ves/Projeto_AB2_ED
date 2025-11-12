# Projeto Final da disciplina de Estrutura de Dados e Algoritmos

#### O projeto é dividido em 4 partes principais:
- Codificação de Huffman
- Resolvedor SAT
- Contagem e Plotagem comparando fila sem heap vs com heap
- Seminário sobre uma aplicação da estrutura AVL

## Codificação de Huffman

Esta parte do projeto implementa um compactador e descompactador de arquivos utilizando o algoritmo de Codificação de Huffman.

### Funcionalidades

1.  **Compressão (`compressao.c`, `utilitarios.c`)**
    * **Contagem de Frequência:** O programa lê o arquivo de entrada byte a byte e conta a frequência de cada byte, armazenando os resultados em uma lista encadeada (`add_atualizar`).
    * **Construção da Fila de Prioridade:** Uma fila de prioridade (implementada como um min-heap) é criada (`criar_heap`, `enqueue`) para armazenar os nós da árvore de Huffman, ordenados por frequência.
    * **Construção da Árvore de Huffman:** O algoritmo constrói a árvore de Huffman (`criar_arvore`) retirando os dois nós de menor frequência do heap, combinando-os em um novo nó "pai" (com a soma das frequências) e reinserindo o pai no heap. Isso se repete até restar apenas um nó (a raiz da árvore).
    * **Geração da Tabela de Códigos:** A árvore é percorrida recursivamente (`construir_tabela`) para gerar os códigos binários (prefix-free) para cada byte.
    * **Escrita do Arquivo Comprimido:**
        * Um cabeçalho de 2 bytes é escrito, contendo os bits de "lixo" no último byte e o tamanho da árvore serializada (`escrever_cabecalho`).
        * A árvore de Huffman é serializada em pré-ordem (`serializar_arvore`) e escrita no arquivo de saída. Isso é crucial para que o descompactador possa reconstruir a árvore.
        * Os dados do arquivo original são lidos novamente, e os códigos de Huffman correspondentes a cada byte são escritos bit a bit no arquivo de saída (`escrever_bits`).

2.  **Descompressão (`descompressao.c`)**
    * **Leitura do Cabeçalho:** O programa lê os 2 bytes de cabeçalho para extrair o número de bits de lixo (`lixo`) e o tamanho da árvore (`tam_arvore`).
    * **Desserialização da Árvore:** A árvore de Huffman é reconstruída a partir do arquivo de entrada lendo sua representação em pré-ordem (`desserializar_arvore`).
    * **Descompressão dos Dados:** O restante do arquivo é lido bit a bit. O programa percorre a árvore de Huffman (`atual = atual->right` ou `atual = atual->left`) até encontrar um nó folha. Ao encontrar uma folha, o byte correspondente é escrito no arquivo de saída e o percurso recomeça da raiz. O processo ignora os bits de "lixo" no final do arquivo.

### Como Executar

O arquivo `main.c` chama a função `menu()` (de `utilitarios.c`), que fornece uma interface de linha de comando simples para o usuário escolher entre:
* **[1] Compactar Arquivo:** Pede o nome do arquivo de entrada e gera um arquivo `.huff`.
* **[2] Descompactar Arquivo:** Pede o nome do arquivo `.huff` e a extensão desejada para o arquivo de saída (ex: `.txt`).
* **[3] Sair**.

---

## Resolvedor SAT

Esta seção implementa um resolvedor para o Problema de Satisfatibilidade Booleana (SAT) usando um algoritmo de backtracking recursivo.

### Funcionalidades

1.  **Leitura da Fórmula (`leitor_header.c`)**
    * O programa é projetado para ler fórmulas no formato padrão **DIMACS CNF**.
    * Ele ignora linhas de comentário (`c`) e processa a linha de problema (`p cnf [num_variaveis] [num_clausulas]`) para inicializar a estrutura da fórmula.
    * Cada cláusula (linhas terminadas em `0`) é lida e armazenada em uma lista encadeada de literais. Um exemplo de entrada é fornecido em `entrada.cnf`.

2.  **Algoritmo de Resolução (`operacoes_sat.c`)**
    * O resolvedor utiliza uma função recursiva `sat` que implementa o algoritmo de backtracking (DPLL simplificado).
    * **Casos Base:**
        * `implica_F(F, I)`: Verifica se a interpretação parcial `I` já satisfaz todas as cláusulas da fórmula `F`. Se sim, retorna `true`.
        * `implica_negF(F, I)`: Verifica se a interpretação parcial `I` já falsifica (torna impossível de satisfazer) alguma cláusula. Se sim, retorna `false` (ramo morto).
    * **Passo Recursivo:**
        * Uma variável indefinida (`xi`) é escolhida.
        * O algoritmo explora recursivamente os dois ramos:
            1.  `sat(F, I_true)` (onde `xi` é definido como `true`).
            2.  `sat(F, I_false)` (onde `xi` é definido como `false`).
        * O resultado do nó atual é a união (OU lógico) dos resultados dos seus filhos (`n->left->resultado || n->right->resultado`).

### Como Executar

O `main.c` lê a fórmula DIMACS da entrada padrão (stdin).
* Após a leitura (`gerar_formula_cnf`), ele chama o resolvedor `sat`.
* Se o resultado for `SAT`, ele imprime "SAT" e uma valoração que satisfaz a fórmula (usando `imprimir_primeiro_caminho_sat`).
* Se o resultado for `UNSAT`, ele imprime "UNSAT".

**Exemplo de uso:**
```bash
./sat < entrada.cnf
```

---

## Contagem e Plotagem comparando fila sem heap vs com heap

Esta parte do projeto compara experimentalmente o desempenho (número de comparações/trocas) da operação de inserção (`enqueue`) em duas implementações de fila de prioridade: uma baseada em lista encadeada ordenada e outra baseada em um max-heap.

### Implementações

1.  **Fila com Heap (`fila_com_heap.c`)**
    * Utiliza uma estrutura de heap (`EstruturaHeap`) onde o maior elemento está sempre na raiz.
    * A função `enqueueHeap` insere um novo elemento no final do array e realiza o "heapify-up" (sobe o elemento) para manter a propriedade do heap. Ela retorna o **número de trocas** (`trocas`) realizadas, que é proporcional a O(log n).

2.  **Fila sem Heap (`fila_sem_heap.c`)**
    * Utiliza uma lista encadeada (`FilaPrioridade`) mantida em ordem decrescente.
    * A função `enqueue` percorre a lista para encontrar a posição correta de inserção do novo elemento. Ela retorna o **número de comparações** (`comps`) realizadas, que no pior caso é O(n).

### Metodologia e Resultados

1.  O programa `valores_aleatorios.c` foi usado para gerar 500 números inteiros aleatórios e salvá-los em `entrada.txt`.
2.  O `main.c` (da pasta `ContagemPlotagem`) lê cada número de `entrada.txt`.
3.  Para cada número, ele o insere em ambas as filas (`fila` e `filaHeap`) e registra o número de comparações/trocas (`c1` e `c2`) no arquivo `comparacoes.csv`, junto com o tamanho atual da estrutura.
4.  O script `script.r` utiliza a biblioteca `ggplot2` para ler o `comparacoes.csv` e gerar um gráfico de dispersão comparando o "Número de Comparações" (eixo Y) pelo "Tamanho da Entrada" (eixo X) para ambas as estruturas.

### Conclusão (Gráfico)

O gráfico gerado (`Gráfico.png`) demonstra visualmente a superioridade da implementação com heap.

![Gráfico de Comparação](https://github.com/PedroN3ves/Projeto_AB2_ED/blob/main/ContagemPlotagem/images/Gr%C3%A1fico.png)

* **Com Heap (Azul):** O número de comparações (trocas) permanece muito baixo e cresce de forma logarítmica (O(log n)), parecendo quase constante no gráfico.
* **Sem Heap (Vermelho):** O número de comparações cresce de forma linear (O(n)) com o tamanho da entrada, resultando em um desempenho muito inferior à medida que a fila aumenta.

---

## Seminário sobre uma aplicação da estrutura AVL

Para demonstrar uma aplicação prática da árvore AVL, esta parte do projeto implementa um **Dicionário** (palavra-significado) que utiliza uma Árvore AVL para se manter balanceada.

### Funcionalidades (`dicionarioAvl.c`)

O arquivo `dicionarioAvl.c` contém a implementação completa.

1.  **Estrutura AVL:**
    * Define a `struct NoAVL`, que armazena `palavra`, `significado`, `altura`, e ponteiros para os filhos `esquerda` e `direita`.
    * Implementa as funções essenciais de balanceamento: `altura`, `fatordeBalanceamento`, `rotacaoDireita`, `rotacaoEsquerda`, `rotacaoDireitaEsquerda`, `rotacaoEsquerdaDireita`, e a função `balancear` que aplica a rotação correta com base no fator de balanceamento.

2.  **Operações do Dicionário:**
    * **Inserir:** A função `inserir` adiciona uma nova palavra e seu significado. Se a palavra já existe (strcasecmp == 0), ela atualiza o significado. Após a inserção, a altura do nó é atualizada e a árvore é balanceada.
    * **Remover:** A função `remover` busca a palavra e a remove usando as regras padrão de remoção em Árvore Binária de Busca (nó folha, nó com um filho, nó com dois filhos). No caso de dois filhos, ela substitui o nó pelo seu antecessor (maior da subárvore esquerda). Após a remoção, a árvore é balanceada.
    * **Buscar:** A função `buscar` procura pela palavra. Se encontrada, imprime a palavra e seu significado; caso contrário, informa que não foi encontrada.
    * **Listar em Ordem:** A função `listarEmOrdem` realiza um percurso em-ordem (esquerda, raiz, direita) na árvore, imprimindo todas as palavras do dicionário em ordem alfabética.

### Como Executar

O programa principal (`main`) fornece um `exibirMenu` interativo que permite ao usuário:
1.  Inserir palavra
2.  Buscar palavra
3.  Listar todas as palavras
4.  Remover palavra
5.  Sair

Ao sair, a função `liberarArvore` é chamada para desalocar toda a memória utilizada pela árvore.
