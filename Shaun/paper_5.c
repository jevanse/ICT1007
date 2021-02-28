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

int main(int argc, char *argv[]) {
	Processes * processes = (Processes *) malloc(sizeof(Processes));
	init_processes(processes);
	//init(processes);
	//improved_round_robin(processes, 500);
	//print_results(processes);
	
	//int quantum = generate_time_quantum(processes); //generated time quantum can be int for now
	
	if (argc == 1) // command line mode
	{
		//take in processes
		int number_of_processes, time_quantum;

		printf("Number of processes? ");
		scanf("%d", &number_of_processes);

		for (int i = 0; i < number_of_processes; i++)
		{
			Process * new_process = (Process*) calloc(0, sizeof(Process));
			printf("Burst time for process %d :", i+1);
			scanf("%d", &new_process->burst_time);

			printf("Arrival time for process %d :", i+1);
			scanf("%d", &new_process->arrival_time);

			printf("\nPriority for processes must be 1, 2 or 3\n");
			printf("Priority time for process %d :", i+1);
			scanf("%d", &new_process->priority);

			new_process->cpu_time = new_process->burst_time;
			new_process->pid = i + 1;

			insert_node(processes, new_process);
		}
		
		print_results(processes);
		processes->size = number_of_processes;
		printf("Enter time quantum: ");
		scanf("%d", &time_quantum);

		improved_round_robin(processes, time_quantum);

		print_results(processes);
		return 0;
	}
	else if (argc != 2)
	{
		printf("Usage: paper_5 <file_name>");
		return -1;
		// generate time quantum that will be used 
	}
	
}

