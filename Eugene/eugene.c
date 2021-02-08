#include <stdio.h>

int main(void) { // Main program
  int i, j, n, loop,tat[10], TQ, ct[10], max_process_time, burst_times[10], response_times[10];
  float awt = 0.0, att=0.0, art=0.0;


printf("Enter the no of processes:");
scanf("%d", &n);

int arr_times[n], process_q[n], exe_times[n], wait_times[n], ready_q[n];


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


//Duplcating another burst times array
for(i=0; i<10; i++)
{
    burst_times[i] = exe_times[i];
}

//Sorting processes based on arrival time


for(i=1;i<n;i++) //Finding the biggest process time among the processes
  {
    if(max_process_time<exe_times[i])
      max_process_time=exe_times[i];
  }


//allocate CPU to first process


//Add the rest of the processess to the ready_q


//conditions if new process arrive

//Test printing
  for(loop = 0; loop < n; loop++) {
    printf("\nProcess %d:\t",loop+1);
    printf("Execution time: %d\t", exe_times[loop]);
    printf("Arrival time: %d\t", arr_times[loop]);
  }
printf("\nTime quantum: %d\n", TQ);
}

