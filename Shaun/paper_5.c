#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "helper.h"

int main(int argc, char *argv[]) {
	Processes * processes = (Processes *) calloc(1,sizeof(Processes));
	init_processes(processes);
	
	if (argc == 1) // command line mode
	{
		int number_of_processes, time_quantum = 0;

		//take in processes information from user
		printf("Number of processes? ");
		scanf("%d", &number_of_processes);

		printf("Time quantum (if tq = 0, the program will generate the time quantum) ?: ");
		scanf("%d", &time_quantum);

		for (int i = 0; i < number_of_processes; i++)
		{
			int tmp = 0;
			int pid = i + 1;
			Process * new_process = (Process*) calloc(1, sizeof(Process));
			printf("Burst time for process %d : ", pid);
			scanf("%d", &tmp);
			new_process->burst_time = tmp;

			printf("Arrival time for process %d : ", pid);
			scanf("%d", &tmp);
			new_process->arrival_time = tmp;

			printf("\nPriority for processes must be 1, 2 or 3\n");
			printf("Priority for process %d : ", pid);
			scanf("%d", &tmp);
			new_process->priority = tmp;

			new_process->cpu_time = new_process->burst_time;
			new_process->pid = pid;		
		
			insert_node(processes, new_process);
		}
		if (!time_quantum)
		{
			time_quantum = generate_dynamic_timequantum(processes);
			printf("time_quantum generated: %d\n", time_quantum);
		}
	
		improved_round_robin(processes, time_quantum);

		print_results(processes);
		return 0;
	}
	else if (argc == 2)
	{
		printf("File mode.\n");
		char * filename = argv[1];
		get_processes(filename, &processes);
		int time_quantum  = generate_dynamic_timequantum(processes);
		improved_round_robin(processes, time_quantum);
		char * outfile_name = (char*) malloc(sizeof(char) * 100); //generate output filename from input filename
		memcpy(outfile_name, filename, strlen(filename)-4); 
		memcpy(outfile_name+strlen(filename)-4, ".csv", 4);
		if (write_results(outfile_name, processes) == -1)
		{
			printf("Write file failed\n");
			return -1;
		}
		printf("Write file succeeded\n");
		return 0;

		

	}
	else if (argc != 2)
	{
		printf("Usage: paper_5 <file_name>");
		return -1;
		// generate time quantum that will be used 
	}
	return 0;
	
}

int improved_round_robin(Processes * processes, int quantum)
{
	irr_process * current = (irr_process* )malloc(sizeof(irr_process));
	irr_process * tmp = (irr_process* )malloc(sizeof(irr_process));
	int time_elapsed = 0, time_at_start_of_quantum = 0, time_quantum = 0, number_of_completed_processes = 0, context_switches = -1;

	irr_process * head = init_irr_processes(processes->head, processes->size);
	
	generate_time_quantum(head, quantum);

	// Continue working until all processes have completed
	while (number_of_completed_processes != processes->size)
	{
		current = head; //reset to head
		while (current)
		{
			bool idle = false;
			time_quantum = 0;
			idle = check_for_idling(head, &time_elapsed); // check if CPU is idling
			time_at_start_of_quantum = time_elapsed; // keep track of time 
			tmp = current;
			if (current->done == true || current->arrival_time > time_elapsed) // if process has either completed or has not arrived yet, skip
			{
				current = current->next;
				continue;
			}

			current = check_for_short_burst_time_processes(head, time_elapsed);
			if (current == NULL)
			{
				current = tmp;
			}

			if (current->burst_time <= current->time_quantum) // process has a bursttime that is shorter than the allocated time quantum
				time_quantum = current->burst_time; // time to run is just its remaining burst time

			else if (current->burst_time > current->time_quantum && 
					current->burst_time <= (current->time_quantum * 1.2) &&
					(current->priority == LOW_PRIORITY || current->priority == MED_PRIORITY)) // check if the process is within 120% of its allocated time quantum
					time_quantum = current->burst_time; // run till completion if it is
					 
			else if (current->burst_time > current->time_quantum && 
					current->burst_time <= current->time_quantum * 1.3 &&
					current->priority == HIGH_PRIORITY) // check if the high priority process is within 130% of its allocated time quantum
				time_quantum = current->burst_time; // run till completion if it is
			else 
				time_quantum = current->time_quantum; // run normally

			if (idle == false) 
				context_switches++; // since this is a preemptive algo, we increment the context switches when the cpu is not idle
			
			
			time_elapsed += time_quantum;
			current->burst_time -= time_quantum;

			if (current->first != true && current->burst_time < current->cpu_time)
			{
				current->response_time = time_at_start_of_quantum - current->arrival_time;
				current->first = true;
			}
			

			if (current->burst_time <= 0) // check if process has completed execution
			{
				set_completed_process_properties(processes->head, current, time_elapsed);
				number_of_completed_processes++;
				if (number_of_completed_processes == processes->size) // check here so that the algorithm does not overrun
					break;
				current = current->next;
				continue;
			}
			current = current->next; // check next process in queue
		}
	}
	processes->context_switches = context_switches;
	return 0;
}