#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "process.h"
#include "scheduler.h"
#include "stats.h"

// Funções de comparação
int compare_arrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

int compare_burst(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->burst_time - p2->burst_time;
}

int compare_priority(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->priority - p2->priority;
}

int compare_deadline(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->deadline - p2->deadline;
}

int compare_period(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->period - p2->period;
}

// Implementação dos algoritmos

void fcfs(Process *p, int n) {
    qsort(p, n, sizeof(Process), compare_arrival);
    int current_time = 0;

    for (int i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time)
            current_time = p[i].arrival_time;

        p[i].start_time = current_time;
        p[i].finish_time = current_time + p[i].burst_time;
        p[i].waiting_time = p[i].start_time - p[i].arrival_time;
        p[i].turnaround_time = p[i].finish_time - p[i].arrival_time;
        current_time = p[i].finish_time;
        
        // Verificar deadline
        if (p[i].deadline > 0 && p[i].finish_time > p[i].deadline)
            p[i].deadline_missed = 1;
    }

    calculate_statistics(p, n, current_time);
}

void sjf(Process *p, int n) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && remaining_time[i] > 0 && 
                remaining_time[i] < min_burst) {
                min_burst = remaining_time[i];
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        if (remaining_time[shortest] == p[shortest].burst_time) {
            p[shortest].start_time = time;
        }

        remaining_time[shortest]--;
        time++;

        if (remaining_time[shortest] == 0) {
            p[shortest].finish_time = time;
            p[shortest].turnaround_time = time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;
            completed++;
            
            if (p[shortest].deadline > 0 && p[shortest].finish_time > p[shortest].deadline)
                p[shortest].deadline_missed = 1;
        }
    }

    free(remaining_time);
    calculate_statistics(p, n, time);
}

void sjf_preemptive(Process *p, int n) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && remaining_time[i] > 0 && 
                remaining_time[i] < min_burst) {
                min_burst = remaining_time[i];
                shortest = i;
            }
        }

        if (shortest == -1) {
            time++;
            continue;
        }

        if (remaining_time[shortest] == p[shortest].burst_time) {
            p[shortest].start_time = time;
        }

        remaining_time[shortest]--;
        time++;

        if (remaining_time[shortest] == 0) {
            p[shortest].finish_time = time;
            p[shortest].turnaround_time = time - p[shortest].arrival_time;
            p[shortest].waiting_time = p[shortest].turnaround_time - p[shortest].burst_time;
            completed++;
            
            if (p[shortest].deadline > 0 && p[shortest].finish_time > p[shortest].deadline)
                p[shortest].deadline_missed = 1;
        }
    }

    free(remaining_time);
    calculate_statistics(p, n, time);
}

void priority_scheduling(Process *p, int n, int preemptive) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
    }

    while (completed < n) {
        int highest_priority = -1;
        int min_priority = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && remaining_time[i] > 0 && 
                p[i].priority < min_priority) {
                min_priority = p[i].priority;
                highest_priority = i;
            }
        }

        if (highest_priority == -1) {
            time++;
            continue;
        }

        if (remaining_time[highest_priority] == p[highest_priority].burst_time) {
            p[highest_priority].start_time = time;
        }

        if (preemptive) {
            remaining_time[highest_priority] = 0;
            time += remaining_time[highest_priority];
        } else {
            remaining_time[highest_priority]--;
            time++;
        }

        if (remaining_time[highest_priority] == 0) {
            p[highest_priority].finish_time = time;
            p[highest_priority].turnaround_time = time - p[highest_priority].arrival_time;
            p[highest_priority].waiting_time = p[highest_priority].turnaround_time - p[highest_priority].burst_time;
            completed++;
            
            if (p[highest_priority].deadline > 0 && p[highest_priority].finish_time > p[highest_priority].deadline)
                p[highest_priority].deadline_missed = 1;
        }
    }

    free(remaining_time);
    calculate_statistics(p, n, time);
}

void round_robin(Process *p, int n, int quantum) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    int *is_queued = malloc(n * sizeof(int));
    int queue[n], front = 0, rear = 0;

    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
        is_queued[i] = 0;
    }

    // Ordenar processos por tempo de chegada
    qsort(p, n, sizeof(Process), compare_arrival);

    // Adicionar primeiro processo à fila
    if (n > 0 && p[0].arrival_time <= time) {
        queue[rear++] = 0;
        is_queued[0] = 1;
    }

    while (completed < n) {
        if (front == rear) {
            time++;
            // Verificar se há novos processos que chegaram
            for (int i = 0; i < n; i++) {
                if (!is_queued[i] && p[i].arrival_time <= time) {
                    queue[rear++] = i;
                    is_queued[i] = 1;
                }
            }
            continue;
        }

        int current = queue[front++];
        is_queued[current] = 0;

        if (remaining_time[current] == p[current].burst_time) {
            p[current].start_time = time;
        }

        int time_slice = (remaining_time[current] < quantum) ? remaining_time[current] : quantum;
        remaining_time[current] -= time_slice;
        time += time_slice;

        // Verificar se há novos processos que chegaram durante o time slice
        for (int i = 0; i < n; i++) {
            if (!is_queued[i] && p[i].arrival_time <= time && i != current) {
                queue[rear++] = i;
                is_queued[i] = 1;
            }
        }

        if (remaining_time[current] > 0) {
            queue[rear++] = current;
            is_queued[current] = 1;
        } else {
            p[current].finish_time = time;
            p[current].turnaround_time = time - p[current].arrival_time;
            p[current].waiting_time = p[current].turnaround_time - p[current].burst_time;
            completed++;
            
            if (p[current].deadline > 0 && p[current].finish_time > p[current].deadline)
                p[current].deadline_missed = 1;
        }
    }

    free(remaining_time);
    free(is_queued);
    calculate_statistics(p, n, time);
}

