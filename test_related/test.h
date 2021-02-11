typedef struct process {
    int pid;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int priority;
} Process;

typedef struct process_node {
    Process process;
    Process *next;
} ProcessNode;
