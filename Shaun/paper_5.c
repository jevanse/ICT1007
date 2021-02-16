#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "paper_5.h"

/* EDGE CASE: 
	Arrival times are not sorted and the 2nd and 3rd node in the linked list is in the time window to be added
	but the, in theory if 2nd node AT > 3rd node AT, 3rd node should run first, but in this code the 2nd node gets
	added. 
*/
#define LOW_PRIORITY 1
#define MED_PRIORITY 2
#define HIGH_PRIORITY 3


int main(void) {
  // higher priority value denotes higher priority
	Processes * processes = (Processes *) malloc(sizeof(Processes));
	init(processes);
	Process * current = processes->head;

	int quantum = generate_time_quantum(processes); //generated time quantum can be int for now
	printf("Quantum Generated: %d\n", quantum);
	improved_round_robin(processes, quantum);

	current = processes->head;
	while (current != NULL)
	{
		printf("P%d TurnaroundTime: %d\n", current->pid, current->turnaround_time);
		printf("P%d WaitingTime: %d\n", current->pid, current->waiting_time);
		current = current->next;

	}
	return 0;
}

int improved_round_robin(Processes * processes, int quantum)
{
	Process * current = processes->head;
	int time_elapsed = 0, time_at_start_of_quantum = 0, time_quantum, queue_front = -1, queue_rear = -1, queue_size = 20, i;
	bool done;
	Process * process_queue = init_queue(queue_size);

	/* Add processes that arrive at time 0 to the queue */
	while (current != NULL)
	{
		
		if (current->arrival_time == 0) 
		{
			//printf("Current pid: %d, arrival time: %d\n", current->pid, current->arrival_time);
			enqueue(current, process_queue, &queue_front, &queue_rear, queue_size);

		}
		current = current->next;
	}

	for(;;)
	{
		done = true;
		for (i = queue_front; i <= queue_rear; i++) 
		{
			time_at_start_of_quantum = time_elapsed;
			current = dequeue(process_queue, &queue_front, &queue_rear);
			if (current == NULL) continue;
			time_quantum = quantum; //quantum should always reset'
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
				current->turnaround_time = time_elapsed - current->arrival_time;
				current->waiting_time = current->turnaround_time - current->cpu_time;
				Process * exiting_process = processes->head;

				while (exiting_process != NULL)
				{
					if (exiting_process->pid == current->pid)
					{
						exiting_process->turnaround_time = current->turnaround_time;
						exiting_process->waiting_time = current->waiting_time;
						break;
					}
					exiting_process = exiting_process->next;
				}
				//printf("P%d exiting, tat->%d, wt->%d\n", current->pid, current->turnaround_time, current->waiting_time);
			}
			//check for other processes that may have arrived
			Process * arriving_process = processes->head;
			while (arriving_process != NULL)
			{
				if (arriving_process->arrival_time <= time_elapsed && arriving_process->arrival_time > time_at_start_of_quantum)
				{		
					if (arriving_process->arrival_time <= time_elapsed) //assume for now that arrival times are sorted
					{
						//add these to the queue first as they arrive before the current time			
						enqueue(arriving_process, process_queue, &queue_front, &queue_rear, queue_size);
					}
				}
				arriving_process = arriving_process->next;
			}
			if (current->burst_time > 0) // "short" remaining burst time has already been checked hence it is ok to assume these will run
			{
				//re-add to queue
				enqueue(current, process_queue, &queue_front, &queue_rear, queue_size);
			}
			done = check_process_execution(processes); //will be set to false if there is still burst time remaining
			//print_processes_in_queue(process_queue, queue_front, queue_rear);
		}
		if (done == true) break;
	}
	return 0;
}

Process * init_queue(int queue_size)
{
	return (Process *) calloc(1, sizeof(Process) * queue_size);
}

void enqueue(Process * process, Process * process_queue, int * front, int * rear, int queue_size)
{
	if (*rear == queue_size - 1) printf("Queue is full\n");
	else 
	{
		if (*front == -1) *front = 0;
		*rear = *rear + 1;
		process_queue[*rear] = *process;
	}
}

Process * dequeue(Process * process_queue, int * front, int * rear)
{
	Process * tmp = (Process *) malloc(sizeof(Process));
	if (*front == -1) 
	{
		printf("Dequeue: Queue is empty\n");
		return NULL;
	}
	else
	{
		tmp = &process_queue[*front];
		*front = *front + 1;
		if (*front > *rear) *front = *rear = - 1;
	}
	return tmp;
}

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


int init(Processes * processes) //function for testing
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
        
		process_2->arrival_time = 0;
		process_2->pid = 2;
		process_2->burst_time = 35;
		process_2->cpu_time = process_2->burst_time;
		process_2->priority = 2;

		process_3->arrival_time = 40;
		process_3->pid = 3;
		process_3->burst_time = 30;
		process_3->priority = 1;
		process_3->cpu_time = process_3->burst_time;

		process_4->arrival_time = 0;
		process_4->pid = 4;
		process_4->burst_time = 100;
		process_4->cpu_time = process_4->burst_time;
		process_4->priority = 3;

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

bool check_process_execution(Processes * processes)
{
	Process * current = processes->head;

	while (current != NULL)
	{
		if (current->burst_time > 0) return false;
		current = current->next;
	}
	return true;
}

void print_processes_in_queue(Process * process_queue, int front, int rear)//debugging function
{
	printf("\nQueue: ");
	for (int i = front; i <= rear; i++)
	{
		printf("\tProcess %d ", process_queue[i].pid);
	}
	printf("\n\n");

}