int improved_round_robin(Processes * processes, int quantum)
{
	//iterate through processes
	irr_process * current = (irr_process* )malloc(sizeof(irr_process));
	irr_process * tmp = (irr_process* )malloc(sizeof(irr_process));
	int time_elapsed = 0, time_at_start_of_quantum = 0, time_quantum = 0, number_of_completed_processes = 0, context_switches = 0;
	//irr_process * process_queue = init_queue(queue_size);
	irr_process * head = init_irr_processes(processes->head, processes->size);
	
	generate_time_quantum(head, quantum);


	while (number_of_completed_processes != processes->size)
	{
		//iterate through processes
		current = head; //reset to head
		
		while (current)
		{
			//check for idle
			time_quantum = 0;
			check_for_idling(head, &time_elapsed);
			time_at_start_of_quantum = time_elapsed;
			tmp = current;
			if (current->done == true || current->arrival_time > time_elapsed)
			{
				current = current->next;
				continue;
			}
			// check for very short burst times
			current = check_for_short_burst_time_processes(head, time_elapsed);
			if (current == NULL)
			{
				current = tmp;
			}

			if (current->burst_time < current->time_quantum) 
				time_quantum = current->burst_time;

			else if (current->burst_time > current->time_quantum && 
					current->burst_time <= (current->time_quantum * 1.2) &&
					(current->priority == LOW_PRIORITY || current->priority == MED_PRIORITY))		
					time_quantum = current->burst_time;
					 
			else if (current->burst_time > current->time_quantum && 
					current->burst_time <= current->time_quantum * 1.3 &&
					current->priority == HIGH_PRIORITY)
				time_quantum = current->burst_time;
			else 
				time_quantum = current->time_quantum;

			context_switches++;
			time_elapsed += time_quantum;
			current->burst_time -= time_quantum;

			if (current->first != true && current->burst_time < current->cpu_time)
			{
				current->response_time = time_at_start_of_quantum - current->arrival_time;
				current->first = true;
			}
			

			if (current->burst_time <= 0) // technically remaining burst_time can only go down to 0, this is to catch edge cases I guess
			{
				set_completed_process_properties(processes->head, current, time_elapsed);
				number_of_completed_processes++;
				current = current->next;
				continue;
			}
			current = current->next;
		}
	}
	processes->context_switches = context_switches;
	return 0;
}


	// //Process * current = processes->head;
	// irr_process * current = (irr_process* )malloc(sizeof(irr_process));
	// irr_process * tmp = (irr_process* )malloc(sizeof(irr_process));
	// int time_elapsed = 0, time_at_start_of_quantum = 0, time_quantum, queue_front = -1, queue_rear = -1, queue_size = 20, i, number_of_completed_processes = 0;
	// irr_process * process_queue = init_queue(queue_size);
	// irr_process * head = init_irr_processes(processes->head, 500, processes->size);
	// generate_time_quantum(head, quantum);
	
	// while (number_of_completed_processes != processes->size)
	// {
	// 	//check processes
	// 	for (i = queue_front; i <= queue_rear; i++) //iterate through each item in the queue
	// 	{
	// 		print_processes_in_queue(process_queue, queue_front, queue_rear);
	// 		time_quantum = 0; // reset on every loop as it is dynamic
	// 		time_at_start_of_quantum = time_elapsed;
	// 		check_queue_for_low_burst_time_proc(process_queue, &queue_front, &queue_rear, queue_size, quantum);

	// 		current = dequeue(process_queue, &queue_front, &queue_rear);

	// 		if (current == NULL)
	// 			printf("Current is NULL\n");

	// 		if (current == NULL && number_of_completed_processes != processes->size)  
	// 			{
	// 				add_arriving_process(head, process_queue, &time_elapsed, time_at_start_of_quantum, &queue_front, &queue_rear, queue_size);
	// 				time_at_start_of_quantum = time_elapsed;
	// 				continue;
	// 			}

	// 		if (current->burst_time < current->time_quantum) 
	// 			time_quantum = current->burst_time;

	// 		else if (current->burst_time > current->time_quantum && 
	// 				current->burst_time <= (current->time_quantum * 1.2) &&
	// 				(current->priority == LOW_PRIORITY || current->priority == MED_PRIORITY))
	// 				{
	// 					time_quantum = current->burst_time;
	// 					printf("Time quantum set because of ctx switch prevention: %d\n", time_quantum);
	// 				}

	// 		else if (current->burst_time > current->time_quantum && 
	// 				current->burst_time <= quantum * 1.3 &&
	// 				current->priority == HIGH_PRIORITY)
	// 			time_quantum = current->burst_time;
	// 		else 
	// 			time_quantum = current->time_quantum;
			
	// 		time_elapsed += time_quantum;
	// 		printf("Time elapsed: %d\n", time_elapsed);
	// 		current->burst_time -= time_quantum;

	// 		if (current->burst_time <= 0) // technically remaining burst_time can only go down to 0, this is to catch edge cases I guess
	// 		{
	// 			set_completed_process_properties(processes->head, current, time_elapsed);
	// 			number_of_completed_processes++;
	// 			printf("P%d exiting, tat->%d, wt->%d\n", current->pid, current->turnaround_time, current->waiting_time);
	// 		}
	// 		//check for other processes that may have arrived during execution of current process

	// 		// irr_process * arriving_process = head;
	// 		// while (arriving_process != NULL)
	// 		// {
	// 		// 	if (arriving_process->arrival_time <= time_elapsed && arriving_process->arrival_time > time_at_start_of_quantum)
	// 		// 	{		
	// 		// 		enqueue(arriving_process, process_queue, &queue_front, &queue_rear, queue_size);
	// 		// 	}
	// 		// 	arriving_process = arriving_process->next;
	// 		// }

	// 		if (current->burst_time > 0) // "short" remaining burst time has already been checked hence it is ok to assume these will run
	// 		{
	// 			//re-add to queue
	// 			enqueue(current, process_queue, &queue_front, &queue_rear, queue_size);
	// 		}
			
	// 		print_processes_in_queue(process_queue, queue_front, queue_rear);

	// 	}
	// 	if (time_elapsed > 5600)
	// 		return 0;
	// }