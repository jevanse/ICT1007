#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Node structure 
struct proc{
    int pid;
    int arrival;
    float burst;
    float current_burst;
    float turnaround_time;
    float waiting_time;
    int response_time;
    struct proc *next;
};

// Store front and end of node
struct queue{
    int size;
    int context_switches;
    struct proc *front, *rear;
};


// Functions used
void enQueue(struct queue *q, int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time, int response_time); // Inserts a node to the end of a queue
void deQueue(struct queue *q); // Removes a node from the front of the queue
void swap(struct proc *current, struct proc *index); // Swap the data between two nodes
void sortArrival(struct queue *q); // Sort a queue by arrival time
void sortBurst(struct queue *q); // Sort a queue by burst time
float getN(struct queue *q, int index); // return the n node in the queue
float findMedium(struct queue *q); // Return the medium burst time in the queue (median)
void splitQueue(struct queue *q, float medium, struct queue *light_queue, struct queue *heavy_queue); // Splits a single queue into two separate queues if current queue size is greater than 1
void display(struct queue *q); // Display current node
int userInput(char *message, int index);



// Create a new node
struct proc *newNode(int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time, int response_time){
    struct proc *temp = (struct proc*)malloc(sizeof(struct proc));
    temp->pid = pid;
    temp->arrival = arrival;
    temp->current_burst = current_burst;
    temp->burst = burst;
    temp->turnaround_time = turnaround_time;
    temp->waiting_time = waiting_time;
    temp->response_time = response_time;
    temp->next = NULL;
    return temp;
}

// Create empty queue
struct queue *createQueue(){
    struct queue *q = (struct queue*)malloc(sizeof(struct queue));
    q->front = q->rear = NULL;
    return q;
}




