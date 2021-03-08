#include <stdio.h>

int main(void) {
 
  int i, j, n, wa[10], tat[10], timeSlice, ct[10], a, max_process_time, temp = 0,  waiting_time = 0;
  float awt = 0.0, att=0.0;
  int loop;


printf("Enter the no of processes:");
scanf("%d", &n);
int burst_times[n]; //Creating the array based on the number of processes
int bu[n+1];
for(i = 0; i < n; i++){
  printf("\nEnter Burst Time for process %d: ",i + 1);
  scanf("%d", &burst_times[i]); //Storing burst times into the burst time array
}
for (i = 0; i < n; ++i) { //Arranging in ascending order
    for (j = i + 1; j < n; ++j) {
      if (burst_times[i] > burst_times[j]){
                    a =  burst_times[i];
                    burst_times[i] = burst_times[j];
                    burst_times[j] = a;
                }
            }
        }
//Get waiting time
wa[0] = 0; //First process have zero waiting time
printf("Waiting Times\n");
 for (i = 0; i < n; i++){
          wa[i] = burst_times[i-1] + wa[i-1];
          printf("%d\n", wa[0]);
          awt += wa[i];
    printf("Process %d:",i+1);
    printf("%d\n", wa[i]);
 }
printf("Average Waiting time:\n");
printf("%f", awt/n);


//Get turnaround time
printf("\n\nTurnaround times:\n");
for(i=0; i<n; i++)
{
    tat[i] = wa[i] + burst_times[i];
    att += tat[i];
    printf("Process %d:",i+1);
    printf("%d\n", tat[i]);
   }
printf("Average Turnaround time:\n");
printf("%f", att/n);



}