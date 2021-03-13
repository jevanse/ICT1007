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
			if(p-> arrival_time == 0){
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

				Process *new_process = (Process*) calloc(1, (sizeof(Process)));

					new_process-> pid = p-> pid;
					new_process-> burst_time = p-> burst_time;
					new_process-> arrival_time = p-> arrival_time;
					new_process-> priority = p-> priority;

				insert_node(out, new_process);
					
				p = p->next;
			}

			Process *new_process = (Process*) calloc(1, (sizeof(Process)));

				new_process-> pid = p-> pid;
				new_process-> burst_time = p-> burst_time;
				new_process-> arrival_time = p-> arrival_time;
				new_process-> priority = p-> priority;

			insert_node(out, new_process);
			
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

	return out;
}

int findSmallestArrivalTime(struct processes *process_queue, int ts, int last_update_ts){
	int next_arrival_time = -1;
	struct process *p = process_queue-> head;

	// Iterate through whole process queue
	while (p != NULL){
		// Find the closest arrival time after the time stamp
		if(p->arrival_time > ts && p->arrival_time > last_update_ts){								// If this process has not arrived yet
			
			if(next_arrival_time == -1){						// If there isnt already a potential result,
				
				next_arrival_time = p-> arrival_time;			// It is automatically the best result
			
			}else if(p-> arrival_time < next_arrival_time){		// Else, if the process arrival time is earlier than the potential result, it becomes the potential result
				
				next_arrival_time = p-> arrival_time;
			
			}
			printf("Found something\n");
		}
		p = p-> next;											// Iterate through all to find best result
	}

	if(next_arrival_time > ts){

		return next_arrival_time;								// Return that result
	
	}
	else{
		printf("FATAL ERROR");
		exit(EXIT_FAILURE);
	}								
}

struct processes* sortBurstTimes(struct processes *p, int n){

	// Allocate memory for duplicated queue and initialize
	// Duplicate the queue so i can screw with it at will
    Processes * dup_ready_queue = (Processes*) malloc(sizeof(Processes));
	init_processes(dup_ready_queue);
	memcpy(dup_ready_queue, p, sizeof(Processes));

	int i = 0;
	struct process *i_node = dup_ready_queue->head;
	struct process *min_node;	

	// Allocate memory for sorted queue and initialize
    Processes * sorted_process_queue = (Processes*) malloc(sizeof(Processes));
	init_processes(sorted_process_queue);

	// Insert smallest element into sorted process queue
	for (i = 0; i < n; i++){
		

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
		printf("\nSorter inserting Node with BT:%d, AT:%d\n", new_process-> burst_time, new_process-> arrival_time);
		min_node-> burst_time = 0;
		i_node = p-> head;
	}

	//printf("sorted_process_queue head bt is: %d\n", sorted_process_queue->head->burst_time);
	//printf("Exit sorter\n");

	// Free the duplicated ready queue
	// Actually not necessary, but, best practices
	free(dup_ready_queue);

	return sorted_process_queue;
}

struct process* swapProcessNodes(struct processes *readyqueue, struct process *origin_node, int n){
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
	struct process* current_node = readyqueue-> head;

	struct process* next_node = current_node->next;
	struct process* nextnext_node = next_node-> next;

	current_node-> next = nextnext_node;
	next_node-> next = current_node;

	//printf("Returning new current node\nBT: %d\tNext BT:%d", next_node->burst_time, next_node->next->burst_time);
	return next_node;
}

struct processes SJF(struct processes *process_queue, int n){
	printf("\n\nStarting SJF Algorithm\n\n");
	int ts = 0, last_update_ts = 0, lp_n = 0, dump;

	// Initialise Ready Queue Linked List
	Processes * SJF_ReadyQueue = (Processes*) malloc(sizeof(Processes));
	init_processes(SJF_ReadyQueue);
	printf("Ready Queue initialised\n");

	// Initialise Output Linked List
	Processes * SJF_Results = (Processes*) malloc(sizeof(Processes));
	init_processes(SJF_Results);
	printf("Result buffer initialised\n");

	// Initialise Buffer Variables
	struct process *current_node;
	struct process *p;

	// Find all processes with Arrival time = 0
	// Add to Ready Queue Linked List
	SJF_ReadyQueue = addToReadyQueue(process_queue, SJF_ReadyQueue, 0, 0);
	SJF_ReadyQueue = sortBurstTimes(SJF_ReadyQueue, SJF_ReadyQueue-> size);
	printf("Initial arrivals added to ready queue\n");