int main(){
    int proc, first = 0;
    float total_turnaround = 0.0; // Total turn around without counting arrival time (runtime)
    float average_turnaround = 0.0; // Total turn around with arrival time
    float total_waiting_time = 0.0; // Total waiting time
    struct queue *waiting_queue = createQueue(); // Waiting queue for processes that have no arrived
    struct queue *ready_queue = createQueue(); // Ready queue for processes arrival time >= run time
    struct queue *light_queue = createQueue(); 
    struct queue *heavy_queue = createQueue();
    ready_queue->size = 0; // Size of ready queue to find medium process
    ready_queue->context_switches = 0; // Set context switch number to 0
    int context_switches = 0;
    
    proc = userInput("Enter number of process: ", 0);
    if(proc <= 0){
      printf("Terminating program...");
      exit(0);
    }
    int cp = proc; // copy of the proc variable, value will change
    int arrival;
    float burst;
    for(int i = 0; i < proc; i++){
        arrival = userInput("Enter arrival time for process ", i+1);                
        burst = userInput("Enter burst time for process ", i+1);        
        enQueue(waiting_queue, i+1, arrival, burst, burst, 0, 0, 0);
    }

    sortBurst(waiting_queue);
    sortArrival(waiting_queue);

    // If arrival time is less than or equals to run time, move process into ready queue
    while(waiting_queue->front != NULL){ // Keeps looping as long as there are processes in queue
      int status = 0;      
      for(int i = 0; i < cp; i++){
        if(waiting_queue->front->arrival <= total_turnaround){ // If processes arrival time greater than or equals to current run time
          enQueue(ready_queue, waiting_queue->front->pid, waiting_queue->front->arrival, waiting_queue->front->burst, waiting_queue->front->current_burst, waiting_queue->front->turnaround_time, waiting_queue->front->waiting_time, waiting_queue->front->response_time); // Adds the process to the end of ready queue
          ready_queue->size++; // Increment the ready queue's size by 1     
          deQueue(waiting_queue); // Removes the process from waiting queue
          first = 1;
        }else if(first == 0){ // If none of the process has arrival time 0
          waiting_queue->front->arrival = 0;
          enQueue(ready_queue, waiting_queue->front->pid, waiting_queue->front->arrival, waiting_queue->front->burst, waiting_queue->front->current_burst, waiting_queue->front->turnaround_time, waiting_queue->front->waiting_time, waiting_queue->front->response_time); // Adds the process to the end of ready queue
          ready_queue->size++; // Increment the ready queue's size by 1     
          deQueue(waiting_queue); // Removes the process from waiting queue
          first = 1;
        }else{
          status = 1;
          first = 0; // There is no more processes that have arrived, break out of the loop
          break;    
        }
      }      
      if(status == 1 || ready_queue->front != NULL){
        float mbt;                
        sortBurst(ready_queue); // Sort ready queue by burst time in ascending order        
        mbt = findMedium(ready_queue); // Get the medium burst time in ready queue        
        splitQueue(ready_queue, mbt, light_queue, heavy_queue); // If ready queue size is larger than 1, split ready queue into light and heavy queue
        mbt = findMedium(light_queue); // Get the medium burst time in light queue, this will be used as the time quantum
        while(light_queue->front != NULL){ // Keep looping till processes in light queue are completed
          if(light_queue->front->current_burst <= mbt){  // If the remaining burst time of the process is less than the time quantum
            if(light_queue->front->response_time == 0){
              light_queue->front->response_time = total_turnaround - light_queue->front->arrival;
            }
            light_queue->front->turnaround_time = total_turnaround;
            light_queue->front->turnaround_time += light_queue->front->current_burst; // Add the remaining time into the turnaround time for the process
            total_turnaround = light_queue->front->turnaround_time; // Add the turnaround time for the process into the total turnaround time 
            light_queue->front->turnaround_time = total_turnaround; // Update the turnaround time to reflect the total turn around time taken till the completion of the task
            average_turnaround += total_turnaround - light_queue->front->arrival; // Adds the turnaround time of the process subtracted by the arrival time 
            light_queue->front->current_burst = 0; // Set the remaining burst time of the process to 0
            light_queue->front->waiting_time = total_turnaround - light_queue->front->burst - light_queue->front->arrival; // Calculates the waiting time for the process
            total_waiting_time += light_queue->front->waiting_time; // Adds the waiting time for the process into total waiting time
            ready_queue->size--; // Decrements the ready queue size by 1
            light_queue->size--; // Decrements the light queue size by 1
            cp--; // Decrements cp which is the remaining processes in waiting queue   
            ready_queue->context_switches++;
            // deQueue(light_queue); // Removes the current process from the front of the queue 
          }else{
            if(light_queue->front->response_time == 0){
              light_queue->front->response_time = total_turnaround - light_queue->front->arrival;
            }
            light_queue->front->turnaround_time = total_turnaround;
            light_queue->front->turnaround_time += mbt; // Adds the time quantum into turnaround time 
            light_queue->front->waiting_time = total_turnaround - light_queue->front->arrival;     
            light_queue->front->current_burst -= mbt; // Subtract current burst time by time quantum
            total_turnaround = light_queue->front->turnaround_time;
            enQueue(light_queue, light_queue->front->pid, light_queue->front->arrival,  light_queue->front->burst, light_queue->front->current_burst, light_queue->front->turnaround_time, light_queue->front->waiting_time, light_queue->front->response_time); // Adds the process to the back of the queue   
            ready_queue->context_switches++;         
          }          
          display(light_queue);             
          deQueue(light_queue); // Removes the current process from the front of the queue
          context_switches = ready_queue->context_switches;  
        }
        mbt = findMedium(heavy_queue);        
        while(heavy_queue->front != NULL){ // Keep looping till processes in heavy queue are completed
          if(heavy_queue->front->current_burst <= mbt){
            if(heavy_queue->front->response_time == 0){
              heavy_queue->front->response_time = total_turnaround - heavy_queue->front->arrival;
            }          
            heavy_queue->front->turnaround_time = total_turnaround;
            heavy_queue->front->turnaround_time += heavy_queue->front->current_burst; // Adds remaining burst time of process to its turnaround time
            total_turnaround = heavy_queue->front->turnaround_time; // Adds process turnaround time to totla turnaround time
            heavy_queue->front->turnaround_time = total_turnaround; // Update process turnaround time
            average_turnaround += total_turnaround - heavy_queue->front->arrival; // Adds total turnaround time subtracted by process arrival time into average turnaround time
            heavy_queue->front->current_burst = 0; // Set remaining burst time to 0
            heavy_queue->front->waiting_time = total_turnaround - heavy_queue->front->burst - heavy_queue->front->arrival; // Calculates waiting time for the process
            total_waiting_time += heavy_queue->front->waiting_time; // Adds process waiting time to total waiting time
            ready_queue->size--; // Decrements ready queue size
            heavy_queue->size--; // Decrements heavy queue size
            cp--; // Decrements cp (remaining processes in waiting queue)      
            ready_queue->context_switches++;  
            // deQueue(heavy_queue); // Removes the process from the front of heavy queue                
          }else{
            if(heavy_queue->front->response_time == 0){
              heavy_queue->front->response_time = total_turnaround - heavy_queue->front->arrival;
            }
            heavy_queue->front->turnaround_time = total_turnaround;
            heavy_queue->front->turnaround_time += mbt; // Adds time quantum to process turnaround time
            heavy_queue->front->waiting_time = total_turnaround - heavy_queue->front->arrival;
            heavy_queue->front->current_burst -= mbt; // Subtract time quantum from process remaining burst time
            total_turnaround = heavy_queue->front->turnaround_time;
            enQueue(heavy_queue, heavy_queue->front->pid, heavy_queue->front->arrival, heavy_queue->front->burst, heavy_queue->front->current_burst, heavy_queue->front->turnaround_time, heavy_queue->front->waiting_time, heavy_queue->front->response_time); // Adds the process to the end of heavy queue      
            ready_queue->context_switches++;      
          }          
          display(heavy_queue); 
          deQueue(heavy_queue); // Removes the process from the front of heavy queue      
          context_switches = ready_queue->context_switches;      
        }        
        
      }      
    }
    if(context_switches > 0){
      context_switches--;
    }    
    printf("Average Turnaround: %.2f\n", average_turnaround / proc);
    printf("Average Waiting time: %.2f\n", total_waiting_time / proc);
    printf("Context switches: %d", context_switches);
    
}


