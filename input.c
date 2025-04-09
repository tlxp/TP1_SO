#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"

int read_input_file(const char *filename, SimConfig *config, Process **process_list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir ficheiro");
        return 0;
    }

    // Valores padrão
    strcpy(config->algorithm, "FCFS");
    strcpy(config->mode, "static");
    config->quantum = 2;
    config->num_processes = 0;
    strcpy(config->arrival_dist, "exponential");
    strcpy(config->burst_dist, "exponential");
    strcpy(config->priority_dist, "uniform");

    char line[256];
    int process_count = 0;
    int max_processes = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Ignorar comentários e linhas vazias
        if (line[0] == '#' || strlen(line) < 3)
            continue;

        // Ler parâmetros de configuração
        if (strncmp(line, "algorithm=", 10) == 0) {
            sscanf(line + 10, "%s", config->algorithm);
        } else if (strncmp(line, "mode=", 5) == 0) {
            sscanf(line + 5, "%s", config->mode);
        } else if (strncmp(line, "quantum=", 8) == 0) {
            sscanf(line + 8, "%d", &config->quantum);
        } else if (strncmp(line, "processes=", 10) == 0) {
            sscanf(line + 10, "%d", &config->num_processes);
            max_processes = config->num_processes;
            if (strcmp(config->mode, "static") == 0) {
                *process_list = malloc(max_processes * sizeof(Process));
            }
        } else if (strncmp(line, "arrival_dist=", 13) == 0) {
            sscanf(line + 13, "%s", config->arrival_dist);
        } else if (strncmp(line, "burst_dist=", 11) == 0) {
            sscanf(line + 11, "%s", config->burst_dist);
        } else if (strncmp(line, "priority_dist=", 14) == 0) {
            sscanf(line + 14, "%s", config->priority_dist);
        } else {
            // Ler processos se for modo estático
            if (strcmp(config->mode, "static") == 0 && process_count < max_processes) {
                Process *p = *process_list + process_count;
                int scanned = sscanf(line, "%d %d %d %d %d %d",
                    &p->id,
                    &p->arrival_time,
                    &p->burst_time,
                    &p->priority,
                    &p->deadline,
                    &p->period);

                // Definir valores padrão para campos não lidos
                if (scanned < 4) p->priority = 1;
                if (scanned < 5) p->deadline = p->arrival_time + p->burst_time + 10;
                if (scanned < 6) p->period = 0;
                
                p->is_periodic = (p->period > 0) ? 1 : 0;
                process_count++;
            }
        }
    }

    // Verificar se todos os parâmetros necessários foram lidos
    if (strcmp(config->mode, "random") == 0 && config->num_processes == 0) {
        printf("Erro: Número de processos não definido para modo random\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}