	// Run till all processes have been calculated
	while(SJF_Results->size < process_queue-> size){
		printf("\n\nCurrent Result output has %d elements\nCurrent Ready Queue has %d elements\n\n", SJF_Results-> size, SJF_ReadyQueue-> size);
		
		// If there is something in the ready queue
		if(SJF_ReadyQueue-> head != NULL){
			printf("Accessing first element of ready queue\n\n");
			// Start processing whatever is at the head of the ready queue
			p = SJF_ReadyQueue-> head;

			// Allocate memory space for new process
			Process *new_process = (Process*) calloc(1, (sizeof(Process)));

				// Set Proccess properties
				new_process-> pid = p-> pid;
				new_process-> burst_time = p-> burst_time;
				new_process-> arrival_time = p-> arrival_time;
				new_process-> waiting_time = ts - p-> arrival_time;
				new_process-> turnaround_time = ts + p-> burst_time;
				new_process-> response_time = new_process-> waiting_time;
			
			// Insert that process into the result linked list
			insert_node(SJF_Results, new_process);

			// Increment the Time Stamp
			ts = ts + new_process->burst_time;

			// Remove the first element in the ready queue
			SJF_ReadyQueue-> head = p-> next;
			SJF_ReadyQueue-> size--;

			printf("ready q decrement, Ready Queue size: %d\n", SJF_ReadyQueue-> size);

		}else if(
			(SJF_ReadyQueue-> size + SJF_Results-> size) 
			!= 
			(process_queue-> size)
			){
			printf("Nothing in the Ready Queue\n");
			// Find the next arrival time and time travel to that point in time
			ts = findSmallestArrivalTime(process_queue, ts, last_update_ts);
			printf("Time travel to T = %d\n\n", ts);
		}

		//printf("Ready Queue Size = %d,\tResult Queue Size = %d\n, Main Queue Size = %d", SJF_ReadyQueue-> size, SJF_Results-> size, process_queue-> size);

		// If not every element has been put in ready queue or result queue, check for more
		if((SJF_ReadyQueue-> size + SJF_Results-> size) != (process_queue-> size)) {
			printf("Checking for new arrivals\n");
			// Check for new arrivals
			SJF_ReadyQueue = addToReadyQueue(process_queue, SJF_ReadyQueue, last_update_ts, ts);
			printf("after add to rdy q, Ready Queue size: %d\n", SJF_ReadyQueue-> size);
			SJF_ReadyQueue = sortBurstTimes(SJF_ReadyQueue, SJF_ReadyQueue-> size);
			last_update_ts = ts;
		}
	}

	SJF_Results-> context_switches = 0;
	return *SJF_Results;
}

struct processes KFactor(struct processes *process_queue, int n){
	printf("\n\nStarting KFactor Algorithm\n\n");
	int ts = 0, last_update_ts = 0, lp_n = 0;
	int k=2;

	// Initialise Ready Queue Linked List
	Processes * KFactor_ReadyQueue = (Processes*) malloc(sizeof(Processes));
	init_processes(KFactor_ReadyQueue);
	printf("Ready Queue initialised\n");

	// Initialise Output Linked List
	Processes * KFactor_Results = (Processes*) malloc(sizeof(Processes));
	init_processes(KFactor_Results);
	printf("Result buffer initialised\n");

	// Initialise Buffer Variables
	struct process *p;
	struct process *p1;
	struct process *p2;

	KFactor_ReadyQueue = addToReadyQueue(process_queue, KFactor_ReadyQueue, 0, 0);
	KFactor_ReadyQueue = sortBurstTimes(KFactor_ReadyQueue, KFactor_ReadyQueue-> size);
	printf("Initial arrivals added to ready queue\n");

	printf("KFactor results size: %d\tmain process queue size: %d\n", KFactor_Results-> size, process_queue-> size);

	int loop = 0;

