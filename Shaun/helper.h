#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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


void swap(Process*, Process*);
void bubbleSort(Process arr[], int);
int init(Processes * processes);
irr_process * init_irr_processes(Process * head, int number_of_processes);
irr_process * init_queue(int);
int improved_round_robin(Processes * processes, int quantum);
bool check_process_execution(Processes *);
void generate_time_quantum(irr_process * head, int time_quantum);
void enqueue(irr_process *, irr_process *, int * front, int * rear, int queue_size);
irr_process * check_for_short_burst_time_processes(irr_process * head,int time_elapsed);
void add_arriving_process(irr_process * head, irr_process * process_queue,int * time_elapsed, int time_at_start_of_quantum,int * front, int * back, int queue_size);
void check_queue_for_low_burst_time_proc(irr_process * process_queue, int * front, int * back, int queue_size,int time_quantum);
void set_completed_process_properties(Process * head, irr_process * process, int exit_time);
irr_process * dequeue(irr_process * process_queue, int * front, int * rear);
void check_for_idling(irr_process * head, int *time_elapsed);
void print_processes_in_queue(irr_process * process_queue, int front, int rear);
void print_results(Processes* processes);