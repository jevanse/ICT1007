// Paper 2 Implementation	-	K-Factor CPU Scheduling Algorithm
// Author: Clement Leow
// Date Created: 09/02/2021 1130H
// Date Updated: 10/02/2021

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

void swap(int* xp, int* yp){
	/*
	* Function: swap
	* ------------------
	* swaps the values in an array
	* *xp: 	values pointed to by pointer x
	* *yp: 	values pointer to by pointer yp
	* temp: Temporary buffer
	*/

	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void sortBurstTimes(int process_bt[], int n){
	/*
	* Function: sortBurstTimes
	* ------------------
	* sorts the array of process burst times in ascending order
	* process_bt[]	: 	array of process burst times
	* n				:	number of elements
	* i,j			:	counter variables
	* min_index		:	buffer variable to store index of smallest element
	*/
	int i, j, min_index;

	// Move array boundary
	for (i = 0; i < n - 1; i++){

		// Find minimum element in array
		min_index = i;
		for (j = i + 1; j < n; j++){
			if (process_bt[j] < process_bt[min_index])
				min_index = j;
		}

		// swap current boundary element with smallest element
		swap(&process_bt[min_index], &process_bt[i]);
	}
}

int main(){
#pragma region Variables
	/*	LEGEND
	*	i, j		: Counter Variables
	*	K	 		: Magic number
	*	n	 		: Number of processes
	*	bt[] 		: Array of Process burst times
	*	wt[] 		: Array of process wait times
	*	tat[]		: Array of process turn around times
	*	ts			: Timestamp
	*	twt			: Total wait time
	*	ttt			: Total turnaround time
	*	lp_twt		: Total wait time for low priority processes
	*	lp_ttt		: Total turnaround time for low priority processes
	*	awt			: Average wait time
	*	att			: Average turn around time
	*	lp_awt: Average wait time for low priority processes
	*	lp_att: Average turn around time for low priority processes	
	*
	*	high priority process	-	processes with burst time 1-15
	*	mid priority process	-	processes with burst time 16-50
	*	low priority process	-	processes with burst time 51-100
	*/
	int i, j, k = 2, n, bt[10000], wt[10000], tat[10000], ts = 0;
	float twt, ttt, lp_twt, lp_ttt;
	float awt, att, lp_awt, lp_att;
#pragma endregion

#pragma region Psuedo Random Number Generator
	// Initialize Psuedo-Random number generator
	time_t t;
	srand((unsigned) time(&t));
#pragma endregion
	
#pragma region Setup
	printf("How many processes to simulate: ");
	scanf("%d", &n);	

	// Randomly populate burst time for n number of processes
	for(i = 0; i < n; i++){
		bt[i] = (rand() % 100) + 1;
	}

	sortBurstTimes(bt, n);
#pragma endregion

#pragma region DEBUG
	printf("\nSorted burst times:\n");
	//for(i = 0; i < n; i++){
	//	printf("%d\t", bt[i]);
	//}
	//printf("\n");
#pragma endregion

#pragma region Standard SJF
	//printf("\nSJF process resolution order:\n");
	for (i = 0; i < n; i++){

		// If not first process to run
		if(i != 0){

			// Calculate data
			tat[i] = ts + bt[i];			//	Process turn around time
			wt[i] = ts;						//	Process wait time
			ttt = ttt + tat[i];				//	Add to total turn around time
			twt = twt + wt[i];				//	Add to total wait time

			// If process is low priority
			if(bt[i] > 50){
				lp_ttt = lp_ttt + tat[i];	//	Add to low prio total turn around time
				lp_twt = lp_twt + wt[i];	//	Add to low prio total wait time
			}
			
			// Update time stamp
			ts = ts + bt[i];
		}
		// If first process to run
		else{
			wt[i] = ts;
			tat[i] = ts + bt[i];

			// Add process burst time to total turn around time
			ttt = ttt + tat[i];

			// If process is low priority add turn over time to lp_ttt
			if(bt[i] > 50)
				lp_ttt = lp_ttt + tat[i];

			// Update timestamp
			ts = ts + bt[i];
		}

		//printf("%d\t", bt[i]);
	}
	//printf("\n");


	
	//printf("\n*DEBUG* twt:%.2fms\tttt:%.2fms\tlp_twt:%.2fms\tlp_ttt:%.2fms\n", twt, ttt, lp_twt, //lp_ttt);
	//
	//for(i = 0; i < n; i++){
	//	printf("p%d wt:%d, tat:%d\n",i+1,wt[i],tat[i]);
	//}

	awt 	= twt / n;
	att 	= ttt / n;
	lp_awt 	= lp_twt / n;
	lp_att 	= lp_ttt / n;

	printf("\n\nResults for Classic SJF\n");
	printf("_______________________________\n\n");		
	printf("Average wait time:\t%.2f\n", awt);
	printf("Average turn around time:\t%.2f\n", att);
	printf("Average low priority process wait time:\t%.2f\n", lp_awt);
	printf("Average low priority process turn around time:\t%.2f\n", lp_att);
	printf("_______________________________\n");
#pragma endregion

#pragma region K-Factor Algorithm

	// Reset Variables
	twt = 0, ttt = 0, lp_twt = 0, lp_ttt = 0, ts = 0;
	//printf("\nK-Factor Algoritm process resolution order:\n");

	for (i = 0; i < n; i++){
		// First/Last process execute
		if(i == 0 || i+1 >= n){

			//printf("%d\t", bt[i]);

			wt[i] = ts;
			tat[i] = ts + bt[i];
			ts = ts + bt[i];

			twt = twt + wt[i];
			ttt = ttt + tat[i];
		}
		else{
			// If process i + 1 is starved
			if ( (k * bt[i]) > (ts + bt[i+1]) ){


				wt[i+1] = ts;
				tat[i+1] = ts + bt[i+1];
				ts = ts + bt[i+1];
				

				twt = twt + wt[i + 1];
				ttt = ttt + tat[i + 1];

				if(bt[i + 1] > 50){
					lp_ttt = lp_ttt + tat[i + 1];	//	Add to low prio total turn around time
					lp_twt = lp_twt + wt[i + 1];	//	Add to low prio total wait time
				}
				
				//printf("%d\t",bt[i+1]);
				
				int temp = bt[i+1];
				bt[i+1] = bt[i];
				bt[i] = temp;
			}
			// If not starved
			else{

				wt[i] = ts;
				tat[i] = ts + bt[i];
				ts = ts + bt[i];

				twt = twt + wt[i];
				ttt = ttt + tat[i];

				if(bt[i] > 50){
					lp_ttt = lp_ttt + tat[i];	//	Add to low prio total turn around time
					lp_twt = lp_twt + wt[i];	//	Add to low prio total wait time
				}
				//printf("%d\t", bt[i]);
			}
		}

		// Increase K factor alternatively. First time will be during i = 3
		if (i != 0 && (i+1) % 2 == 1){
			k = k + 1;
		}

		// printf("\niteration = %d\ttime = %d\tK = %d\n",i, ts, k);
	}

	//printf("\n");


	// printf("\n*DEBUG* twt:%.2fms\tttt:%.2fms\tlp_twt:%.2fms\tlp_ttt:%.2fms\n\n", twt, ttt, lp_twt, lp_ttt);
	
	//for(i = 0; i < n; i++){
	//	printf("p%d wt:%d, tat:%d\n", i+1, wt[i],tat[i]);
	//}
	awt 	= twt / n;
	att 	= ttt / n;
	lp_awt 	= lp_twt / n;
	lp_att 	= lp_ttt / n;

	printf("\n\nResults for K - Factor Algorithm\n");
	printf("_______________________________\n\n");		
	printf("Average wait time:\t%.2f\n", awt);
	printf("Average turn around time:\t%.2f\n", att);
	printf("Average low priority process wait time:\t%.2f\n", lp_awt);
	printf("Average low priority process turn around time:\t%.2f\n", lp_att);
	printf("_______________________________\n");
#pragma endregion
}