	// Run till all processes have been calculated
	while(KFactor_Results-> size < process_queue-> size){

		printf("\n\nCurrent Result output has %d elements\n\n", KFactor_Results-> size);

		p = KFactor_ReadyQueue-> head;

		// If ready Queue is not empty
		if(KFactor_ReadyQueue-> head != NULL){
			// If not first process to ever be scheduled and Ready Queue has more than 1 element
			if(loop > 0 && p-> next != NULL){

				printf("Checking for KFactor\n");

				p1 = p;
				p2 = p->next;

				printf("k=%d, %d, %d\n",k , k * p1-> burst_time, ts + p2-> burst_time - p2-> arrival_time);

				if( (k * p1-> burst_time) > (ts + p2-> burst_time - p2-> arrival_time) ){
					p = swapProcessNodes(KFactor_ReadyQueue, p, KFactor_ReadyQueue-> size);

				}

			}
			
			p-> waiting_time = ts - p-> arrival_time;
			p-> turnaround_time = ts + p->burst_time;
			p-> response_time = p-> waiting_time;

			insert_node(KFactor_Results, p);

			KFactor_ReadyQueue-> head = p-> next;
			KFactor_ReadyQueue-> size--;

			ts = ts + p-> burst_time;
			
			if(loop != 0 && (loop+1) % 2 == 1){
			k++;
			}
			
			printf("Loop: %d\n", loop);
			loop++;
			printf("Loop: %d\n", loop);
		}else if((KFactor_ReadyQueue-> size + KFactor_Results-> size) != process_queue-> size){
			printf("Nothing in the Ready Queue\n");
			// Find the next arrival time and time travel to that point in time
			ts = findSmallestArrivalTime(process_queue, ts, last_update_ts);
			printf("Time travel to T = %d\n\n", ts);
		}

		if(	(KFactor_ReadyQueue-> size + KFactor_Results-> size) != process_queue-> size){
			printf("Checking for new arrivals\n");
			// Check for new arrivals
			KFactor_ReadyQueue = addToReadyQueue(process_queue, KFactor_ReadyQueue, last_update_ts, ts);
			KFactor_ReadyQueue = sortBurstTimes(KFactor_ReadyQueue, KFactor_ReadyQueue-> size);
			last_update_ts = ts;
		}
	}
	KFactor_Results-> context_switches = 0;
	return *KFactor_Results;
}

void exportResults(struct processes *result, char *dataset, char *type){
	char *dir = (char *) malloc(50);
	char *filename = (char *)malloc(50);
	struct process *p = result-> head;

	FILE *fp;

	printf("Setting up directory\n");
	printf("%s\n", type);
	dir = "test_related/test_results/paper_2/";
	dir = strcat(dir, type);
	printf("%s", dir);
	dir = strcat(dir, "/");

	printf("Setting Filename\n");
	filename = strcat(dataset,".csv");

	printf("Creating File\n");
	fp = fopen(strcat(dir, filename), "w+");

	printf("Writing headers\n");
	fprintf(fp, "pid, bursttime, arrivaltime, priority, turnaroundtime, waittime, responsetime, contextswitches");
	
	printf("Writing results\n");
	while(p!= NULL){
		fprintf(fp,"\n%d,%d,%d,%d,%d,%d,%d,0", p->pid, p->burst_time, p->arrival_time, p->priority,p->turnaround_time,p->waiting_time,p->waiting_time);
		p = p-> next;
	}

	printf("Closing file");
	fclose(fp);
}

int main(){
	int n;
	char has_test_case;
	char *filename = (char *) malloc(50);
	char *file_contents;
	char *dir = (char *) malloc(1000);
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

		//printf("%s", filename);

		//read_file(&filename, &file_contents);

		get_processes(filename, &process_queue);

		p = process_queue->head;

		// Check if anything was found
		if(p == NULL){
			printf("No testcases found\nExiting Application\n");
			exit(EXIT_FAILURE);
		}

		// Debug
		while(p != NULL){
			printf("PID:%d, BT:%d, AT:%d, Prio:%d\n", p->pid, p->burst_time, p->arrival_time, p->priority);
			p = p->next;
		}

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

	//KFactorLoop
	Processes * KFactor_IN = (Processes*)malloc(sizeof(Processes));
	init_processes(KFactor_IN);
	memcpy(KFactor_IN, process_queue, sizeof(Processes));

	struct processes KFactor_Results = KFactor(KFactor_IN, n);

	p = SJF_Results.head;
	for(int i = 0; i < SJF_Results.size; i++){
		printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
	 	p=p->next;
	}
	// exit(EXIT_SUCCESS);

	printf("\n\n\n");

	p = KFactor_Results.head;
	for(int i = 0; i < KFactor_Results.size; i++){
		printf("Process %d - Burst time: %d\tArrival time: %d\tWait time: %d\tTurnaround time: %d\n", p->pid, p->burst_time, p->arrival_time, p->waiting_time, p->turnaround_time);
		p=p->next;
	}

	printf("Exporting SJF\n\n");
	filename = strrchr(filename, '/') + 1;
	filename = strtok(filename, ".");

	int retcode;

	// Write result for SJF
	sprintf(dir, "test_related/test_results/paper_2/SJF/%s.csv", filename);
	printf("%s", dir);
	retcode = write_results(dir, &SJF_Results);

	if(retcode == -1){
		printf("File not created\n");
		exit(EXIT_FAILURE);
	}

	// Write result for KFactor
	sprintf(dir, "test_related/test_results/paper_2/KFactor/%s.csv", filename);
	printf("%s", dir);
	retcode = write_results(dir, &SJF_Results);

	if(retcode == -1){
		printf("File not created\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}