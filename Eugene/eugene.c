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

void remove_element(int array[] , int element, int array_length)
{
   
   for(int i = 0; i < array_length; i ++){
      if (array[i] == element){
        array[i] = 0;
      }
   }
}


void shift_left(int array[], int n){
  int temp, lol;
    temp=array[0];

    for(int i=0; i < n ; i++)
    {
        
        array[i]=array[i+1];

    }
    
      for(int i=0; i < n ; i++)
    {
       if(array[i] == 0){
         array[i] = temp;
         break;
       }
    }

    array[n-1]=0;

  
}


void insert_element(int array[], int new_element, int position, int n){
  for(int i = n - 1; i > position; i--){
    array[i] = array[i-1];

  }
   array[position] = new_element;
 
}


void add_to_rear(int array[], int n, int element){
  for(int i  = 0 ; i < n ; i++){
    if(array[i] == 0){
      array[i] = element;
      break;
      }
}
}

int sum_of_elements(int array[], int array_size){
  int sum;
  for(int i =0; i < array_size; i++)
    {
        sum = sum + array[i];
    }
    return sum;
}

int get_zeros(int array[],int array_length){
int no_of_0 = 0;
for (int i = 0; i < array_length; i++){
  if(array[i]== 0){
    no_of_0 += 1;
  }

}
return no_of_0;
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
  	int i, j, n, et_buffer, loop,tat[10], TQ, ct[10], max_process_time, response_times[10], max_arrival_time, size_t, lmao_buffer, total_burst_time; int next_exe_time_start=0, total_arr_time, time_added;
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

//Initialize all values of the waiting time to zero
for (i = 0; i < n; i++){
  process[i].waiting_time = 0;
}

for(i = 0; i < n; i++){
  printf("Process %d \t burst time: %d \t arrival time: %d \t waiting time: %d\n", i+1, process[i].burst_time, process[i].arrival_time, process[i].waiting_time);
  
}

int ready_q[n];
      //Control variable to check if array is empty
for(i=0; i < n; i ++){
  ready_q[i] = 0;
  printf("\n%d\n", ready_q[i]);
}



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

#pragma endregions

//Calculate total burst time
for(i = 0; i < n; i ++){
   total_burst_time += process[i].burst_time;
   printf("Total process time needed: %d", total_burst_time);
}
for(i = 0; i < n; i ++){
   total_arr_time += process[i].arrival_time;
  
}



//Need to arrage base on arrival times
int arr_times[n];
for(i = 0; i <n; i ++){
  arr_times[i] = process[i].arrival_time;
}
ascending(arr_times, n);
next_exe_time_start = arr_times[0] + TQ;
printf("\n1st next start time %d", next_exe_time_start);

for(i = 0; i <n; i++){
  printf("\nArrival times ascending: %d", arr_times[i]);
}
// // Getting timestamps where it changes
// int timestamps[n];
// timestamps[0] = arr_times[0] + TQ;
// for(i = 0; i < n; i++){
  
//   if(arr_times[i+1] <= arr_times[i] + TQ ){
//     if(){

//     }
//    timestamps[i] += TQ;
//   }

//   else{
//     timestamps[i] = process[ready_q[i]-1].arrival_time +TQ;
//   }
// }




// //Need a timer to keep track based on the arrival times, after every cycle of TQ, rearrange, if process arrive before TQ finish, add to rear first. 
	int timer = 0;
  int new_counter = 0;

	while (timer <= total_burst_time + total_arr_time){
    
    
    printf("\nCurrent time %d\n", timer);
       printf("\nReady Queue:");
  for(i = 0; i < n; i++){
 
    printf("\n%d", ready_q[i]);
  }
    if(ready_q[0] != 0){ //Check to see if process q is empty
    process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
    }
    int ar_buffer = ready_q[0];

		for(i = 0; i < n; i++){
      
      
			if (timer == process[i].arrival_time && process[i].arrival_time == process[i-1].arrival_time && i > 0
      && ar_buffer == 0){  //If >1 processes comes in at t=0 check to see which one has smaller exe time. 
        new_counter = 1;
        add_to_rear(ready_q, n, process[i].process_number);
        printf("First process arrival time %d\n", process[i].arrival_time);
        printf("Comparing process arrival time %d \n",process[i-1].arrival_time);
        printf("First process should not be here");
        //Sort ascending
       
        for (int i = 0; i < n; ++i) {
		        for (int j = i + 1; j < n; ++j) {
			          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
                ready_q[i] !=0 && ready_q[j] !=0){
				        lmao_buffer = ready_q[i];
				        ready_q[i] = ready_q[j];
			          ready_q[j] = lmao_buffer;
			          }
		          }
	          }
            
      }

      else if(timer == process[i].arrival_time){ //Since CPU is already allocated after one second, add to rear.
          new_counter = 1;
          printf("Should be added to the rear");
          add_to_rear(ready_q, n, process[i].process_number);
          // if(next_exe_time_start == 0){
          //   next_exe_time_start = process[i].arrival_time;
          // }
          
      }
    }
  //   int number_of_zeros = get_zeros(ready_q, n);
    
  //   int last_number_zeros;
 
  //  if (number_of_zeros < last_number_zeros){ //Check when the process came in
  //  time_added = timer;
   

      // next_exe_time_start = process[ready_q[0]-1].arrival_time + TQ ;
      // printf("next exe time %d", next_exe_time_start);


  //     else if(process[ready_q[0]-1].burst_time == 0 && timer > 0 && ready_q[0] != 0){ //at any point t > 0, check if process has finished execution
  //       // if(process[ready_q[1]-1].arrival_time > previous_exe_time_start + TQ){
  //       //    previous_exe_time_start = process[ready_q[1]-1].arrival_time;
  //       // }
  //       // else{
  //       //     previous_exe_time_start = timer;        
  //       // }
  //       next_exe_time_start = timer + TQ;
  //               printf("Process has finished execution please rearrange");
  //                remove_element(ready_q, 0 ,n);
  //               //Rearange and print out before removing
  //                           for (int i = 0; i < n; ++i) {
	// 	            for (int j = i + 1; j < n; ++j) {
	// 		          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
  //               ready_q[i] !=0 && ready_q[j] !=0){
	// 			        lmao_buffer = ready_q[i];
	// 			        ready_q[i] = ready_q[j];
	// 		          ready_q[j] = lmao_buffer;
	// 		}
	// 	}
	// }
  //   printf("\nReady Queue:");
  // for(i = 0; i < n; i++){
 
  //   printf("\n%d", ready_q[i]);
   
  // }
  //       }
    if(process[ready_q[0]-1].burst_time == 0 && ready_q[0] != 0){
      
      
    
    printf("Process %d has finished execution", process[ready_q[0]-1].process_number);
    next_exe_time_start = timer + TQ;

    if(new_counter > 0){ //If process time is up, and new process added, rearrange
        printf("new process was added before time is up");


           remove_element(ready_q, process[ready_q[0]-1].process_number ,n);
            shift_left(ready_q, n);
                //Rearange and print out before removing
              for (int i = 0; i < n; ++i) {
		            for (int j = i + 1; j < n; ++j) {
			          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
                ready_q[i] !=0 && ready_q[j] !=0){
				        lmao_buffer = ready_q[i];
				        ready_q[i] = ready_q[j];
			          ready_q[j] = lmao_buffer;
			}
		}
	}
  new_counter = 0; //Resetting the time stamp where it last got added
    }



    else{
      printf("No need process was added");
     remove_element(ready_q, ready_q[0] , n);
      printf("After removing: %d", ready_q[0]);
      shift_left(ready_q, n);
                //Rearange and print out before removing
  //             for (int i = 0; i < n; ++i) {
	// 	            for (int j = i + 1; j < n; ++j) {
	// 		          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
  //               ready_q[i] !=0 && ready_q[j] !=0){
	// 			        lmao_buffer = ready_q[i];
	// 			        ready_q[i] = ready_q[j];
	// 		          ready_q[j] = lmao_buffer;
	// 		}
	// 	}
	// }
    }
          printf("\nReady Queue:");
  for(i = 0; i < n; i++){
 
    printf("\n%d", ready_q[i]);
  }
  }
  
    
    else if(timer == next_exe_time_start && process[ready_q[0]].process_number != 0){

       if(new_counter > 0){
      printf("Process is added before this, need to sort");
                    for (int i = 0; i < n; ++i) {
		                for (int j = i + 1; j < n; ++j) {
			          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
                ready_q[i] !=0 && ready_q[j] !=0){
				        lmao_buffer = ready_q[i];
				        ready_q[i] = ready_q[j];
			          ready_q[j] = lmao_buffer;
			}
		}
	}
  new_counter = 0;
    }
    else{
      shift_left(ready_q, n);
    }
        //  if(process[ready_q[1]-1].arrival_time > previous_exe_time_start + TQ){
        //    previous_exe_time_start = process[ready_q[1]-1].arrival_time;
         
        // }
        // else{
        //     previous_exe_time_start = timer;        
        // }
      
        
          // if(process[ready_q[0]-1].burst_time != 0 && ready_q[1] != 0){

          //   //Need to check if its the only process left, if only process left dont rearrange. 

          //     printf("\nRearrange processes in ascending order based on burst time but front has not finished execution");

          //     shift_left(ready_q, n);

          // }
          //If process finished execution, just rearrage without process
          printf("Next start time %d", next_exe_time_start);
          printf("Process has not finished execution but tq is up");
  //             for (int i = 0; i < n; ++i) {
	// 	                for (int j = i + 1; j < n; ++j) {
	// 		          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
  //               ready_q[i] !=0 && ready_q[j] !=0){
	// 			        lmao_buffer = ready_q[i];
	// 			        ready_q[i] = ready_q[j];
	// 		          ready_q[j] = lmao_buffer;
	// 		}
	// 	}
	// }
     if(process[ready_q[0]-1].process_number != 0){ 
        next_exe_time_start = next_exe_time_start + TQ;
      }
      else{
        next_exe_time_start = 0;
      }





