#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../test_related/algorithm_testing.h"

void swap(Process*, Process*);
void bubbleSort(Process arr[], int);
int init(Processes * processes);
Process * init_queue(int);
int improved_round_robin(Processes * processes, int quantum);
bool check_process_execution(Processes *);
int generate_time_quantum(Processes * processes);
void enqueue(Process *, Process *, int * front, int * rear, int queue_size);
Process * dequeue(Process * process_queue, int * front, int * rear);
void print_processes_in_queue(Process * process_queue, int front, int rear);

