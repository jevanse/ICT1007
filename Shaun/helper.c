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
    //create a copy of the linked list 
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


void check_queue_for_low_burst_time_proc(irr_process * process_queue, int * front, int * back, int queue_size,int time_quantum)
{
    int threshold = time_quantum * 0.2;
    irr_process * temp = (irr_process *) calloc(1, sizeof(irr_process));
    int i;
    //iterate through queue, if any ones are low bt move them to front
    if (*front == -1) // queue is empty
        return;

    printf("Before adding low burst time to front\n\t");

    print_processes_in_queue(process_queue, *front, *back);

    for (i = *front; i <= *back; i++)
    {
        printf("Comparison bursttimes: %d, threshhold -> %d\n", process_queue[i].burst_time, threshold);
        if (process_queue[i].burst_time <= threshold)
        {
            memcpy(temp, &process_queue[i], sizeof(irr_process));
            break;
        }
    }
    if (temp->pid != 0)
    {
        printf("Temp pid->%d\n", temp->pid);
        // add to front
        // i should be the index where the process to be moved is
        // move to the right by one
        memcpy(&process_queue[*front + 1], &process_queue[*front], sizeof(irr_process)  *  i);
        // now process_queue[0] should be empty
        memcpy(&process_queue[*front], temp, sizeof(irr_process));

    }
    printf("After adding low burst time to front\n\t");

    print_processes_in_queue(process_queue, *front, *back);
    return;
}

