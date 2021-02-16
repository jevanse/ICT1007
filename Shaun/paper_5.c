#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "..\test_related\algorithm_testing.h"


#define LOW_PRIORITY 1
#define MED_PRIORITY 2
#define HIGH_PRIORITY 3

void swap(Process *xp, Process *yp)  
{  
    Process temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  
  
// A function to implement bubble sort  
void bubbleSort(Process arr[], int n)  
{  
    int i, j;  
    for (i = 0; i < n-1; i++)      
      
    // Last i elements are already in place  
    for (j = 0; j < n-i-1; j++)  
        if (arr[j].burst_time > arr[j+1].burst_time)  
            swap(&arr[j], &arr[j+1]);  
}  

int init(Processes * processes) 
{
	init_processes(processes);
    
    Process *process_1 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_2 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_3 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_4 = (Process*) calloc(1,(sizeof(Process)));

    process_1->arrival_time = 0;
		process_1->pid = 1;
		process_1->burst_time = 40;
		process_1->cpu_time = process_1->burst_time;
		process_1->priority = 3;
		process_1->completed = false;
        
		process_2->arrival_time = 0;
		process_2->pid = 2;
		process_2->burst_time = 35;
		process_2->cpu_time = process_2->burst_time;
		process_2->priority = 2;
		process_2->completed = false;	

		process_3->arrival_time = 0;
		process_3->pid = 3;
		process_3->burst_time = 30;
		process_3->priority = 1;
		process_3->cpu_time = process_3->burst_time;
		process_3->completed = false;

		process_4->arrival_time = 0;
		process_4->pid = 4;
		process_4->burst_time = 100;
		process_4->cpu_time = process_4->burst_time;
		process_4->priority = 3;
    process_4->completed = false;

    insert_node(processes, process_1);
    insert_node(processes, process_2);
    insert_node(processes, process_3);
    insert_node(processes, process_4);
		return 0;
}

int generate_time_quantum(Processes * processes)
{
	int i = 0;
	int quantum = 0;
	Process * temp_copy = (Process *) malloc(sizeof(Process) * processes->size);
	Process * current = processes->head;
	
	while (current != NULL) //make a copy of the linked list
	{
		memcpy(&temp_copy[i++], current, sizeof(Process));
		current = current->next;
	}
	bubbleSort(temp_copy, processes->size);
	// find the median
	if (processes->size % 2 == 0)
	{
		quantum = (int) (temp_copy[processes->size /2 - 1].burst_time + temp_copy[processes->size /2].burst_time) / 2;
	}
	else
	{
		quantum = (int) temp_copy[processes->size /2].burst_time;
	}

	free(temp_copy);
	free(current);
	return quantum;
}


int improved_round_robin(Processes * processes, int quantum)
{
	Process * current = processes->head;
	int time_elapsed, time_quantum;
	bool done;

	//TODO: Make a queue - processes that are ready
	// 			- Check time elapsed against arrival time, 
	// 			- If process has arrived, add to queue (check at the end of time slice)

	for(;;)
	{
		done = true;
		while (current != NULL) // end of linked list
		{
			if (current->completed) 
			{
				current = current->next; //skip to next element
				continue;
			}
			else done = false;

			time_quantum = quantum; //quantum should always reset
			if (current->burst_time < time_quantum) time_quantum = current->burst_time;
			if (current->burst_time < quantum * 1.2 && (current->priority == LOW_PRIORITY || current->priority == MED_PRIORITY))
			{
				//don't overwrite the quantum passed in
				time_quantum = current->burst_time;
			}
			else if (current->burst_time < quantum * 1.3 && current->priority == HIGH_PRIORITY)
			{
				time_quantum = current->burst_time;
			}

			time_elapsed += time_quantum;
			current->burst_time -= time_quantum;

			if (current->burst_time <= 0)
			{
				current->burst_time = 0;
				current->completed = true;
				current->turnaround_time = time_elapsed - current->arrival_time;
				current->waiting_time = current->turnaround_time - current->cpu_time;
			}
			current = current->next;
		}
		if (done == true) break;
	}
	return 0;
}




int main(void) {
  // higher priority value denotes higher priority
	Processes * processes = (Processes *) malloc(sizeof(Processes));
	init(processes);
	Process * current = processes->head;

	int quantum = generate_time_quantum(processes); //generated time quantum can be int for now
	printf("Quantum Generated: %d\n", quantum);
	improved_round_robin(processes, quantum);

	while (current != NULL)
	{
		printf("P%d TurnaroundTime: %d\n", current->pid, current->turnaround_time);
		printf("P%d WaitingTime: %d\n", current->pid, current->waiting_time);
		current = current->next;

	}


	return 0;



}