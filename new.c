#include <stdio.h>
#include <stdlib.h>

// Create a linked list node
struct proc{
    int pid;
    int arrival;
    float burst;
    float current_burst;
    float turnaround_time;
    float waiting_time;
    struct proc *next;
};

// Store front and end of queue
struct queue{
    int size;
    struct proc *front, *rear;
};

void enQueue(struct queue *q, int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time);
void deQueue(struct queue *q);
void swap(struct proc *current, struct proc *index);
void sortArrival(struct queue *q);
void sortBurst(struct queue *q);
float getN(struct queue *q, int index);
float findMedium(struct queue *q);



// Create a new linked list node
struct proc *newNode(int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time){
    struct proc *temp = (struct proc*)malloc(sizeof(struct proc));
    temp->pid = pid;
    temp->arrival = arrival;
    temp->current_burst = current_burst;
    temp->burst = burst;
    temp->turnaround_time = turnaround_time;
    temp->waiting_time = waiting_time;
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
    int proc;    
    float total_turnaround = 0.0;
    float average_turnaround = 0.0;
    float total_waiting_time = 0.0;
    struct queue *waiting_queue = createQueue();
    struct queue *ready_queue = createQueue();
    struct queue *light_queue = createQueue();
    struct queue *heavy_queue = createQueue();   
    struct queue *complete = createQueue();    
    ready_queue->size = 0; // Size of ready queue to find medium process
    struct queue *temp;
    printf("Enter number of process: ");
    scanf("%d", &proc);
    int cp = proc;
    int arrival;
    float burst;
    for(int i = 0; i < proc; i++){
        printf("Enter arrival time for processes %d: ", i+1);
        scanf("%d", &arrival);
        printf("Enter burst time for processes %d: ", i+1);
        scanf("%f", &burst);
        enQueue(waiting_queue, i, arrival, burst, burst, 0, 0);
    }

    sortBurst(waiting_queue);
    sortArrival(waiting_queue);
    printf("Testing...\n");

    // If arrival time is less than or equals to run time, move process into ready queue
    while(waiting_queue->front != NULL){ // Keeps looping as long as there are processes in queue
      int status = 0;      
      for(int i = 0; i < cp; i++){
        if(waiting_queue->front->arrival <= total_turnaround){ // If processes arrival time matches current run time
          enQueue(ready_queue, waiting_queue->front->pid, waiting_queue->front->arrival, waiting_queue->front->burst, waiting_queue->front->current_burst, waiting_queue->front->turnaround_time, waiting_queue->front->waiting_time);
          ready_queue->size++;          
          deQueue(waiting_queue);
        }else{
          status = 1;          
          break;    
        }
      }      
      if(status == 1 || ready_queue->front != NULL){
        float mbt;        
        // Sort ready queue by burst time in ascending order
        sortBurst(ready_queue);
        // Get the medium burst time in ready queue
        mbt = findMedium(ready_queue);        
        printf("MBT: %.2f\n", mbt);
        // Work on processes in ready queue
        splitQueue(ready_queue, mbt, light_queue, heavy_queue);
        mbt = findMedium(light_queue);
        printf("light mbt: %.2f\n", mbt);
        while(light_queue->front != NULL){ // Keep looping till processes in light queue are completed
          if(light_queue->front->current_burst <= mbt){            
            light_queue->front->turnaround_time += light_queue->front->current_burst;
            total_turnaround += light_queue->front->turnaround_time;
            light_queue->front->turnaround_time = total_turnaround;
            average_turnaround += total_turnaround - light_queue->front->arrival;
            light_queue->front->current_burst = 0;            
            light_queue->front->waiting_time = total_turnaround - light_queue->front->burst - light_queue->front->arrival;
            total_waiting_time += light_queue->front->waiting_time;
            ready_queue->size--;
            light_queue->size--;
            cp--;           
          }else{
            light_queue->front->turnaround_time += mbt;            
            light_queue->front->current_burst -= mbt;
            enQueue(light_queue, light_queue->front->pid, light_queue->front->arrival, light_queue->front->burst, light_queue->front->current_burst, light_queue->front->turnaround_time, light_queue->front->waiting_time);
          }
          printf("Light:\n");
          display(light_queue);   
          deQueue(light_queue);
        }
        mbt = findMedium(heavy_queue);
        printf("heavy mbt: %.2f\n", mbt);
        while(heavy_queue->front != NULL){ // Keep looping till processes in heavy queue are completed
          if(heavy_queue->front->current_burst <= mbt){            
            heavy_queue->front->turnaround_time += heavy_queue->front->current_burst;
            total_turnaround += heavy_queue->front->turnaround_time;
            heavy_queue->front->turnaround_time = total_turnaround;
            average_turnaround += total_turnaround - heavy_queue->front->arrival;
            heavy_queue->front->current_burst = 0;            
            heavy_queue->front->waiting_time = total_turnaround - heavy_queue->front->burst - heavy_queue->front->arrival;            
            total_waiting_time += heavy_queue->front->waiting_time;
            ready_queue->size--;
            heavy_queue->size--;
            cp--;        
          }else{
            heavy_queue->front->turnaround_time += mbt;            
            heavy_queue->front->current_burst -= mbt;
            enQueue(heavy_queue, heavy_queue->front->pid, heavy_queue->front->arrival, heavy_queue->front->burst, heavy_queue->front->current_burst, heavy_queue->front->turnaround_time, heavy_queue->front->waiting_time);
          }
          printf("Heavy:\n");
          display(heavy_queue);     
          deQueue(heavy_queue);
        }
      }
    }    
    printf("Average Turnaround: %.2f\n", average_turnaround / proc);
    printf("Average Waiting time: %.2f\n", total_waiting_time / proc);
    
}


// Add node to queue
void enQueue(struct queue *q, int pid, int arrival, float burst, float current_burst, float turnaround_time, float waiting_time){
    // Create a new linked list node
    struct proc *temp = newNode(pid, arrival, burst, current_burst, turnaround_time, waiting_time);

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

void splitQueue(struct queue *q, float medium, struct queue *light_queue, struct queue *heavy_queue){
  while(q->front != NULL){
    if(q->front->burst <= medium){
      enQueue(light_queue, q->front->pid, q->front->arrival, q->front->burst, q->front->current_burst, q->front->turnaround_time, q->front->waiting_time);
      light_queue->size++;
      deQueue(q);
    }else{
      enQueue(heavy_queue, q->front->pid, q->front->arrival, q->front->burst, q->front->current_burst, q->front->turnaround_time, q->front->waiting_time);
      heavy_queue->size++;
      deQueue(q);
    }    
  }  
}

void display(struct queue *q){ // Display all items in the node
  struct proc *temp = q->front;
  
  printf("PID: %d\t Burst time: %.2f\t Current Burst time: %.2f\t Arrival time: %d\t Turnaround time: %.2f\t Waiting time: %.2f\n", temp->pid, temp->burst, temp->current_burst, temp->arrival, temp->turnaround_time, temp->waiting_time);
    
}