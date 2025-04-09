#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "stats.h"

void calculate_statistics(Process *plist, int n, int total_time) {
    int total_waiting = 0;
    int total_turnaround = 0;
    int deadline_misses = 0;
    int total_cpu_time = 0;
    int periodic_processes = 0;
    int aperiodic_processes = 0;
    
    // Calcular métricas por processo
    for (int i = 0; i < n; i++) {
        // Atualizar tempos caso não tenham sido calculados
        if (plist[i].finish_time == -1) {
            plist[i].finish_time = total_time;
            plist[i].turnaround_time = plist[i].finish_time - plist[i].arrival_time;
            plist[i].waiting_time = plist[i].turnaround_time - plist[i].burst_time;
        }
        
        total_waiting += plist[i].waiting_time;
        total_turnaround += plist[i].turnaround_time;
        total_cpu_time += plist[i].burst_time;
        
        if (plist[i].deadline > 0 && plist[i].finish_time > plist[i].deadline) {
            plist[i].deadline_missed = 1;
            deadline_misses++;
        }
        
        if (plist[i].is_periodic) {
            periodic_processes++;
        } else {
            aperiodic_processes++;
        }
    }
    
    // Calcular médias
    double avg_waiting = (double) total_waiting / n;
    double avg_turnaround = (double) total_turnaround / n;
    double cpu_util = 100.0 * (double) total_cpu_time / total_time;
    double throughput = (double) n / total_time;
    double deadline_miss_rate = (double) deadline_misses / n * 100.0;
    
    // Escrever relatório detalhado
    FILE *fp = fopen("output.txt", "w");
    if (!fp) {
        printf("Erro ao abrir ficheiro de saída.\n");
        return;
    }
    
    fprintf(fp, "📊 Relatório de Simulação 📊\n\n");
    fprintf(fp, "Algoritmo: %s\n", plist[0].algorithm_used);
    fprintf(fp, "Total de Processos: %d (Periódicos: %d, Não-periódicos: %d)\n", n, periodic_processes, aperiodic_processes);
    fprintf(fp, "Tempo Total de Simulação: %d unidades\n\n", total_time);
    
    fprintf(fp, "ID\tTipo\tChegada\tBurst\tPrior\tDeadline\tInício\tFim\tEspera\tTurnaround\tMissed\n");
    fprintf(fp, "----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\t%s\t%d\t%d\t%d\t%d\t\t%d\t%d\t%d\t%d\t\t%s\n",
                plist[i].id,
                plist[i].is_periodic ? "PER" : "APER",
                plist[i].arrival_time,
                plist[i].burst_time,
                plist[i].priority,
                plist[i].deadline,
                plist[i].start_time,
                plist[i].finish_time,
                plist[i].waiting_time,
                plist[i].turnaround_time,
                plist[i].deadline_missed ? "SIM" : "NÃO");
    }
    
    fprintf(fp, "\n📈 Métricas Gerais:\n");
    fprintf(fp, "--------------------------------\n");
    fprintf(fp, "Tempo Médio de Espera: %.2f\n", avg_waiting);
    fprintf(fp, "Tempo Médio de Retorno: %.2f\n", avg_turnaround);
    fprintf(fp, "Utilização da CPU: %.2f%%\n", cpu_util);
    fprintf(fp, "Throughput: %.2f processos/unidade\n", throughput);
    fprintf(fp, "Deadline Miss Rate: %.2f%% (%d misses)\n", deadline_miss_rate, deadline_misses);
    
    fclose(fp);
    
    // Mostrar resumo no console
    printf("\n📊 Resultados da Simulação:\n");
    printf("--------------------------------\n");
    printf("Tempo Médio de Espera: %.2f\n", avg_waiting);
    printf("Tempo Médio de Retorno: %.2f\n", avg_turnaround);
    printf("Utilização da CPU: %.2f%%\n", cpu_util);
    printf("Throughput: %.2f processos/unidade\n", throughput);
    printf("Deadline Miss Rate: %.2f%% (%d misses)\n", deadline_miss_rate, deadline_misses);
    printf("\nRelatório detalhado salvo em 'output.txt'\n");
}
