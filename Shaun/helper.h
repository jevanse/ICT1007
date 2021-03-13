#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../test_related/algorithm_testing.h"

#define LOW_PRIORITY 1
#define MED_PRIORITY 2
#define HIGH_PRIORITY 3

typedef struct irr_struct 
{
    int pid;
    int burst_time;
    int cpu_time; // Same as burst_time, but this variable will not be changed even if burst_time will be changed in the future
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int priority;
    int time_quantum;
    bool done;
    bool first;
    struct irr_struct *next;
} irr_process;


void swap(int*, int*);
void bubbleSort(int arr[], int n);
int init(Processes * processes);
irr_process * init_irr_processes(Process * head, int number_of_processes);
int improved_round_robin(Processes * processes, int quantum);
void generate_time_quantum(irr_process * head, int time_quantum);
irr_process * check_for_short_burst_time_processes(irr_process * head,int time_elapsed);
void set_completed_process_properties(Process * head, irr_process * process, int exit_time);
bool check_for_idling(irr_process * head, int *time_elapsed);
void print_results(Processes* processes);
int generate_dynamic_timequantum(Processes * processes);