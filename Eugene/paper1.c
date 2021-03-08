#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*Process struct */

typedef struct process
{
	int process_number;
	int burst_time;
	int arrival_time;
	int waiting_time;
	int turnaround_time;
	int burst_time_dup;
	int response_time;
	int first_run_counter;

}

Process;

/*Helper functions to assist with the algorithm */

void remove_element(int array[], int element, int array_length)
{
	for (int i = 0; i < array_length; i++)
	{
		if (array[i] == element)
		{
			array[i] = 0;
		}
	}
}

void shift_left(int array[], int n)
{
	int temp, lol;
	temp = array[0];

	for (int i = 0; i < n; i++)
	{
		array[i] = array[i + 1];

	}

	for (int i = 0; i < n; i++)
	{
		if (array[i] == 0)
		{
			array[i] = temp;
			break;
		}
	}

	array[n - 1] = 0;

}

void add_to_rear(int array[], int n, int element)
{
	for (int i = 0; i < n; i++)
	{
		if (array[i] == 0)
		{
			array[i] = element;
			break;
		}
	}
}

void ascending(int array[], int number)
{
	// Arranges Ready Queue according to burst time in ascending order
	int array_buffer;
	for (int i = 0; i < number; ++i)
	{
		for (int j = i + 1; j < number; ++j)
		{
			if (array[i] > array[j])
			{
				array_buffer = array[i];
				array[i] = array[j];
				array[j] = array_buffer;
			}
		}
	}
}

/*Main prorgram */

