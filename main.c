#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "scheduler.h"
#include "input.h"
#include "process_generator.h"

int main(int argc, char *argv[]) {
    SimConfig config;
    Process *plist = NULL;
    
    // Inicializar random seed
    srand(time(NULL));
    
    // Ler ficheiro de configuração
    if (!read_input_file("input.txt", &config, &plist)) {
        printf("Erro ao ler ficheiro de entrada.\n");
        return 1;
    }
    
    // Gerar processos se for modo random
    if (strcmp(config.mode, "random") == 0) {
        plist = malloc(config.num_processes * sizeof(Process));
        generate_processes(plist, config.num_processes, config);
    }
    
    // Executar algoritmo selecionado
    if (strcmp(config.algorithm, "FCFS") == 0) {
        strcpy(plist[0].algorithm_used, "First-Come First-Served");
        fcfs(plist, config.num_processes);
    } else if (strcmp(config.algorithm, "SJF") == 0) {
        strcpy(plist[0].algorithm_used, "Shortest Job First");
        sjf(plist, config.num_processes);
    } else if (strcmp(config.algorithm, "SJF_P") == 0) {
        strcpy(plist[0].algorithm_used, "Shortest Job First (Preemptive)");
        sjf_preemptive(plist, config.num_processes);
    } else if (strcmp(config.algorithm, "PRIO") == 0) {
        strcpy(plist[0].algorithm_used, "Priority Scheduling");
        priority_scheduling(plist, config.num_processes, 0);
    } else if (strcmp(config.algorithm, "PRIO_P") == 0) {
        strcpy(plist[0].algorithm_used, "Priority Scheduling (Preemptive)");
        priority_scheduling(plist, config.num_processes, 1);
    } else if (strcmp(config.algorithm, "RR") == 0) {
        strcpy(plist[0].algorithm_used, "Round Robin");
        round_robin(plist, config.num_processes, config.quantum);
    } else if (strcmp(config.algorithm, "EDF") == 0) {
        strcpy(plist[0].algorithm_used, "Earliest Deadline First");
        edf(plist, config.num_processes);
    } else if (strcmp(config.algorithm, "RM") == 0) {
        strcpy(plist[0].algorithm_used, "Rate Monotonic");
        rate_monotonic(plist, config.num_processes);
    } else {
        printf("Algoritmo desconhecido: %s\n", config.algorithm);
        free(plist);
        return 1;
    }
    
    free(plist);
    return 0;
}
