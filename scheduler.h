#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

void fcfs(Process *p, int n);
void sjf(Process *p, int n);
void sjf_preemptive(Process *p, int n);
void priority_scheduling(Process *p, int n, int preemptive);
void round_robin(Process *p, int n, int quantum);
void edf(Process *p, int n);
void rate_monotonic(Process *p, int n);

#endif