//Geting waiting time


       
    printf("\nBurst time left for process : %d", process[ready_q[0]-1].burst_time);
    
                
      printf("\nReady Queue:");
  for(i = 0; i < n; i++){
 
    printf("\n%d", ready_q[i]);
  }

    }
   

  


  //     else{ //If no new process exist
            
              
  //       if(timer % TQ == 0 && timer > 0){

  //       if(process[ready_q[0]].burst_time == 0){ //Remove from array if burst time = 0; and rearrange
  //               printf("Process has finished execution");

  //       }
          
  //         printf("\nRearrange processes in ascending order based on burst time"); 
  //         printf("\nBurst time for process %d in ready queue: %d", process[ready_q[i]].process_number, process[ready_q[i]].burst_time);
    
  //               for (int i = 0; i < n; ++i) {
	// 	                for (int j = i + 1; j < n; ++j) {
	// 		          if (process[ready_q[i]-1].burst_time > process[ready_q[j]-1].burst_time && ready_q[i] && 
  //               ready_q[i] !=0 && ready_q[j] !=0){
	// 			        lmao_buffer = ready_q[i];
	// 			        ready_q[i] = ready_q[j];
	// 		          ready_q[j] = lmao_buffer;
	// 		}
	// 	}
	// }
  //   }

  //           }
            
