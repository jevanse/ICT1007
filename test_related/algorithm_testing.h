#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_READ_FAILED -1
#define MEM_ALLOC_FAILED -2
#define PARSE_FILE_FAILED -3

#define TRUE 1
#define FALSE 0

typedef struct process {
    int pid;
    int burst_time;
    int cpu_time; // Same as burst_time, but this variable will not be changed even if burst_time will be changed in the future
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int priority;
    struct process *next;
} Process;

typedef struct processes {
    int size;
    Process *head;
    Process *tail;
} Processes;

int remove_spaces (char *str_trimmed, const char *str_untrimmed);
int init_processes(Processes *processes);
int insert_node(Processes *processes, Process *process);
int test_insertion();
int read_file(const char *filename, char ** file_contents);
int parse_file_contents(const char *file_contents, Processes **processes);
int get_processes(const char *filename, Processes **processes);
void test_read();