int main(void)
{
	// Variable delclaration

	int i, j, n, et_buffer, loop, tat[10], TQ, ct[10], max_process_time, response_times[10], max_arrival_time, size_t, queue_buffer, total_burst_time;
	int next_exe_time_start = 0, total_arr_time, time_added;
	float total_waiting_time = 0, total_turnaround_time = 0;
	float awt = 0.0, att = 0.0, art = 0.0;

	printf("Enter the no of processes:");
	scanf("%d", &n);

	struct process process[n];	//Create the number of processes based on input.

	/*Getting the process inputs (burst time, arrival time, time quantum) */

	for (i = 0; i < n; i++)
	{
		printf("Enter burst time for process %d:", i + 1);
		scanf("%d", &process[i].burst_time);

		printf("Enter Arrival time for process %d:", i + 1);
		scanf("%d", &process[i].arrival_time);

		process[i].process_number = i + 1;
	}

	printf("\nEnter the size of time quantum:");
	scanf("%d", &TQ);

	//Finding biggest process time
	max_process_time = 0;
	for (i = 0; i < n; i++)
	{
		if (max_process_time < process[i].burst_time)
		{
			max_process_time = process[i].burst_time;
		}
	}

	for (i = 0; i < n; i++)
	{
		process[i].burst_time_dup = process[i].burst_time;
		process[i].first_run_counter = 0;
	}

	//Find latest arrival
	max_arrival_time = 0;

	for (i = 0; i < n; i++)
	{
		if (max_arrival_time < process[i].arrival_time)
		{
			max_arrival_time = process[i].arrival_time;
		}
	}

	//Initialize all values of the waiting time to zero intially.
	for (i = 0; i < n; i++)
	{
		process[i].waiting_time = 0;
	}

	//Printing out inputs.
	for (i = 0; i < n; i++)
	{
		printf("Process %d \t burst time: %d \t arrival time: %d \t waiting time: %d\n", i + 1, process[i].burst_time, process[i].arrival_time, process[i].waiting_time);

	}

	int ready_q[n];
	//Control variable to check if array is empty
	for (i = 0; i < n; i++)
	{
		ready_q[i] = 0;
	}

	//Calculate total burst time
	for (i = 0; i < n; i++)
	{
		total_burst_time += process[i].burst_time;
		//printf("Total process time needed: %d", total_burst_time);
	}

	for (i = 0; i < n; i++)
	{
		total_arr_time += process[i].arrival_time;

	}

	//Need to arrage base on arrival times
	int arr_times[n];
	for (i = 0; i < n; i++)
	{
		arr_times[i] = process[i].arrival_time;
	}

	ascending(arr_times, n);
	next_exe_time_start = arr_times[0] + TQ;
	//printf("\n1st next start time %d", next_exe_time_start);

	/*Main Loop */

	int timer = 0;
	int new_counter = 0;
	int cxt_switches = 0;
	int init_proc = 0;

	while (timer <= total_burst_time + arr_times[n - 1])
	{
		printf("\n\nCurrent time %d\n", timer);

		if (ready_q[0] != 0)
		{
			//Check to see if process q is empty
			process[ready_q[0] - 1].burst_time = process[ready_q[0] - 1].burst_time - 1;
		}

		int ar_buffer = ready_q[0];

		for (i = 0; i < n; i++)
		{
			if (timer == process[i].arrival_time && process[i].arrival_time == process[i - 1].arrival_time && i > 0 &&
				ar_buffer == 0)
			{
				//If >1 processes comes in at t=0 check to see which one has smaller exe time. 
				new_counter = 1;
				add_to_rear(ready_q, n, process[i].process_number);
				/*printf("First process arrival time %d\n", process[i].arrival_time);
				printf("Comparing process arrival time %d \n",process[i-1].arrival_time);
				printf("First process should not be here"); */

				//Sort ascending

				for (int i = 0; i < n; ++i)
				{
					for (int j = i + 1; j < n; ++j)
					{
						if (process[ready_q[i] - 1].burst_time > process[ready_q[j] - 1].burst_time && ready_q[i] &&
							ready_q[i] != 0 && ready_q[j] != 0)
						{
							queue_buffer = ready_q[i];
							ready_q[i] = ready_q[j];
							ready_q[j] = queue_buffer;
						}
					}
				}
			}
			else if (timer == process[i].arrival_time)
			{
				//Since CPU is already allocated after one second, add to rear.
				new_counter = 1;
				//printf("Should be added to the rear");
				add_to_rear(ready_q, n, process[i].process_number);

			}
		}

		if (process[ready_q[0] - 1].burst_time == 0 && ready_q[0] != 0)
		{
			printf("Process %d has finished execution\n", process[ready_q[0] - 1].process_number);
			next_exe_time_start = timer + TQ;

			if (new_counter > 0)
			{
				//If process time is up, and new process added, rearrange
				remove_element(ready_q, process[ready_q[0] - 1].process_number, n);
				shift_left(ready_q, n);
				//Rearange and print out before removing
				for (int i = 0; i < n; ++i)
				{
					for (int j = i + 1; j < n; ++j)
					{
						if (process[ready_q[i] - 1].burst_time > process[ready_q[j] - 1].burst_time && ready_q[i] &&
							ready_q[i] != 0 && ready_q[j] != 0)
						{
							queue_buffer = ready_q[i];
							ready_q[i] = ready_q[j];
							ready_q[j] = queue_buffer;
						}
					}
				}

				new_counter = 0;	//Resetting the time stamp where it last got added
			}
			else
			{
				remove_element(ready_q, ready_q[0], n);
				shift_left(ready_q, n);
			}

			if (ready_q[0] != 0)
			{
				//Only print out if ready queue is not empty
				printf("\nReady Queue:\n");
				for (i = 0; i < n; i++)
				{
					if (ready_q[i] != 0)
					{
						printf("\t%d", ready_q[i]);
					}
				}
			}

			if (ready_q[0] != 0)
			{
				cxt_switches++;
			}
		}
		else if (timer == next_exe_time_start && process[ready_q[0]].process_number != 0)
		{
			init_proc = ready_q[0];
			if (new_counter > 0)
			{
				printf("TQ is up, rearrange");
				for (int i = 0; i < n; ++i)
				{
					for (int j = i + 1; j < n; ++j)
					{
						if (process[ready_q[i] - 1].burst_time > process[ready_q[j] - 1].burst_time && ready_q[i] &&
							ready_q[i] != 0 && ready_q[j] != 0)
						{
							queue_buffer = ready_q[i];
							ready_q[i] = ready_q[j];
							ready_q[j] = queue_buffer;
						}
					}
				}

				new_counter = 0;	//Reseting the counter
			}
			else
			{
				shift_left(ready_q, n);
			}

			if (init_proc != ready_q[0])
			{
				cxt_switches++;
			}

			// printf("Next start time %d", next_exe_time_start);
			// printf("Process has not finished execution but tq is up");

			if (process[ready_q[0] - 1].process_number != 0)
			{
				next_exe_time_start = next_exe_time_start + TQ;
			}
			else
			{
				next_exe_time_start = 0;
			}

			//printf("\nBurst time left for process : %d", process[ready_q[0]-1].burst_time);

			if (ready_q[0] != 0)
			{
				//Only print out if ready queue is not empty
				printf("\nReady Queue:\n");
				for (i = 0; i < n; i++)
				{
					if (ready_q[i] != 0)
					{
						printf("\t%d", ready_q[i]);
					}
				}
			}
		}

		for (i = 0; i < n; i++)
		{
			if (process[i].process_number != ready_q[0] && process[i].burst_time != 0)
			{
				process[i].waiting_time += 1;
			}
		}

		if (ready_q[0] != 0)
		{
			process[ready_q[0] - 1].first_run_counter++;
		}

		if (process[ready_q[0] - 1].first_run_counter == 1)
		{
			process[ready_q[0] - 1].response_time = timer;
		}

		if (ready_q[0] != 0)
		{
			//Only print out if ready queue is not empty
			printf("\nReady Queue:\n");
			for (i = 0; i < n; i++)
			{
				if (ready_q[i] != 0)
				{
					printf("\t%d", ready_q[i]);
				}
			}
		}

		timer++;
	}

	for (i = 0; i < n; i++)
	{
		process[i].waiting_time -= process[i].arrival_time;
		process[i].turnaround_time = process[i].waiting_time + process[i].burst_time_dup;
		process[i].response_time = process[i].response_time - process[i].arrival_time;
		total_waiting_time += process[i].waiting_time;
		total_turnaround_time += process[i].turnaround_time;
    printf("Process %d \t waiting time: %d \t turnaround time: %d \t response time time: %d\n", i + 1, process[i].waiting_time, process[i].turnaround_time, process[i].response_time);
	}





	awt = total_waiting_time / n;
	printf("\nAverage waiting time %f", total_waiting_time / n);
	printf("\nAverage turnaround time %f", total_turnaround_time / n);
	printf("\nNumber of context switches %d", cxt_switches);
}
