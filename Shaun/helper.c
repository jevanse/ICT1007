#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helper.h"

#define RETURN_ERROR -1

irr_process * init_queue(int queue_size)
{
	return (irr_process *) calloc(1, sizeof(irr_process) * queue_size);
    
}

irr_process * init_irr_processes(Process* head, int number_of_processes)
{
    //create a copy of the linked list and add properties that this algo requires
    Process * current = head;
    irr_process * tail = (irr_process*) calloc(1, sizeof(irr_process));
    irr_process * irr_head = (irr_process*) calloc(1, sizeof(irr_process)); // set all members to 0
    

    for (int i = 0; i < number_of_processes; i++)
    {
        if (i == 0)
        {
            //populate irr_head
            irr_head->pid = head->pid;
            irr_head->burst_time = head->burst_time;
            irr_head->cpu_time = head->cpu_time;
            irr_head->waiting_time = head->waiting_time;
            irr_head->arrival_time = head->arrival_time;
            irr_head->turnaround_time = head->turnaround_time;
            irr_head->priority = head->priority;
            tail = irr_head;
        }
        else 
        {
            irr_process * inserting_node = (irr_process*) calloc(1, sizeof(irr_process));

            inserting_node->pid = current->pid;
            inserting_node->burst_time = current->burst_time;
            inserting_node->cpu_time = current->cpu_time;
            inserting_node->waiting_time = current->waiting_time;
            inserting_node->arrival_time = current->arrival_time;
            inserting_node->turnaround_time = current->turnaround_time;
            inserting_node->priority = current->priority;
            tail->next = inserting_node;
            tail = inserting_node;
        }
        current = current->next;
        
    }
    return irr_head;
}


irr_process * check_for_short_burst_time_processes(irr_process * head,int time_elapsed)
{
    irr_process * current = head;

    while (current)
    {
        if (current->arrival_time <= time_elapsed && current->burst_time <= current->time_quantum * 0.2 && current->done == false) // means process has arrived
            return current;
        current = current->next;
    }
    return NULL;
}

void set_completed_process_properties(Process * head, irr_process * process, int exit_time)
{
    //calculate turnaroundtime, waiting time etc.
    process->turnaround_time = exit_time - process->arrival_time;
    process->waiting_time = process->turnaround_time - process->cpu_time;
    process->done = true;
    Process * current = head;
    while (current)
    {
        if (current->pid == process->pid)
        {
            current->turnaround_time = process->turnaround_time;
            current->waiting_time = process->waiting_time;
            current->response_time = process->response_time;
            break;
        }
        current = current->next;
    }
}


int generate_dynamic_timequantum(Processes * processes)
{
    Process * current = processes->head;
    int * burst_times = (int *) calloc(processes->size, sizeof(int));
    for (int i = 0; i < processes->size; i++)
    {
        burst_times[i] = current->burst_time;
        current = current->next;
        if (current == NULL)
            break;
    }
    // Sort burst times 
    
    bubbleSort(burst_times, processes->size);
    
    // Find median 
    int median = (processes->size * 0.8) -1;
    if (median <= -1) //catch impossible edge cases
    {
        median = 0;
    }
    
    return burst_times[median];
}

void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
// A function to implement bubble sort  
void bubbleSort(int arr[], int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++)      
      
    // Last i elements are already in place  
    for (j = 0; j < n-i-1; j++)  
        if (arr[j] > arr[j+1])  
            swap(&arr[j], &arr[j+1]);  
}  

void generate_time_quantum(irr_process * head, int time_quantum)
{
	irr_process * curr = head;
    while (curr)
    {
        if (curr->priority <= 0 || curr->priority > 3)
            curr->priority = 2;
        if (curr->priority == LOW_PRIORITY)
            curr->time_quantum = time_quantum * 0.8;
        else if (curr->priority == MED_PRIORITY)
            curr->time_quantum = time_quantum;
        else if (curr->priority == HIGH_PRIORITY)
            curr->time_quantum = time_quantum * 1.2;
        curr = curr->next;
    }
}

bool check_for_idling(irr_process * head, int *time_elapsed)
{
    irr_process * current = head;
    int time_skip = INT_MAX;
    bool return_val = false;
    while (current)
    {
        if (current->done)
        {
            current = current->next;
            continue;
        }
            
        if (current->arrival_time <= *time_elapsed) // if there is any process that arrives before the time elapsed, it is already in the ready queue
            return return_val;
        if (current->done == false && current->arrival_time < time_skip)
        {
            //set time_skip to smallest arrival time
            time_skip = current->arrival_time;
            return_val = true;
        }
        current = current->next;
    }

    *time_elapsed = time_skip; // edit time directly
    return return_val;
}

void print_results(Processes* processes)
{
    Process * current = processes->head;
    printf("| PID | Burst time | Arrival Time | Turn Around Time | Waiting Time | Response Time | Priority |\n");
	//iterate through processes and print results into a table
    while (current)
    {
        printf("| %3d | %10d | %12d | %16d | %12d | %13d | %8d |\n", current->pid, current->cpu_time, current->arrival_time, current->turnaround_time, current->waiting_time, current->response_time, current->priority);
        current = current->next;
    }
    printf("------------------------------------------------------------------------------------------------\n");
    printf("\nNumber of context switches : %d\n", processes->context_switches);
}