void add_arriving_process(irr_process * head, irr_process * process_queue,int * time_elapsed, int time_at_start_of_quantum, int * front, int * back, int queue_size)
{
    irr_process * current = head;

    while (current)
    {
        printf("Front value: %d\n", *front);
        printf("Current -> arrivaltime = %d\n", current->arrival_time );
        printf("Time elapsed: %d\n", *time_elapsed);
        if (current->arrival_time <= *time_elapsed && current->arrival_time >= time_at_start_of_quantum) 
        {
            enqueue(current, process_queue, front, back, queue_size);
            return;
        }
        //check if the queue is empty, if it is skip to the first process, while updating time_elapsed
        else if (*front == -1 && *time_elapsed < current->arrival_time) // will only occur once 
	    {
		    printf("Add arriving process: Queue is empty\n");
            //proceed to skip 
            *time_elapsed = current->arrival_time;
            printf("TIme elapsed inside add_arriving_process: %d", *time_elapsed);
            enqueue(current, process_queue, front, back, queue_size);
            return; //return as you should re-check the time_elapsed after executing the current process

        }//since processes arrive in order, returning the first arrival time that is bigger than the current time is fine
        current = current->next;
    }
    free(current);
    print_processes_in_queue(process_queue, *front, *back);

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


void enqueue(irr_process * process, irr_process * process_queue, int * front, int * rear, int queue_size)
{
	if (*rear == queue_size - 1) printf("Queue is full\n");
	else 
	{
		if (*front == -1) *front = 0;
		*rear = *rear + 1;
		process_queue[*rear] = *process;
	}
}

irr_process * dequeue(irr_process * process_queue, int * front, int * rear)
{
	irr_process * tmp = (irr_process *) malloc(sizeof(irr_process));
	if (*front == -1) 
	{
		printf("Dequeue: Queue is empty\n");
        free(tmp);
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


int init(Processes * processes) //function for testing
{
	init_processes(processes);
    
    Process *process_1 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_2 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_3 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_4 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_5 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_6 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_7 = (Process*) calloc(1,(sizeof(Process)));

     /* test case 1 */
    // process_1->arrival_time = 0;
	// process_1->pid = 1;
	// process_1->burst_time = 550;
	// process_1->cpu_time = process_1->burst_time;
	// process_1->priority = 3;
       
	// process_2->arrival_time = 200;
	// process_2->pid = 2;
	// process_2->burst_time = 800;
	// process_2->cpu_time = process_2->burst_time;
	// process_2->priority = 1;

	// process_3->arrival_time = 100;
	// process_3->pid = 3;
	// process_3->burst_time = 200;
	// process_3->priority = 3;
	// process_3->cpu_time = process_3->burst_time;

	// process_4->arrival_time = 400;
	// process_4->pid = 4;
	// process_4->burst_time = 2600;
	// process_4->cpu_time = process_4->burst_time;
	// process_4->priority = 2;

    // process_5->arrival_time = 0;
	// process_5->pid = 5;
	// process_5->burst_time = 1600;
	// process_5->cpu_time = process_5->burst_time;
	// process_5->priority = 2;

    /* test case 2 */
    // process_1->arrival_time = 0;
	// process_1->pid = 1;
	// process_1->burst_time = 550;
	// process_1->cpu_time = process_1->burst_time;
	// process_1->priority = 3;
       
	// process_2->arrival_time = 0;
	// process_2->pid = 2;
	// process_2->burst_time = 1250;
	// process_2->cpu_time = process_2->burst_time;
	// process_2->priority = 1;

	// process_3->arrival_time = 0;
	// process_3->pid = 3;
	// process_3->burst_time = 1950;
	// process_3->priority = 3;
	// process_3->cpu_time = process_3->burst_time;

	// process_4->arrival_time = 0;
	// process_4->pid = 4;
	// process_4->burst_time = 50;
	// process_4->cpu_time = process_4->burst_time;
	// process_4->priority = 3;

    // process_5->arrival_time = 0;
	// process_5->pid = 5;
	// process_5->burst_time = 500;
	// process_5->cpu_time = process_5->burst_time;
	// process_5->priority = 2;

    // process_6->arrival_time = 0;
	// process_6->pid = 6;
	// process_6->burst_time = 1200;
	// process_6->cpu_time = process_6->burst_time;
	// process_6->priority = 1;

    // process_7->arrival_time = 0;
	// process_7->pid = 7;
	// process_7->burst_time = 100;
	// process_7->cpu_time = process_7->burst_time;
	// process_7->priority = 3;

    process_1->arrival_time = 200;
	process_1->pid = 1;
	process_1->burst_time = 600;
	process_1->cpu_time = process_1->burst_time;
	process_1->priority = 2;
       
	process_2->arrival_time = 1200;
	process_2->pid = 2;
	process_2->burst_time = 1000;
	process_2->cpu_time = process_2->burst_time;
	process_2->priority = 2;

	process_3->arrival_time = 1000;
	process_3->pid = 3;
	process_3->burst_time = 1300;
	process_3->priority = 2;
	process_3->cpu_time = process_3->burst_time;

    insert_node(processes, process_1);
    insert_node(processes, process_2);
    insert_node(processes, process_3);
    // insert_node(processes, process_4);
    // insert_node(processes, process_5);
    // insert_node(processes, process_6);
    // insert_node(processes, process_7);

	return 0;
}

void generate_time_quantum(irr_process * head, int time_quantum)
{
	irr_process * curr = head;
    while (curr)
    {
        if (curr->priority == LOW_PRIORITY)
            curr->time_quantum = time_quantum * 0.8;
        else if (curr->priority == MED_PRIORITY)
            curr->time_quantum = time_quantum;
        else if (curr->priority == HIGH_PRIORITY)
            curr->time_quantum = time_quantum * 1.2;
        curr = curr->next;
    }
}
void check_for_idling(irr_process * head, int *time_elapsed)
{
    irr_process * current = head;
    int time_skip = 2 ^ 32;
    while (current)
    {
        if (current->done)
        {
            current = current->next;
            continue;
        }
            
        if (current->arrival_time <= *time_elapsed)
            return;
        if (current->done == false && current->arrival_time < time_skip)
        {
            //set time_skip to smallest arrival time
            time_skip = current->arrival_time;
        }
        current = current->next;
    }

    *time_elapsed = time_skip;
    //printf("Time skipped to: %d\n", time_skip);
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

void print_processes_in_queue(irr_process * process_queue, int front, int rear)//debugging function
{
	printf("\nQueue: ");
	for (int i = front; i <= rear; i++)
	{
		printf("\tProcess %d ", process_queue[i].pid);
        printf("->burstTime = %d\n", process_queue[i].burst_time);
	}
	printf("\n\n");

}

void print_results(Processes* processes)
{
    Process * current = processes->head;
    printf("| PID | Burst time | Arrival Time | TurnAroundTime | Waiting Time | Response Time | Priority \n");
	//iterate through processes and print results into a table
    while (current)
    {
        printf("| %d\t| %d\t| %d\t| %d\t| %d\t| %d\t| %d|\n", current->pid, current->cpu_time, current->arrival_time, current->turnaround_time, current->waiting_time, current->response_time, current->priority);
        current = current->next;
    }
    printf("--------------------------------------------------------\n");
    printf("Number of context switches %d\n", processes->context_switches); // ToDo: Change this to real ctx 
}

int generate_dynamic_timequantum(Processes * processes)
{
    Process * current = processes->head;
    int * burst_times = (int *) calloc(processes->size, sizeof(int));
    for (int i = 0; i < processes->size; i++)
    {
        printf("Inside loop, ctr %d\n", i);
        burst_times[i] = current->burst_time;
        current = current->next;
        if (current == NULL)
            break;
    }
    // Sort burst times 
    printf("Before bubbleSort\n");
    bubbleSort(burst_times, processes->size);
    printf("After bubbleSort\n");
    // Find median 
    int median = (processes->size * 0.8) -1;
    printf("Median found: %d\n", median);
    return burst_times[median];
}