// process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
  
      
      

    //Remove process from ready q array if burst time = 0;
  


 
        
      
    


          // if (ready_q[0] == 0){ //If no new process exist in ready queue
          //   	ready_q[0] = process[i].process_number; //Add to front of ready queue

          //     printf("\nThis process %d is inserted at %d seconds\n", process[i].process_number, timer);
          //     printf("Burst time:%d\n", process[process[i].process_number -1].burst_time);
          // }
        
          // else { //Compare exe time of process with the process in the queue from front to rear
          //       printf("Process number waiting: %d\n", process[i].process_number); 
               
          //       for(j = 0; j < n; j++){
          //         if(process[i].burst_time < process[ready_q[j]-1].burst_time){ 
                  


          //         insert_element(ready_q, process[i].process_number, j+1 , n);
                 

          //         break;
          //         }
          //         else if (process[i].burst_time > process[ready_q[j]-1].burst_time &&
          //         process[i].burst_time < process[ready_q[n-1]-1].burst_time){
          //             continue;
          //         }


          //         else //Insert to the rear
          //         {
          //           printf("Process %d should be inserted to the rear", process[i].process_number);
              
          //           // printf("Bigger process time should come here");
          //           // printf("Curent ready queue first position%d", ready_q[0]);
          //           //  insert_new(ready_q, n, process[i].process_number);
          //           }
          //         }
         
          //       }
              
               
         
            
           
        //    else if(ready_q[0] != 0){ //Process exist
        //    printf("Process alr exist");
        //       if(process[ready_q[0]-1].burst_time <= TQ){
        //          process[ready_q[0]-1].burst_time = 0;
        //       }
        //       else{
        //         process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
        //       }
        // //        process[ready_q[0]-1].burst_time = process[ready_q[0]-1].burst_time - 1;
        // //        printf("Process %d remaining time %d", process[ready_q[0]-1].process_number, process[ready_q[0]-1].burst_time);
        // }
  
      

  
            // if(process[ready_q[0]-1].burst_time == process[ready_q[0]-1].burst_time - TQ){ //Allocate CPU based on TQ and check if its exceeds.
            //           shift_left(ready_q,n);


// printf("Timer buffer %d", timer - process[ready_q[0]-1].arrival_time);
  for (i = 0; i < n; i++){
  if(process[i].process_number != ready_q[0] && process[i].burst_time != 0){
    process[i].waiting_time += 1;
  }
}



// printf("Latest number of zeros %d", number_of_zeros);
// printf("Initial number of zeros %d", last_number_zeros);
//   last_number_zeros = number_of_zeros;
	timer++;
}
  }
//   for(i = 0; i < n; i++){
//   printf("\nProcess %d waiting times %d",process[i].process_number ,process[i].waiting_time - process[i].arrival_time);
  
// }


 

        //   if (process[i].burst_time <= TQ && process[i].burst_time == timer){
        //     process[i].burst_time = 0;
        //     printf("time left for process %d: %d", process[i].process_number, process[i].burst_time);
        // }  
          
         

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



    
  

