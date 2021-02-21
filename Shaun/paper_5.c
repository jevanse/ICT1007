#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helper.h"
/*
Todo: 
	1) Copy from processes to own struct, as I require more variables - done
	2) Update generate time quantum function - done
	3) Account for edge case where there is a period of idling between processes
		-> If no processes are in queue, check for next earliest arrival time and set time elapsed to that - done
	4) Check all processes in ready queue and see if their remaining burst time = 20% * TQ, if they are, execute them first
	5) Test cases when theres idle time

*/

int main(void) {
  // higher priority value denotes higher priority
	Processes * processes = (Processes *) malloc(sizeof(Processes));
	init(processes);
	Process * current = processes->head;

	//int quantum = generate_time_quantum(processes); //generated time quantum can be int for now
	//printf("Quantum Generated: %d\n", quantum);
	
	improved_round_robin(processes, 0);

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
	
	//Process * current = processes->head;
	irr_process * current = (irr_process* )malloc(sizeof(irr_process));
	irr_process * tmp = (irr_process* )malloc(sizeof(irr_process));
	int time_elapsed = 0, time_at_start_of_quantum = 0, time_quantum, queue_front = -1, queue_rear = -1, queue_size = 20, i, number_of_completed_processes = 0;
	irr_process * process_queue = init_queue(queue_size);
	irr_process * head = init_irr_processes(processes->head, 500, processes->size);
	generate_time_quantum(head, 500);
	
	while (number_of_completed_processes != processes->size)
	{
		//check processes
		for (i = queue_front; i <= queue_rear; i++) //iterate through each item in the queue
		{
			print_processes_in_queue(process_queue, queue_front, queue_rear);
			time_quantum = 0; // reset on every loop as it is dynamic
			time_at_start_of_quantum = time_elapsed;

			//TODO 4: checking function should be here to move the selected node to the front

			current = dequeue(process_queue, &queue_front, &queue_rear);
			if (current == NULL)
				printf("Current is NULL\n");

			if (current == NULL && number_of_completed_processes != processes->size)  
				{
					add_arriving_process(head, process_queue, &time_elapsed, &queue_front, &queue_rear, queue_size);
					continue;
				}

			if (current->burst_time < current->time_quantum) 
				time_quantum = current->burst_time;

			else if (current->burst_time > current->time_quantum && 
					current->burst_time < current->time_quantum * 1.2 &&
					(current->priority == LOW_PRIORITY || current->priority == MED_PRIORITY))
				time_quantum = current->burst_time;

			else if (current->burst_time > current->time_quantum && 
					current->burst_time < quantum * 1.3 &&
					current->priority == HIGH_PRIORITY)
				time_quantum = current->burst_time;
			else 
				time_quantum = current->time_quantum;
			
			time_elapsed += time_quantum;
			printf("Time elapsed: %d\n", time_elapsed);
			current->burst_time -= time_quantum;

			if (current->burst_time <= 0) // technically remaining burst_time can only go down to 0, this is to catch edge cases I guess
			{
				set_completed_process_properties(processes->head, current, time_elapsed);
				number_of_completed_processes++;
				printf("P%d exiting, tat->%d, wt->%d\n", current->pid, current->turnaround_time, current->waiting_time);
			}
			//check for other processes that may have arrived during execution of current process

			irr_process * arriving_process = head;
			while (arriving_process != NULL)
			{
				if (arriving_process->arrival_time <= time_elapsed && arriving_process->arrival_time > time_at_start_of_quantum)
				{		
					enqueue(arriving_process, process_queue, &queue_front, &queue_rear, queue_size);
				}
				arriving_process = arriving_process->next;
			}

			if (current->burst_time > 0) // "short" remaining burst time has already been checked hence it is ok to assume these will run
			{
				//re-add to queue
				enqueue(current, process_queue, &queue_front, &queue_rear, queue_size);
			}
			
			print_processes_in_queue(process_queue, queue_front, queue_rear);

		}
	}
	 return 0;
}


// int logic()
// {
// 				Process * arriving_process = processes->head;
// 			while (arriving_process != NULL)
// 			{
// 				if (arriving_process->arrival_time <= time_elapsed && arriving_process->arrival_time > time_at_start_of_quantum)
// 				{		
// 					if (arriving_process->arrival_time <= time_elapsed) //assume for now that arrival times are sorted
// 					{
// 						//add these to the queue first as they arrive before the current time			
// 						enqueue(arriving_process, process_queue, &queue_front, &queue_rear, queue_size);
// 					}
// 				}
// 				arriving_process = arriving_process->next;
// 			}
// }