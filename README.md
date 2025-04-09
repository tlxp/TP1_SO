# TP1_SO - Simulador de Escalonamento de Processos

Este projeto implementa um simulador de algoritmos de escalonamento de processos, incluindo:

- **FCFS** (First Come First Serve)
- **SJF** (Shortest Job First)
- **SJF Preemptivo**
- **Escalonamento por Prioridade** (com e sem preempção)
- **Round Robin**
- **EDF** (Earliest Deadline First)
- **Rate Monotonic**

## Estrutura do Projeto

- **main.c**: Interface principal que lê os dados dos processos e executa o algoritmo.
- **scheduler.c**: Implementações dos algoritmos de escalonamento.
- **process.c**: Estruturas e funções relacionadas aos processos.
- **stats.c**: Cálculo e exibição de estatísticas (tempo médio de espera, turnaround, etc.).
- **Makefile**: Arquivo para compilação do projeto.
- **README.md**: Instruções e explicações.
- **processes.txt**: Arquivo de entrada de exemplo com a definição dos processos.

## Compilação

Para compilar o projeto, utilize o comando:

```bash
make

Este comando irá compilar todos os arquivos fonte e gerar o executável do simulador.

Execução
Após a compilação, você pode executar o simulador com o seguinte comando:

bash
Copiar
Editar
./simulador
Certifique-se de que o arquivo processes.txt está no mesmo diretório que o executável, pois o simulador irá ler os dados dos processos a partir deste arquivo.

Formato do Arquivo de Entrada
O arquivo processes.txt deve seguir o seguinte formato:

plaintext
Copiar
Editar
# Configuração do Algoritmo e Processos
algorithm=FCFS                # Algoritmo de escalonamento
mode=static                   # Modo de operação: static ou random
quantum=4                     # Quantum para algoritmos que utilizam
processes=10                  # Número total de processos

# Definição dos Processos
# ID  Chegada  Burst  Prioridade  Deadline  Período
0  0      5      1          10        15        0
1  1      3      2          12        20        0
2  2      8      1          15        25        1
3  3      6      3          20        30        0
4  4      4      4          18        28        1
5  5      2      2          14        22        0
6  6      7      1          16        26        1
7  7      5      3          19        29        0
8  8      4      2          11        21        1
9  9      3      5          17        27        0
Resultados
Após a execução, o simulador irá gerar um relatório detalhado das estatísticas de desempenho dos processos, incluindo:

Tempo médio de espera

Tempo médio de turnaround

Taxa de utilização da CPU

Taxa de misses de deadline

Os resultados serão exibidos no console e também salvos em um arquivo chamado output.txt.