void edf(Process *p, int n) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    Process *active_processes = malloc(n * sizeof(Process));
    int active_count = 0;

    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
        if (p[i].is_periodic) {
            p[i].next_period = p[i].arrival_time;
        }
    }

    while (completed < n) {
        // Atualizar processos ativos (incluindo periódicos)
        active_count = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].is_periodic) {
                // Verificar se é hora de um novo período
                if (time >= p[i].next_period) {
                    // Criar nova instância do processo periódico
                    active_processes[active_count] = p[i];
                    active_processes[active_count].arrival_time = p[i].next_period;
                    active_processes[active_count].deadline = p[i].next_period + p[i].period;
                    active_processes[active_count].next_period += p[i].period;
                    active_count++;
                }
            } else if (p[i].arrival_time <= time && remaining_time[i] > 0) {
                active_processes[active_count] = p[i];
                active_count++;
            }
        }

        if (active_count == 0) {
            time++;
            continue;
        }

        // Ordenar por deadline (EDF)
        qsort(active_processes, active_count, sizeof(Process), compare_deadline);

        // Executar o processo com deadline mais próximo
        int current_id = active_processes[0].id;
        int current_idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].id == current_id) {
                current_idx = i;
                break;
            }
        }

        if (current_idx == -1) {
            time++;
            continue;
        }

        if (remaining_time[current_idx] == p[current_idx].burst_time) {
            p[current_idx].start_time = time;
        }

        remaining_time[current_idx]--;
        time++;

        if (remaining_time[current_idx] == 0) {
            p[current_idx].finish_time = time;
            p[current_idx].turnaround_time = time - p[current_idx].arrival_time;
            p[current_idx].waiting_time = p[current_idx].turnaround_time - p[current_idx].burst_time;
            completed++;
            
            if (p[current_idx].deadline > 0 && p[current_idx].finish_time > p[current_idx].deadline)
                p[current_idx].deadline_missed = 1;
        }
    }

    free(remaining_time);
    free(active_processes);
    calculate_statistics(p, n, time);
}

void rate_monotonic(Process *p, int n) {
    int time = 0, completed = 0;
    int *remaining_time = malloc(n * sizeof(int));
    Process *active_processes = malloc(n * sizeof(Process));
    int active_count = 0;

    for (int i = 0; i < n; i++) {
        remaining_time[i] = p[i].burst_time;
        p[i].remaining_time = p[i].burst_time;
        if (p[i].is_periodic) {
            p[i].next_period = p[i].arrival_time;
        }
    }

    while (completed < n) {
        // Atualizar processos ativos (incluindo periódicos)
        active_count = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].is_periodic) {
                // Verificar se é hora de um novo período
                if (time >= p[i].next_period) {
                    // Criar nova instância do processo periódico
                    active_processes[active_count] = p[i];
                    active_processes[active_count].arrival_time = p[i].next_period;
                    active_processes[active_count].deadline = p[i].next_period + p[i].period;
                    active_processes[active_count].next_period += p[i].period;
                    active_count++;
                }
            } else if (p[i].arrival_time <= time && remaining_time[i] > 0) {
                active_processes[active_count] = p[i];
                active_count++;
            }
        }

        if (active_count == 0) {
            time++;
            continue;
        }

        // Ordenar por período (Rate Monotonic)
        qsort(active_processes, active_count, sizeof(Process), compare_period);

        // Executar o processo com menor período
        int current_id = active_processes[0].id;
        int current_idx = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].id == current_id) {
                current_idx = i;
                break;
            }
        }

        if (current_idx == -1) {
            time++;
            continue;
        }

        if (remaining_time[current_idx] == p[current_idx].burst_time) {
            p[current_idx].start_time = time;
        }

        remaining_time[current_idx]--;
        time++;

        if (remaining_time[current_idx] == 0) {
            p[current_idx].finish_time = time;
            p[current_idx].turnaround_time = time - p[current_idx].arrival_time;
            p[current_idx].waiting_time = p[current_idx].turnaround_time - p[current_idx].burst_time;
            completed++;
            
            if (p[current_idx].deadline > 0 && p[current_idx].finish_time > p[current_idx].deadline)
                p[current_idx].deadline_missed = 1;
        }
    }

    free(remaining_time);
    free(active_processes);
    calculate_statistics(p, n, time);
}

