#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "process.h"
#include "utils.h"
#include "input.h"



void generate_processes(Process *processes, int n, SimConfig config) {
    int current_arrival = 0;
    double lambda_arrival, lambda_burst;
    
    // Definir parâmetros baseados no config
    if (strcmp(config.arrival_dist, "exponential") == 0) {
        lambda_arrival = 0.5; // Valor padrão
    } else if (strcmp(config.arrival_dist, "poisson") == 0) {
        lambda_arrival = 1.0; // Valor padrão
    } else {
        lambda_arrival = 0.3; // Fallback
    }

    if (strcmp(config.burst_dist, "exponential") == 0) {
        lambda_burst = 0.3;
    } else if (strcmp(config.burst_dist, "normal") == 0) {
        lambda_burst = 5.0; // Média para normal
    } else {
        lambda_burst = 0.5; // Fallback
    }

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        
        // Gerar tempo de chegada
        if (strcmp(config.arrival_dist, "exponential") == 0) {
            current_arrival += (int) exponential_random(lambda_arrival);
        } else if (strcmp(config.arrival_dist, "poisson") == 0) {
            current_arrival += poisson_random(lambda_arrival);
        } else {
            current_arrival += random_int(1, 3); // Fallback uniforme
        }
        
        processes[i].arrival_time = current_arrival;
        
        // Gerar burst time
        if (strcmp(config.burst_dist, "exponential") == 0) {
            processes[i].burst_time = (int) exponential_random(lambda_burst) + 1;
        } else if (strcmp(config.burst_dist, "normal") == 0) {
            processes[i].burst_time = (int) fmax(1, normal_random(lambda_burst, 2.0));
        } else {
            processes[i].burst_time = random_int(1, 10); // Fallback uniforme
        }
        
        processes[i].remaining_time = processes[i].burst_time;
        
        // Gerar prioridade
        if (strcmp(config.priority_dist, "uniform") == 0) {
            processes[i].priority = random_int(1, 10);
        } else if (strcmp(config.priority_dist, "weighted") == 0) {
            // Prioridade mais baixa tem maior probabilidade
            int r = random_int(1, 100);
            if (r < 40) processes[i].priority = 1;
            else if (r < 70) processes[i].priority = 2;
            else if (r < 85) processes[i].priority = 3;
            else if (r < 95) processes[i].priority = 4;
            else processes[i].priority = 5;
        } else {
            processes[i].priority = random_int(1, 5); // Fallback
        }
        
        // Definir deadline e período para processos periódicos
        processes[i].is_periodic = (random_int(1, 100) < 30 ? 1 : 0); // 30% chance de ser periódico
        if (processes[i].is_periodic) {
            processes[i].period = random_int(10, 30);
            processes[i].deadline = processes[i].arrival_time + processes[i].period;
        } else {
            processes[i].period = 0;
            processes[i].deadline = processes[i].arrival_time + processes[i].burst_time + random_int(5, 15);
        }
        
        // Inicializar estatísticas
        processes[i].start_time = -1;
        processes[i].finish_time = -1;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].deadline_missed = 0;
    }
}
