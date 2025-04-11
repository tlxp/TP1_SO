#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "scheduler.h"
#include "process_generator.h"

// Estrutura para passar dados entre callbacks
typedef struct {
    GtkWidget *algorithm_combo;
    GtkWidget *mode_combo;
    GtkWidget *quantum_spin;
    GtkWidget *process_count_spin;
    GtkWidget *arrival_dist_combo;
    GtkWidget *burst_dist_combo;
    GtkWidget *priority_dist_combo;
    GtkWidget *window;
} AppWidgets;

// Função para executar a simulação
void run_simulation(SimConfig config) {
    Process *plist = NULL;
    
    // Gerar processos se for modo random
    if (strcmp(config.mode, "random") == 0) {
        plist = malloc(config.num_processes * sizeof(Process));
        generate_processes(plist, config.num_processes, config);
    } else {
        // Modo estático - ler do arquivo
        if (!read_input_file("input.txt", &config, &plist)) {
            printf("Erro ao ler ficheiro de entrada.\n");
            return;
        }
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
    }

    free(plist);
}

// Callback para o botão de iniciar simulação
void on_run_clicked(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    SimConfig config;
    
    // Obter valores dos widgets
    const gchar *algorithm = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->algorithm_combo));
    const gchar *mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->mode_combo));
    int quantum = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->quantum_spin));
    int process_count = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(widgets->process_count_spin));
    const gchar *arrival_dist = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->arrival_dist_combo));
    const gchar *burst_dist = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->burst_dist_combo));
    const gchar *priority_dist = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widgets->priority_dist_combo));
    
    // Preencher estrutura de configuração
    strncpy(config.algorithm, algorithm, sizeof(config.algorithm));
    strncpy(config.mode, mode, sizeof(config.mode));
    config.quantum = quantum;
    config.num_processes = process_count;
    strncpy(config.arrival_dist, arrival_dist, sizeof(config.arrival_dist));
    strncpy(config.burst_dist, burst_dist, sizeof(config.burst_dist));
    strncpy(config.priority_dist, priority_dist, sizeof(config.priority_dist));
    
    // Executar simulação
    run_simulation(config);
    
    // Mostrar mensagem de conclusão
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(widgets->window),
                                              GTK_DIALOG_DESTROY_WITH_PARENT,
                                              GTK_MESSAGE_INFO,
                                              GTK_BUTTONS_OK,
                                              "Simulação concluída!\nOs resultados foram salvos em 'output.txt'");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Callback para quando o modo é alterado
void on_mode_changed(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    const gchar *mode = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    
    // Habilitar/desabilitar controles com base no modo selecionado
    if (strcmp(mode, "random") == 0) {
        gtk_widget_set_sensitive(widgets->process_count_spin, TRUE);
        gtk_widget_set_sensitive(widgets->arrival_dist_combo, TRUE);
        gtk_widget_set_sensitive(widgets->burst_dist_combo, TRUE);
        gtk_widget_set_sensitive(widgets->priority_dist_combo, TRUE);
    } else {
        gtk_widget_set_sensitive(widgets->process_count_spin, FALSE);
        gtk_widget_set_sensitive(widgets->arrival_dist_combo, FALSE);
        gtk_widget_set_sensitive(widgets->burst_dist_combo, FALSE);
        gtk_widget_set_sensitive(widgets->priority_dist_combo, FALSE);
    }
}

// Callback para quando o algoritmo é alterado
void on_algorithm_changed(GtkWidget *widget, gpointer data) {
    AppWidgets *widgets = (AppWidgets *)data;
    const gchar *algorithm = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
    
    // Habilitar/desabilitar quantum para Round Robin
    if (strcmp(algorithm, "RR") == 0) {
        gtk_widget_set_sensitive(widgets->quantum_spin, TRUE);
    } else {
        gtk_widget_set_sensitive(widgets->quantum_spin, FALSE);
    }
}

// Função principal
int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button;
    
    AppWidgets widgets;
    
    gtk_init(&argc, &argv);
    
    // Criar janela principal
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulador de Escalonamento de Processos");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Criar grid para organizar os widgets
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);
    
    // Adicionar widgets ao grid
    
    // Algoritmo de escalonamento
    label = gtk_label_new("Algoritmo:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);
    
    widgets.algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "FCFS");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "SJF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "SJF_P");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "PRIO");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "PRIO_P");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "RR");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "EDF");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.algorithm_combo), "RM");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets.algorithm_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), widgets.algorithm_combo, 1, 0, 1, 1);
    g_signal_connect(widgets.algorithm_combo, "changed", G_CALLBACK(on_algorithm_changed), &widgets);
    
    // Modo de operação
    label = gtk_label_new("Modo:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);
    
    widgets.mode_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.mode_combo), "static");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.mode_combo), "random");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets.mode_combo), 0);
    gtk_grid_attach(GTK_GRID(grid), widgets.mode_combo, 1, 1, 1, 1);
    g_signal_connect(widgets.mode_combo, "changed", G_CALLBACK(on_mode_changed), &widgets);
    
    // Quantum (para Round Robin)
    label = gtk_label_new("Quantum (RR):");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);
    
    widgets.quantum_spin = gtk_spin_button_new_with_range(1, 100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets.quantum_spin), 2);
    gtk_widget_set_sensitive(widgets.quantum_spin, FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets.quantum_spin, 1, 2, 1, 1);
    
    // Número de processos (para modo random)
    label = gtk_label_new("Nº de Processos:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);
    
    widgets.process_count_spin = gtk_spin_button_new_with_range(1, 100, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widgets.process_count_spin), 5);
    gtk_widget_set_sensitive(widgets.process_count_spin, FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets.process_count_spin, 1, 3, 1, 1);
    
    // Distribuição de chegada (para modo random)
    label = gtk_label_new("Dist. Chegada:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
    
    widgets.arrival_dist_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.arrival_dist_combo), "exponential");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.arrival_dist_combo), "poisson");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.arrival_dist_combo), "uniform");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets.arrival_dist_combo), 0);
    gtk_widget_set_sensitive(widgets.arrival_dist_combo, FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets.arrival_dist_combo, 1, 4, 1, 1);
    
    // Distribuição de burst (para modo random)
    label = gtk_label_new("Dist. Burst:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 5, 1, 1);
    
    widgets.burst_dist_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.burst_dist_combo), "exponential");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.burst_dist_combo), "normal");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.burst_dist_combo), "uniform");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets.burst_dist_combo), 0);
    gtk_widget_set_sensitive(widgets.burst_dist_combo, FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets.burst_dist_combo, 1, 5, 1, 1);
    
    // Distribuição de prioridade (para modo random)
    label = gtk_label_new("Dist. Prioridade:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 6, 1, 1);
    
    widgets.priority_dist_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.priority_dist_combo), "uniform");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(widgets.priority_dist_combo), "weighted");
    gtk_combo_box_set_active(GTK_COMBO_BOX(widgets.priority_dist_combo), 0);
    gtk_widget_set_sensitive(widgets.priority_dist_combo, FALSE);
    gtk_grid_attach(GTK_GRID(grid), widgets.priority_dist_combo, 1, 6, 1, 1);
    
    // Botão para executar simulação
    button = gtk_button_new_with_label("Executar Simulação");
    g_signal_connect(button, "clicked", G_CALLBACK(on_run_clicked), &widgets);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 7, 2, 1);
    
    // Salvar referência para a janela principal
    widgets.window = window;
    
    // Mostrar todos os widgets
    gtk_widget_show_all(window);
    
    gtk_main();
    
    return 0;
}
