#ifndef INPUT_H
#define INPUT_H

#include "process.h"

typedef struct {
    int num_processes;
    char mode[20];
    char algorithm[20];
    int quantum;
    
    // Novos membros adicionados para armazenar distribuições
    char arrival_dist[20];   // Para a distribuição de chegada
    char burst_dist[20];     // Para a distribuição de burst
    char priority_dist[20];  // Para a distribuição de prioridade
    
} SimConfig;

int read_input_file(const char *filename, SimConfig *config, Process **plist);

#endif

