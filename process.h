#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time; // Para algoritmos preemptivos
    int priority;
    int deadline;
    int period;
    int next_period; // Para processos periódicos
    
    // Estatísticas
    int start_time;
    int finish_time;
    int waiting_time;
    int turnaround_time;
    int deadline_missed;
    int is_periodic; // Flag para processos periódicos

    // Informação extra
    char algorithm_used[64]; // Nome do algoritmo usado (para output)
} Process;

#endif
