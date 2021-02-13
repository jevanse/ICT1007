typedef struct process {
    int pid;
    int burst_time;
    int cpu_time; // Same as burst_time, but this variable will not be changed even if burst_time will be changed in the future
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int priority;
    struct process *next;
} Process;
