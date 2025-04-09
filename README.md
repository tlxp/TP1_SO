# TP1 - Simulador de Escalonamento de Processos

Este projeto é um simulador de algoritmos de escalonamento de processos, desenvolvido como parte da disciplina de Sistemas Operacionais.

## 📁 Estrutura do Projeto

- `main.c` – Ponto de entrada do programa.
- `input.c/h` – Leitura e tratamento de entrada de dados (processos).
- `scheduler.c/h` – Implementação dos algoritmos de escalonamento.
- `process_generator.c/h` – Geração e organização de processos.
- `stats.c/h` – Cálculo e exibição de estatísticas.
- `utils.c/h` – Funções auxiliares.
- `input.txt` – Arquivo de entrada contendo os processos a serem escalonados.
- `Makefile` – Automatiza a compilação do projeto.

## 🔧 Compilação

Compile o projeto com:

```bash
make
▶️ Execução
Execute o programa com:

bash
Copiar
Editar
./main input.txt
📄 Formato do Arquivo de Entrada
Cada linha do input.txt representa um processo com o seguinte formato:

php-template
Copiar
Editar
<tempo_chegada> <tempo_duracao> <prioridade> <deadline> <periodo>
Exemplo:

Copiar
Editar
0 5 2 10 0
2 3 1 12 0
4 2 3 15 0
Nota: Os campos extras são usados em algoritmos como Prioridade, EDF e Rate Monotonic.

🧠 Algoritmos Implementados
O simulador suporta os seguintes algoritmos de escalonamento:

🟢 FCFS (First-Come, First-Served)

🟢 SJF (Shortest Job First)

Versão preemptiva (sjf_preemptive)

Versão não-preemptiva (sjf)

🟢 Priority Scheduling

🟢 Round Robin

🟢 EDF (Earliest Deadline First)

🟢 Rate Monotonic

📊 Saída
A saída do programa inclui:

Ordem de execução dos processos

Tempos de início e término de cada processo

Tempo de espera médio

Tempo de turnaround médio

Utilização da CPU

👥 Autores
José Amoreira - 51712

Diogo Gomes - 51618

Tiago Valério - 52334

📌 Observações
Verifique se o arquivo input.txt está no mesmo diretório que o executável.

Algoritmos podem ser selecionados editando diretamente o main.c ou adicionando suporte a argumentos de linha de comando.
