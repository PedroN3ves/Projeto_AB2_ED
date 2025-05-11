caminho <- file.choose()
# lê o arquivo comparacoes.csv do caminho indicado e cria um data frame chamado 'dados'
# os nomes das colunas são definidos manualmente
dados <- read.csv(caminho, 
                  header = TRUE, 
                  sep = "", #o R analisará o que separa os dados do arquivo
                  col.names = c("Comparacao_sem_heap", "Comparacao_com_heap", "Tamanho"))

# carrega o pacote ggplot2, que é usado para criar gráficos de forma mais flexível e bonita
library(ggplot2)

# inicia a construção do gráfico usando o data frame 'dados'
# define que o eixo x será baseado na coluna 'Tamanho'
ggplot(dados, aes(x = Tamanho)) +
  
  # adiciona os pontos para os dados da coluna Comparacao_sem_heap
  # com cor azul e um pouco de transparência
  geom_point(aes(y = Comparacao_sem_heap, color = "Com Heap"), 
             size = 4, alpha = 0.7) +
  
  # adiciona os pontos para os dados da coluna Comparacao_com_heap
  # com cor vermelha e o mesmo estilo visual
  geom_point(aes(y = Comparacao_com_heap, color = "Sem Heap"), 
             size = 4, alpha = 0.7) +
  
  # define os títulos do gráfico, dos eixos e da legenda de cores
  labs(title = "Comparação de Desempenho: Heap vs Sem Heap",
       x = "Tamanho da Entrada",
       y = "Número de Comparações",
       color = "Tipo de Estrutura") +
  
  # define manualmente as cores que serão usadas na legenda do gráfico
  scale_color_manual(values = c("Sem Heap" = "red", "Com Heap" = "blue")) +
  
  # aplica um tema mais limpo e moderno ao gráfico
  theme_minimal()
