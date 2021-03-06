#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include <ctype.h>
#include <regex.h>
#include "../test_related/algorithm_testing.h"

void init_RNG(){
	// Initialize Psuedo-Random number generator
	time_t t;
	srand((unsigned) time(&t));
}

struct processes* addToReadyQueue(struct processes *process_queue, struct processes *old_ready_queue, int last_update_ts, int ts){
	
	// Initialize LinkedList to return
	Processes * out = (Processes*) malloc(sizeof(Processes));
	init_processes(out);

	// Initialize Buffer Variables
	struct process *p;
	struct process *delimiter;

	printf("\nTS = %d\n\n", ts);
	// Check Time Stamp
	if(ts == 0){
		// Set pointer to address at head of linked list
		p = process_queue->head;

		while(p != NULL){
			if(p->arrival_time == 0){
				printf("Inserting node with arrival time = 0\n");
				
				Process *new_process = (Process*) calloc(1, (sizeof(Process)));

				new_process-> pid = p-> pid;
				new_process-> burst_time = p-> burst_time;
				new_process-> arrival_time = p-> arrival_time;
				new_process-> priority = p-> priority;

				insert_node(out, new_process);
			}
			p = p-> next;
		}
	}else{
		// Step 1:	Add existing ready queue to output

		if(old_ready_queue->head != NULL){
			p = old_ready_queue->head;

			// Iterate old ready queue and add to new output
			while(p-> next != NULL){
				
				p = p-> next;
			}
			insert_node(out, p);
		}

		// Step 2:	Add processes that have arrived to queue
		p = process_queue-> head;
		delimiter = process_queue-> tail;

		// Iterate the process queue
		while (p != delimiter){

			// If the arrival time is less than or equal to current Time Stamp
			// and more than the Time Stamp when ready queue was last updated
			// add to out

			if(p->arrival_time <= ts	&&	  p->arrival_time > last_update_ts){
				printf("Inserting Node with Arrival time of %d\tBT:%d\n", p->arrival_time, p-> burst_time);
				
				Process *new_process = (Process*) calloc(1, (sizeof(Process)));
				
				new_process-> pid = p-> pid;
				new_process-> burst_time = p-> burst_time;
				new_process-> arrival_time = p-> arrival_time;
				new_process-> priority = p-> priority;
				insert_node(out, new_process);
			}

			// Traverse the process queue linked list
			p = p-> next;
		}
		
		if(p->arrival_time <= ts	&&	  p->arrival_time > last_update_ts){
				printf("Inserting Node with Arrival time of %d\tBT:%d\n", p->arrival_time, p-> burst_time);
				
				Process *new_process = (Process*) calloc(1, (sizeof(Process)));

				new_process-> pid = p-> pid;
				new_process-> burst_time = p-> burst_time;
				new_process-> arrival_time = p-> arrival_time;
				new_process-> priority = p-> priority;
				insert_node(out, new_process);
			}
	}

	// Return the output linked list
	// p = out-> head;
	// printf("Adder Ready queue output\n****************\n");
	// while(p != NULL){
	// 	printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
	//  	p = p-> next;
	// }
	// printf("\n\n");
	return out;
}

int findSmallestArrivalTime(struct processes *process_queue, int ts){
	int next_arrival_time = -1;
	struct process *p = process_queue-> head;

	// Iterate through whole process queue
	while (p != NULL){
		// Find the closest arrival time after the time stamp
		if(p->arrival_time > ts){								// If this process has not arrived yet
			if(next_arrival_time == -1){						// If there isnt already a potential result,
				next_arrival_time = p-> arrival_time;			// It is automatically the best result
			}else if(p-> arrival_time < next_arrival_time){		// Else, if the process arrival time is earlier than the potential result, it becomes the potential result
				next_arrival_time = p-> arrival_time;
			}
			printf("Found something");
		}
		p = p-> next;											// Iterate through all to find best result
	}

	return next_arrival_time;									// Return that result
}

