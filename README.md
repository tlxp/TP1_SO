
# TP1 - Simulador de Escalonamento de Processos

Este projeto é um simulador de algoritmos de escalonamento de processos, desenvolvido como parte da disciplina de Sistemas Operacionais.

Agora com suporte a **interface gráfica (GTK+ 3)**!

---

## 📁 Estrutura do Projeto

- `main.c` – Versão CLI (linha de comando) do simulador.
- `main_gui.c` – Versão com interface gráfica GTK.
- `input.c/h` – Leitura e tratamento de entrada de dados (processos).
- `scheduler.c/h` – Implementação dos algoritmos de escalonamento.
- `process_generator.c/h` – Geração e organização de processos.
- `stats.c/h` – Cálculo e exibição de estatísticas.
- `utils.c/h` – Funções auxiliares.
- `input.txt` – Arquivo de entrada contendo os processos a serem escalonados.
- `output.txt` – Arquivo com os resultados da simulação.
- `Makefile` – Automatiza a compilação da versão CLI.
- `README.md` – Este documento.

---

## 🔧 Compilação

### 📦 Interface Gráfica (GTK+ 3)

Requisitos:

```bash
sudo apt-get install libgtk-3-dev
```

Compilação:

```bash
gcc -o scheduler_gui main_gui.c process_generator.c scheduler.c stats.c input.c utils.c -lm `pkg-config --cflags --libs gtk+-3.0`
```

### 💻 Versão CLI

Para compilar via Makefile:

```bash
make
```

---

## ▶️ Execução

### 🖥️ Modo Gráfico (GUI)

```bash
./scheduler_gui
```

### 📂 Modo Linha de Comando

```bash
./main input.txt
```

---

## 📄 Formato do Arquivo de Entrada

Cada linha do `input.txt` representa um processo com o seguinte formato:

```txt
<tempo_chegada> <tempo_duracao> <prioridade> <deadline> <periodo>
```

Exemplo:

```
0 5 2 10 0
2 3 1 12 0
4 2 3 15 0
```

> ⚠️ Os campos `deadline` e `periodo` são usados em algoritmos como EDF e Rate Monotonic.

---

## 🧠 Algoritmos Implementados

- FCFS (First-Come, First-Served)
- SJF (Shortest Job First)
  - Preemptivo (SJF_P)
  - Não-Preemptivo
- Priority Scheduling
  - Preemptivo (PRIO_P)
  - Não-Preemptivo
- Round Robin (RR)
- EDF (Earliest Deadline First)
- Rate Monotonic (RM)

---

## 📊 Saída

A saída da simulação inclui:

- Ordem de execução dos processos
- Tempos de início e término
- Tempo de espera médio
- Tempo de turnaround médio
- Utilização da CPU

Resultados são gravados no arquivo:

```
output.txt
```

---

## 👥 Autores

- José Amoreira – 51712  
- Diogo Gomes – 51618  
- Tiago Valério – 52334

---

## 📌 Observações

- Certifique-se de que o `input.txt` esteja no mesmo diretório do executável.
- No modo gráfico, selecione o algoritmo, modo de operação (estático ou aleatório) e distribuições via interface.
- No modo CLI, o algoritmo deve ser definido manualmente no código (`main.c`), caso não use a GUI.

---