// Add node to queue
void enQueue(struct queue *q, int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time, int response_time){
    // Create a new linked list node
    struct proc *temp = newNode(pid, arrival, burst, current_burst, turnaround_time, waiting_time, response_time);    

    // If queue is empty, then new node is front and rear
    if(q->rear == NULL){
        q->front = q->rear = temp;        
        return;
    }

    // Add the new node at the end of queue
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove from queue
void deQueue(struct queue *q){

    // If queue is empty, return NULL
    if(q->front == NULL){
        return;
    }
    struct proc *temp = q->front;

    // Store previous front and move front node ahead
    q->front = q->front->next;

    if(q->front == NULL){
        q->rear = NULL;
    }

    free(temp);
}


// Swap data between two nodes
void swap(struct proc *current, struct proc *index){
  int temp_arrival, temp_pid;
  float temp_burst, temp_current;
  // Swap pid
  temp_pid = current->pid;
  current->pid = index->pid;
  index->pid = temp_pid;
  // Swap arrival
  temp_arrival = current->arrival;
  current->arrival = index->arrival;
  index->arrival = temp_arrival;
  // Swap burst time
  temp_burst = current->burst;
  current->burst = index->burst;
  index->burst = temp_burst;
  // Swap current burst
  temp_current = current->current_burst;
  current->current_burst = index->current_burst;
  index->current_burst = temp_current;
}

// Sort by arrival time in ascending order
void sortArrival(struct queue *q){
  struct proc *current = q->front, *index = NULL;

  if(q == NULL){
    return;
  }else{
    while(current != NULL){
      // Points to next node of current
      index = current->next;

      while(index != NULL){
        if(current->arrival > index->arrival){
          swap(current, index);
        }
        index = index->next;
      }
      current = current->next;
    }
  } 
}

void sortBurst(struct queue *q){
  struct proc *current = q->front, *index = NULL;

  if(q == NULL){
    return;
  }else{
    while(current != NULL){
      // Points to next node of current
      index = current->next;

      while(index != NULL){
        if(current->burst > index->burst){
          swap(current, index);
        }
        index = index->next;
      }
      current = current->next;
    }
  } 
}


// Return the Nth node in queue
float getN(struct queue *q, int index){
  struct proc *current = q->front;
  int count = 0;
  while(current != NULL){
    if(count == index-1){
      return current->burst;
    }
    count++;
    current = current->next;
  }
  return 0;
}


// Return the median process burst time in given queue
float findMedium(struct queue *q){
  struct queue *ready_queue = q;
  float num1, num2;
  float mbt = 0.0;
  if(ready_queue->size % 2 == 0){ // Even number; take average of two middle numbers    
    num1 = (ready_queue->size / 2);    
    num2 = (ready_queue->size / 2) + 1;    
    num1 = getN(ready_queue, num1);
    num2 = getN(ready_queue, num2);
    mbt = (num1 + num2) / 2;
  }else{ // Odd number; take the middle number    
    num1 = (ready_queue->size / 2) + 1;          
    num1 = getN(ready_queue, num1);          
    mbt = num1;
  }
  return mbt;
}

// Splits the ready queue into two separate queue (light, heavy)
void splitQueue(struct queue *q, float medium, struct queue *light_queue, struct queue *heavy_queue){
  while(q->front != NULL){
    if(q->front->burst <= medium){
      enQueue(light_queue, q->front->pid, q->front->arrival, q->front->burst, q->front->current_burst, q->front->turnaround_time, q->front->waiting_time, q->front->response_time);
      light_queue->size++; 
      deQueue(q);
    }else{
      enQueue(heavy_queue, q->front->pid, q->front->arrival, q->front->burst, q->front->current_burst, q->front->turnaround_time, q->front->waiting_time, q->front->response_time);
      heavy_queue->size++;
      deQueue(q);
    }    
  }  
}

void display(struct queue *q){ // Display all items in the node (debugging)
  struct proc *temp = q->front;
  
  printf("PID: %d\t Burst time: %.2f\t Current Burst time: %.2f\t Arrival time: %d\t Turnaround time: %.2f\t Waiting time: %.2f\t Response time: %d\n", temp->pid, temp->burst, temp->current_burst, temp->arrival, temp->turnaround_time, temp->waiting_time, temp->response_time);
    
}

int userInput(char *message, int index){ //Input validation to receive only integers and number of processes cannot be 0
  long a;
  char buf[1024]; // use 1KB just to be sure
  int success; // flag for successful conversion

  do
  {   
      if(index == 0){
        printf("%s",message);
      }else{
        printf("%s%d: ", message, index);
      }
      
      if (!fgets(buf, 1024, stdin))
      {
          // reading input failed:
          return 1;
      }

      // have some input, convert it to integer:
      char *endptr;

      errno = 0; // reset error number
      a = strtol(buf, &endptr, 10);
      if (errno == ERANGE)
      {
          printf("Sorry, this number is too small or too large.\n");
          success = 0;
      }
      else if (endptr == buf)
      {
          // no character was read          
          success = 0;
      }
      else if (*endptr && *endptr != '\n')
      {
          // *endptr is neither end of string nor newline,
          // so we didn't convert the *whole* input          
          success = 0;
      }
      else
      {
          success = 1;
      }
  } while (!success); // repeat until we got a valid number

  return a;
}