struct processes* sortBurstTimes(struct processes *p, int n){
	int i = 0;
	struct process *i_node = p->head;
	struct process *min_node;

	// Allocate memory for sorted queue and initialize
    Processes * sorted_process_queue = (Processes*) malloc(sizeof(Processes));
	init_processes(sorted_process_queue);

	//printf("\nSorter Variables initialized\n");	//DEBUG

	// Insert smallest element into sorted process queue
	for (i = 0; i < n; i++){
		//printf("\nInserting Node %d\n", i+1);

		// Iterate through all elements
		for (int j = 0; j < n; j++){

			if(j == 0){							//If first element to be checked, auto smallest
				min_node = i_node;
			}else if( min_node-> burst_time < 1){
				min_node = i_node;
			}
			else if(
				(i_node-> burst_time < min_node-> burst_time)	//Current node bt less than minimum node bt
						&& 										//and
				(i_node-> burst_time > 0)						//Current node bt is valid
			){
				min_node = i_node;
			}
			if(i_node->next != NULL){							//If not last element
				i_node = i_node-> next;							//Travese to next node
			}
		}
		//printf("Insert node with bt %d\n", min_node->burst_time);

		// Allocate memory space for new process
		Process *new_process = (Process*) calloc(1, (sizeof(Process)));

			// Set Proccess properties
			new_process-> pid = min_node-> pid;
			new_process-> burst_time = min_node-> burst_time;
			new_process-> cpu_time = min_node-> cpu_time;
			new_process-> arrival_time = min_node-> arrival_time;

		insert_node(sorted_process_queue, new_process);
		min_node-> burst_time = 0;
		i_node = p-> head;
	}

	//printf("sorted_process_queue head bt is: %d\n", sorted_process_queue->head->burst_time);
	//printf("Exit sorter\n");
	return sorted_process_queue;
}

struct process* swapProcessNodes(struct processes *queue, struct process *origin_node, int n){
	/*
	* Function: swapProcessNodes
	* ------------------
	* Swaps 2 adjacent Nodes in a linked list
	*
	* queue	: 	linked list of processes
	* origin_node	:	Process that needs to be swapped with its neighbour
	* n				:	number of elements
	*
	* returns		:	new node in original position of origin_node
	*/
	struct process* previous_node = queue-> head;
	struct process* current_node = previous_node-> next;
	//printf("\nOrigin Node bt: %d\n\n", origin_node->burst_time);
	char temp[20];
	// printf("Iterating...\n");
	for(int i = 0; i < n; i++){
		//printf("Current iterated node bt: %d\n", current_node-> burst_time);

		// If current node is node that needs to be swapped
		if (current_node == origin_node){
			//printf("Node found\n");
			struct process* next_node = current_node->next;
			struct process* nextnext_node = next_node-> next;

			previous_node-> next = next_node;
			next_node-> next = origin_node;
			current_node-> next = nextnext_node;

			//printf("Returning new current node\nBT: %d\tNext BT:%d", next_node->burst_time, next_node->next->burst_time);
			return next_node;
		}
		
		// Traverse linked list
		previous_node = current_node;
		current_node = current_node-> next;
	}

	printf("Swap failed\n");
	scanf("%s", temp);
	return origin_node;
}

struct processes SJF(struct processes *process_queue, int n){
	printf("\n\nStarting SJF Algorithm\n\n");
	int ts = 0, last_update_ts = 0, dump;

	// Initialise Ready Queue Linked List
	Processes * SJF_ReadyQueue = (Processes*) malloc(sizeof(Processes));
	init_processes(SJF_ReadyQueue);
	printf("Ready Queue initialised\n");

	// Initialise Output Linked List
	Processes * SJF_Results = (Processes*) malloc(sizeof(Processes));
	init_processes(SJF_Results);
	printf("Result buffer initialised\n");

	// Initialise Buffer Variables
	struct process *p;

