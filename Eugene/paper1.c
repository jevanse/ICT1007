#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*Process struct, defining properties of a process */
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


/* Helper functions to assist with the algorithm */


/* Function to remove process from queue */
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

/* Function that moves the processes in the queue up */
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

/* Function to add process to the back of queue */
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

/* Function that arranges the queue in ascending order based on burst time*/
void ascending(int array[], int number)
{
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

/*Main program */

int main(void)
{
	/* Variable delclaration */

	int i, j, n, et_buffer, loop, tat[10], TQ, ct[10], max_process_time, response_times[10], max_arrival_time, size_t, queue_buffer, total_burst_time;
	int next_exe_time_start = 0, time_added;
	float total_waiting_time = 0, total_turnaround_time = 0;
	float awt = 0.0, att = 0.0, art = 0.0;


 /* Prompt user input for number of processes */

	printf("Enter the no of processes:");
	scanf("%d", &n);

/* Create the number of process structs accordingly */
	struct process process[n];

	/*Getting the process inputs (burst time, arrival time, time quantum) using a loop */
	for (i = 0; i < n; i++)
	{
		printf("Enter burst time for process %d:", i + 1);
		scanf("%d", &process[i].burst_time);

		printf("Enter Arrival time for process %d:", i + 1);
		scanf("%d", &process[i].arrival_time);

		process[i].process_number = i + 1;
	}

  /* User input for Time quantum */
	printf("\nEnter the size of time quantum:");
	scanf("%d", &TQ);

	/*Getting the biggest process time to assist with the loop*/
	max_process_time = 0;
	for (i = 0; i < n; i++)
	{
		if (max_process_time < process[i].burst_time)
		{
			max_process_time = process[i].burst_time;
		}
	}

  /*Initialising the first run counter to 0, used to calculate response time
  it will record the time when the process is first being ran. */
	for (i = 0; i < n; i++)
	{
		process[i].burst_time_dup = process[i].burst_time;
		process[i].first_run_counter = 0;
	}

	/*Getting the latest arrival time*/
	max_arrival_time = 0;

	for (i = 0; i < n; i++)
	{
		if (max_arrival_time < process[i].arrival_time)
		{
			max_arrival_time = process[i].arrival_time;
		}
	}

  /*Initally all process waiting times are zero*/
	for (i = 0; i < n; i++)
	{
		process[i].waiting_time = 0;
	}

	/*Printing out the user inputs*/
	for (i = 0; i < n; i++)
	{
		printf("Process %d \t burst time: %d \t arrival time: %d \t waiting time: %d\n", i + 1, process[i].burst_time, process[i].arrival_time, process[i].waiting_time);

	}

 /* Initialising elements in the ready queue to 0 instead of 'garabge values' */
 	int ready_q[n];
	for (i = 0; i < n; i++)
	{
		ready_q[i] = 0;
	}

	/* Calculate total burst time */
	for (i = 0; i < n; i++)
	{
		total_burst_time += process[i].burst_time;
	}

	/*Arranging arrival times in order */
	int arr_times[n];
	for (i = 0; i < n; i++)
	{
		arr_times[i] = process[i].arrival_time;
	}

	ascending(arr_times, n);

  /*Determine the next timestamp to check for any new processes coming in before TQ is up*/
	next_exe_time_start = arr_times[0] + TQ;

	/*Main Loop */

  /* Loop Variables*/
  
	int timer = 0;        /* Timer for this loop */
	int new_counter = 0;  /* Variable counter to check if process is added*/
	int cxt_switches = 0; /* Counter for context switches */
	int init_proc = 0;    /* Initial process to check for change */

	while (timer <= total_burst_time + arr_times[n - 1])
	{
		printf("\n\nCurrent time %d\n", timer);


    /* If process exist in the ready queue */
		if (ready_q[0] != 0)
		{
      /* Allocate CPU (reducing the remaning burst time */
			process[ready_q[0] - 1].burst_time = process[ready_q[0] - 1].burst_time - 1;
		}

    /* Buffer to capture the process at the front of the ready queue */
		int ar_buffer = ready_q[0];
    
    /* Loop to check for any new processes*/
		for (i = 0; i < n; i++)
		{ 

      /* Condition if process have same arrival time as the previous process and if timer > 0 and nothing in ready queue */
			if (timer == process[i].arrival_time && process[i].arrival_time == process[i - 1].arrival_time && i > 0 &&
				ar_buffer == 0)
			{
				/* Counter to check if there is a new process arrived before TQ is up or any process finished execution*/
				new_counter = 1;

        /* Add newly arrived process to the rear */
				add_to_rear(ready_q, n, process[i].process_number); 
			

				/* Sort ascending based on burst time if processes have the same arrival time */
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
      /* Condition to capture any process if timer matches with arrival time*/
			else if (timer == process[i].arrival_time)
			{

					/* Counter to check if there is a new process arrived before TQ is up or any process finished execution*/
          
				new_counter = 1;

        /* Add newly arrived process to the rear */
				add_to_rear(ready_q, n, process[i].process_number);

			}
		}

    /* Condition to check whether a process finished execution */
		if (process[ready_q[0] - 1].burst_time == 0 && ready_q[0] != 0)
		{
			printf("Process %d has finished execution\n", process[ready_q[0] - 1].process_number);

      /* Determine when its the next timestamp to rearrage if new processes added */
			next_exe_time_start = timer + TQ;


      /* If there is new process added before process finished execution, rearrange based on burst time left*/
			if (new_counter > 0)
			{
				/* Remove process from queue if burst time = 0 */
				remove_element(ready_q, process[ready_q[0] - 1].process_number, n);
				shift_left(ready_q, n);
				
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

				new_counter = 0;	/* Resetting the counter that checks for new processes */
			}


      /* If no new process added, just remove process that finished execution and move processes up 
      like a normal round robin */
			else
			{
				remove_element(ready_q, ready_q[0], n);
				shift_left(ready_q, n);
			}
      /* Printing out the processes in the ready queue */
			if (ready_q[0] != 0)
			{
				printf("\nReady Queue:\n");
				for (i = 0; i < n; i++)
				{
					if (ready_q[i] != 0)
					{
						printf("\t%d", ready_q[i]);
					}
				}
			}

      /* Increament the number of context switches if process finished execution and another process moves up*/
			if (ready_q[0] != 0)
			{
				cxt_switches++;
			}
		}

    /* Check if TQ is up */
		else if (timer == next_exe_time_start && process[ready_q[0]].process_number != 0)
		{
      /* Get the initial process at the start of the queue */
			init_proc = ready_q[0];

      /* If there is new process added before process finished execution, rearrange based on burst time left */
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
        /* Resetting the new process counter */
				new_counter = 0;
			}

      /* If not new process added, just move processes up like a normal round robin*/
			else
			{
				shift_left(ready_q, n);
			}

      /* Increment of context switches */
			if (init_proc != ready_q[0])
			{
				cxt_switches++;
			}

      /* Determine when its the next timestamp to rearrage if new processes added */
			if (process[ready_q[0] - 1].process_number != 0)
			{
				next_exe_time_start = next_exe_time_start + TQ;
			}

      /* Condition to catch if all processess have finished execution (if front of queue has no process) */
			else
			{
				next_exe_time_start = 0;
			}

			if (ready_q[0] != 0)
			{
				/* Formatting the output based on the requirement */
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

    /* As long as CPU is not allocated to the process (or process not at the front of queue), increment their waiting time */
		for (i = 0; i < n; i++)
		{
			if (process[i].process_number != ready_q[0] && process[i].burst_time != 0)
			{
				process[i].waiting_time += 1;
			}
		}

    /*Getting the response time for each process*/
    
    /* Increment the first run counter if the process ran before */
		if (ready_q[0] != 0)
		{
			process[ready_q[0] - 1].first_run_counter++;
		}

    /*If counter incremented, assign the timer to the process response time */
		if (process[ready_q[0] - 1].first_run_counter == 1)
		{
			process[ready_q[0] - 1].response_time = timer;
		}

		if (ready_q[0] != 0)
		{
			/* Formatting the output based on the requirement */
			printf("\nReady Queue:\n");
			for (i = 0; i < n; i++)
			{
				if (ready_q[i] != 0)
				{
					printf("\t%d", ready_q[i]);
				}
			}
		}
    
    /* Cycling the timer */
		timer++;
	}

  /* For loop to print the waiting times, reponse, turnaround times of each process*/
	for (i = 0; i < n; i++)
	{
		process[i].waiting_time -= process[i].arrival_time;
		process[i].turnaround_time = process[i].waiting_time + process[i].burst_time_dup;
		process[i].response_time = process[i].response_time - process[i].arrival_time;
		total_waiting_time += process[i].waiting_time;
		total_turnaround_time += process[i].turnaround_time;
    printf("Process %d \t waiting time: %d \t turnaround time: %d \t response time time: %d\n", i + 1, process[i].waiting_time, process[i].turnaround_time, process[i].response_time);
	}

/* Printing out Average waiting and turnaround time as well as the number of context switches*/
	awt = total_waiting_time / n;
	printf("\nAverage waiting time %f", total_waiting_time / n);
	printf("\nAverage turnaround time %f", total_turnaround_time / n);
	printf("\nNumber of context switches %d", cxt_switches);
}
