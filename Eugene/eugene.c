#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct process{
  int process_number;
  int burst_time;
  int arrival_time;
  int waiting_time;
  int turnaround_time;

} Process;

void compare_burst_times(){

}



void shift_left(int array[], int n){
  int temp;
    temp=array[0];
    for(int i=0;i<n-1;i++)
    {
        array[i]=array[i+1];
    }
    array[n-1]=temp;
}


void insert_element(int array[], int new_element, int position, int n){
  for(int i = n - 1; i > position; i--){
    array[i] = array[i-1];

  }
   array[position] = new_element;
 
}


void insert_new(int array[], int n, int element){
  for(int i  = 0 ; i < n ; i++){
    if(array[i] == 0){
      array[i] = element;
      break;
      }
}
}


void shift_right(){

}


void ascending(int array[], int number){
	// Arranges Ready Queue according to burst time in ascending order
  	int array_buffer;
	for (int i = 0; i < number; ++i) {
		for (int j = i + 1; j < number; ++j) {
			if (array[i] > array[j]){
				array_buffer = array[i];
				array[i] = array[j];
				array[j] = array_buffer;
			}
		}
	}
}

int main(void) {
	// Main program
  	int i, j, n, et_buffer, loop,tat[10], TQ, ct[10], max_process_time, response_times[10], max_arrival_time, size_t, lmao_buffer;
  	float awt = 0.0, att=0.0, art=0.0;



printf("Enter the no of processes:");
scanf("%d", &n);

struct process process[n]; //Creating the number of processes

for(i = 0; i < n; i++ ) //Getting process inputs
    {
        printf("Enter burst time for process %d:", i+1);
        scanf("%d", &process[i].burst_time);


        printf("Enter Arrival time for process %d:", i+1);
        scanf("%d", &process[i].arrival_time);

        process[i].process_number = i+1;
    }



	//Getting the time quantum
	printf("\nEnter the size of time quantum:");
	scanf("%d", &TQ);


//Finding biggest process time
  max_process_time = 0;
	for(i = 0; i < n; i++){
       
    	if(max_process_time < process[i].burst_time){
			  max_process_time = process[i].burst_time;
		}
  	}

//Find latest arrival
  max_arrival_time = 0;
	for(i = 0; i < n; i++){
    	if(max_arrival_time < process[i].arrival_time){
			  max_arrival_time = process[i].arrival_time;
		}
  	}



for(i = 0; i < n; i++){
  printf("Process %d \t burst time: %d \t arrival time: %d\n", i+1, process[i].burst_time, process[i].arrival_time);
}

int ready_q[n];
      //Control variable to check if array is empty
for(i=0; i < n; i ++){
  ready_q[i] = 0;
  printf("%d\n", ready_q[i]);
}



	// int arr_times[n], process_q[n], burst_times[n], wait_times[n], ready_q[n];
	// int process_buffer, ex_buffer;


	// for(i = 0; i < n; i++){
  // 		printf("\nEnter Execution Time for process %d: ",i + 1);
  // 		scanf("%d", &burst_times[i]); //Storing burst times into the burst time array
	// }

	// for(i = 0; i < n; i++){
  // 		printf("\nEnter Arrival Time for process %d: ",i + 1);
  // 		scanf("%d", &arr_times[i]); //Storing arrival times into the burst time array
	// }


  

	//Sorting processes based on arrival time (ascending order)

	//This is only the initial arrangement
	// for(i = 0; i< n; i++){ //populating the queue with processes
  //   	process_q[i] = i+1;
  //   	//printf("Process: %d", process_q[i]);
	// }

	// // Arrange burst time based on arr time
	// for (i = 0; i < n; ++i) {
  // 		for (j = i + 1; j < n; ++j) {
	// 		if (arr_times[i] > arr_times[j]){
	// 		et_buffer =  arr_times[i];
	// 		arr_times[i] = arr_times[j];
	// 		arr_times[j] = et_buffer;

	// 		process_buffer = process_q[i]; //Inital arrangement based on arrival time
	// 		process_q[i] = process_q[j];
	// 		process_q[j] = process_buffer;

	// 		ex_buffer = burst_times[i];
	// 		burst_times[i] = burst_times[j];
	// 		burst_times[j] = ex_buffer;
	// 		}
  // 		}
	// }
// // NEED to arrange based on shortest remaining time left after first process is allocated

// for(i=1;i<n;i++) //Finding the biggest process time among the processes
//   {
//     if(max_process_time<exe_times[i])
//       max_process_time=exe_times[i];
//   }

#pragma region Eugene code
	// int ts = 0;	//Initialize timestamp to 0
	// int max_arr_time = 0;
	// int ready_queue[n];
	// int last_addition_timestamp = -1;

	// Find last arr time
	// for (i = 0; i < n; i++){
	// 	if(arr_times[i] > max_arr_time){
	// 		max_arr_time = arr_times[i];
	// 	}
	// }

	// printf("\n **DEBUG**\n\nLast Arrival time: %d\n", max_arr_time);

	// while (ts <= max_arr_time + max_process_time){
	// 	// Check for new processes
	// 	for (i = 0; i < n; i++){
	// 		if(last_addition_timestamp < arr_times[i] && arr_times[i] <= ts){
	// 			ready_queue[i] = i;
	// 			printf("In Ready Queue: P%d at t=%d\n", ready_queue[i]+1, ts);
	// 		} 
	// 	}
	// 	last_addition_timestamp = ts;
	// 	// printf("CP1\n");

	// 	bool exec = false;
	// 	for(i=0; i < n; i++){
	// 		if(burst_times[ready_queue[i]] > 0 && ready_queue[i] < n){
	// 			exec = true;

	// 			printf("n = %d\n", i);
	// 			printf("process: %d\n", ready_queue[i]+1);
	// 			printf("Burst time: %d\n", burst_times[ready_queue[i]]);

	// 			// Execute the process remember to populate the burst times
	// 			if(burst_times[ready_queue[i]] > TQ){
	// 				ts = ts + TQ;
	// 				burst_times[ready_queue[i]] = burst_times[ready_queue[i]] - TQ;
	// 				printf("Executed P%d, remaining burst time: %dms\n", ready_queue[i]+1, burst_times[ready_queue[i]]);
	// 			}
	// 			else{
	// 				ts = ts + burst_times[ready_queue[i]];
	// 				burst_times[ready_queue[i]] = 0;
	// 				printf("Executed P%d, remaining burst time: %dms\n", ready_queue[i]+1, burst_times[ready_queue[i]]);
	// 			} 
	// 		}
	// 	}


	// 	// If notthing was executed, increment timestamp by 1
	// 	if(!exec){
	// 		printf("This shouldnt show");
	// 		ts++;
	// 	}

	// 	printf("CP2\n");
	// }

// #pragma endregions

// //Need a timer to keep track based on the arrival times
	int timer = 0;
	while (timer <= max_arrival_time + max_process_time){
		for(i = 0; i < n; i++){
			if (timer == process[i].arrival_time){  //If new process arrive
      printf("Current process at first position:%d\n", ready_q[0]);
      printf("Current process burst time: %d\n", process[ready_q[0]-1].burst_time);
          if (ready_q[0] == 0){ //If no new process exist in ready queue
            	ready_q[0] = process[i].process_number; //Add to front of ready queue

              printf("\nThis process %d is inserted at %d seconds\n", process[i].process_number, timer);
              printf("Burst time:%d\n", process[process[i].process_number -1].burst_time);
          }
        
          else { //Compare exe time of process with the process in the queue from front to rear
                printf("Process number waiting: %d\n", process[i].process_number); 
           
                for(j = 0; j < n; j++){
                  if(process[i].burst_time < process[ready_q[j]-1].burst_time){

                  insert_element(ready_q, process[i].process_number, j , n);
                 


                  break;
                  }
                  else if (process[i].burst_time > process[ready_q[j]-1].burst_time &&
                  process[i].burst_time < process[ready_q[n-1]-1].burst_time){
                      continue;
                  }


                  else //Insert to the rear
                  {
                    printf("Process %d should be inserted to the rear", process[i].process_number);
              
                    // printf("Bigger process time should come here");
                    // printf("Curent ready queue first position%d", ready_q[0]);
                    //insert_new(ready_q, n, process[i].process_number);
                    }
                  }
         
                }
              
               
           }
            
           
        //    else if(ready_q[0] != 0){ //Process exist
        //    printf("Process alr exist");
        //       if(process[ready_q[0]-1].burst_time <= TQ){
        //          process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
            
                 
        //       }
        //        process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
        //        printf("Process %d remaining time %d", process[ready_q[0]-1].process_number, process[ready_q[0]-1].burst_time);
        // }
  
      
	}
   printf("\nReady queue first position:%d\n", ready_q[0]);
            // if(process[ready_q[0]-1].burst_time == process[ready_q[0]-1].burst_time - TQ){ //Allocate CPU based on TQ and check if its exceeds.
            //           shift_left(ready_q,n);
  for(i = 0;i <n;i ++){
    printf("%d", ready_q[i]);
  }
                    
	timer++;
  printf("\nCurrent time %d\n", timer);
  }
  
  
  }

        //   if (process[i].burst_time <= TQ && process[i].burst_time == timer){
        //     process[i].burst_time = 0;
        //     printf("time left for process %d: %d", process[i].process_number, process[i].burst_time);
        // }  
          
         

        //   else{
            
        //       process[i].burst_time = process[i].burst_time - 1;
        //     }
            
        //   printf("time left for process %d: %d", process[i].process_number, process[i].burst_time);
        // }

        // else if(ready_q[i] != 0){
        //   if(process[ready_q[i]-1].burst_time <= TQ){
        //       process[ready_q[i]-1].burst_time = process[ready_q[i]-1].burst_time - 1;
        //   }
        // }

          //printf("\n%d\n", ready_q[i]);
          
          // for (int i = 0; i < n; ++i) { //Arrangning the processes
		      //   for (int j = i + 1; j < n; ++j) {
			    //     if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] != 0){
				  //         lmao_buffer = ready_q[i];
				  //         ready_q[i] = ready_q[j];
				  //         ready_q[j] = lmao_buffer;
			    //     }
		      //   }
	        // }
          //printf("\nNext in ready queue:%d", ready_q[i]);
          // if(process[i].process_number == ready_q[i]){
          //   printf("\n%d\n",process[i].burst_time);
          // }

          //printf("%d",process[ready_q[i]].burst_time);
          // for(j = i +1; j < n; j++){

          // }
  
      // else{ //If no new process added to queue
      //     if(ready_q[0] != 0){
      //      for(i = 0; i<n; ++i){
      //        for(j = i + 1 ; j<n; ++j)
      //        if(process[ready_q[i]].burst_time > process[ready_q[j]].burst_time){
      //             lmao_buffer = process[ready_q[i]].burst_time;
			// 	          process[ready_q[i]].burst_time = process[ready_q[j]].burst_time;
			// 	          process[ready_q[j]].burst_time = lmao_buffer;
      //        }
      //      }
      //       if (process[ready_q[i]].burst_time <= TQ){
      //       process[ready_q[i]].burst_time = 0;
        
      //   }  
      //       else{
      //       process[ready_q[i]].burst_time = process[ready_q[i]].burst_time - TQ;
           
      //   }
      //     }
      //     else{
      //       printf("execution done");
      //     }
      //   }


      




        //Compare execution times Again


				// printf("\nWhats left %d",burst_times[i]);
				// //If execution time != 0; add it to the back of the queue


				// //Arranging execution times
				// // for (int u = 0; u < n; ++u) {
				// // for (int o = u + 1; o < n; ++o) {
				// // 	ex_buffer = burst_times[u];
				// // 	burst_times[u] = burst_times[o];
				// // 	burst_times[o] = ex_buffer;
				// // }
				// // }
				// printf("\nNext in Ready Queue: Process %d", ready_q[i]);
		

  

    //timer++;
  
      
    //     ready_q[timer] = process_q[timer];    //Add process to the queue if time is reached
    //     printf("Ready queue: %d", ready_q[timer]);
 
     //allocate CPU to first process, arrival time = 0, minus the tq off the first
   //Continue the timer
  // printf("\nAfter %d seconds:", timer);
  // // printf("\nReady queue:\n");
  // // printf("Process %d:",ready_q[timer]);
  // timer++;
  
  //  for(loop = 0; loop < n; loop++) {
  //      printf("Process %d:",ready_q[loop]);
  //  }   
// printf("Ready queue:\n");
//    for(loop = 0; loop < n; loop++) {
//      printf("Process %d:",ready_q[loop]);
//    }
  
  



//Add the rest of the processess to the ready_q


//Compare exe_time of process with the existing processes from front to rear





//Test printing

// printf("\nArrival Times:\n");
//    for(loop = 0; loop < n; loop++) {
//      printf("Process %d: arrival time: %d\t",process_q[loop],arr_times[loop]);
//    }
// // printf("Ready queue:\n");
//     for(loop = 0; loop < n; loop++) {
//       printf("Process %d:",process_q[loop]);
//    }

// printf("\nTime quantum: %d\n", TQ);
// for (i = 0; i < n; i++){
//      printf("\nProcess %d:\t",i+1);
//      printf("Execution time: %d\t", exe_times[i]);
//      printf("Arrival time: %d\t", arr_times[i]);
//  }



    
  