	// Find all processes with Arrival time = 0
	// Add to Ready Queue Linked List
	SJF_ReadyQueue = addToReadyQueue(process_queue, SJF_ReadyQueue, 0, 0);
	SJF_ReadyQueue = sortBurstTimes(SJF_ReadyQueue, SJF_ReadyQueue-> size);
	printf("Initial arrivals added to ready queue\n");

	p = process_queue-> head;
	printf("Main process queue\n**********\n");
	while(p != NULL){
		printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
	 	p = p-> next;
	}
	printf("\n\n");

	// Run till all processes have been calculated
	while(SJF_Results->size < n){
		printf("\n\nCurrent Result output has %d elements\n\n", SJF_Results-> size);
		// If there is something in the ready queue
		if(SJF_ReadyQueue-> head != NULL){
			printf("Accessing first element of ready queue\n\n");
			// Start processing whatever is at the head of the ready queue
			p = SJF_ReadyQueue-> head;
			p-> waiting_time = ts - p-> arrival_time;
			p-> turnaround_time = ts + p->burst_time;

			// Insert that process into the result linked list
			insert_node(SJF_Results, p);

			// Increment the Time Stamp
			ts = ts + p->burst_time;

			// Remove the first element in the ready queue
			SJF_ReadyQueue-> head = p-> next;
			SJF_ReadyQueue-> size--;

		}else{
			printf("Nothing in the Ready Queue\n");
			// Find the next arrival time and time travel to that point in time
			ts = findSmallestArrivalTime(process_queue, ts);
			printf("Time travel to T = %d\n\n", ts);
		}

		if(SJF_ReadyQueue-> size + SJF_Results-> size < n){
			printf("Checking for new arrivals\n");
			// Check for new arrivals
			SJF_ReadyQueue = addToReadyQueue(process_queue, SJF_ReadyQueue, last_update_ts, ts);
			SJF_ReadyQueue = sortBurstTimes(SJF_ReadyQueue, SJF_ReadyQueue-> size);
			last_update_ts = ts;
		}

		p = SJF_ReadyQueue-> head;
		while(p != NULL){
			printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
	 		p = p-> next;
		}
	}

	return *SJF_Results;
}

int main(){
	int n;
	char has_test_case;
	char filename[200];
	char *file_contents;
	struct process *p;
	int temp;

	Processes *process_queue = (Processes*) malloc(sizeof(Processes));
	init_processes(process_queue);

	// Check if user has test has_test_case
	printf("Do you have a test case? [y/n]:\t");
	scanf("%c", &has_test_case);
	has_test_case = tolower(has_test_case);

	if(has_test_case == 'y'){
		printf("Enter relative file path: ");
		scanf("%s", filename);

		read_file(filename, &file_contents);

		get_processes(filename, &process_queue);

		p = process_queue->head;

		while(p != NULL){
			printf("PID:%d, BT:%d, AT:%d, Prio:%d\n", p->pid, p->burst_time, p->arrival_time, p->priority);
			p = p->next;
		}

		scanf("%d", &temp);
	}else if(has_test_case == 'n'){
		printf("How many processes to simulate:\t");
		scanf("%d", &n);

		init_RNG();
		for (int i = 0; i < n; i++){
			Process *new_process = (Process*) calloc(1, (sizeof(Process)));

			new_process-> pid = i;
			new_process-> burst_time = (rand()%100) + 1;
			new_process-> arrival_time = (rand()%50);

			insert_node(process_queue, new_process);
		}
		printf("Test case initialised\n");

	}else{
		printf("Input not recognised.\nExiting application.");
		exit(EXIT_FAILURE);
	}

	// SJF Loop
	Processes * SJF_IN = (Processes*)malloc(sizeof(Processes));
	init_processes(SJF_IN);
	memcpy(SJF_IN, process_queue, sizeof(Processes));

	struct processes SJF_Results = SJF(SJF_IN, n);

	p = SJF_Results.head;
	for(int i = 0; i < SJF_Results.size; i++){
		printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
	 	p=p->next;
	}
	// exit(EXIT_SUCCESS);

}