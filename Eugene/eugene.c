#include <stdio.h>


void ascending(int array[], int number){
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


int main(void) { // Main program
  int i, j, n, et_buffer, loop,tat[10], TQ, ct[10], max_process_time, burst_times[10], response_times[10];
  float awt = 0.0, att=0.0, art=0.0;


printf("Enter the no of processes:");
scanf("%d", &n);

int arr_times[n], process_q[n], exe_times[n], wait_times[n], ready_q[n];
int process_buffer, ex_buffer;


for(i = 0; i < n; i++){
  printf("\nEnter Execution Time for process %d: ",i + 1);
  scanf("%d", &exe_times[i]); //Storing burst times into the burst time array
}

for(i = 0; i < n; i++){
  printf("\nEnter Arrival Time for process %d: ",i + 1);
  scanf("%d", &arr_times[i]); //Storing arrival times into the burst time array
}

//Getting the time quantum
printf("\nEnter the size of time quantum:");
scanf("%d",&TQ);
max_process_time=exe_times[0];


for(i=1;i<n;i++) //Finding the biggest process time among the processes
  {
    if(max_process_time<exe_times[i])
      max_process_time=exe_times[i];
  }
  

//Sorting processes based on arrival time (ascending order)

//This is only the initial arrangement
for(i = 0; i< n; i++){ //populating the queue with processes
    process_q[i] = i+1;
    //printf("Process: %d", process_q[i]);
}

for (i = 0; i < n; ++i) {
  for (j = i + 1; j < n; ++j) {
    if (arr_times[i] > arr_times[j]){
      et_buffer =  arr_times[i];
      arr_times[i] = arr_times[j];
      arr_times[j] = et_buffer;

      process_buffer = process_q[i]; //Inital arrangement based on arrival time
      process_q[i] = process_q[j];
      process_q[j] = process_buffer;

      ex_buffer = exe_times[i];
      exe_times[i] = exe_times[j];
      exe_times[j] = ex_buffer;
      
      
    }
  }
}
// // NEED to arrange based on shortest remaining time left after first process is allocated

// for(i=1;i<n;i++) //Finding the biggest process time among the processes
//   {
//     if(max_process_time<exe_times[i])
//       max_process_time=exe_times[i];
//   }



//Need a timer to keep track based on the arrival times
int timer = 0;

  
while (timer < 100){
        for(i=0; i <n; i++){
          if (timer == arr_times[i]){
            
            //Add to ready queue
            ready_q[i] = process_q[i];
            printf("\nThis process %d is inserted at %d seconds\n", process_q[i], timer);

            // Allocate CPU to the first process
            printf("Execution time: %d", exe_times[i]);
            exe_times[i] = exe_times[i] - TQ;
            printf("\nWhats left %d",exe_times[i]);
            //If execution time != 0; add it to the back of the queue


            //Arranging execution times
            for (int u = 0; u < n; ++u) {
              for (int o = u + 1; o < n; ++o) {
                ex_buffer = exe_times[u];
                exe_times[u] = exe_times[o];
                exe_times[o] = ex_buffer;
              }
            }
            printf("\nNext in Ready Queue: Process %d", ready_q[i]);
            printf("\nCurrent time %d", timer);
            

         
            
          }

        }
  
      
  timer++;
 

}




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

    

    }
  

