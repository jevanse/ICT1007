#include "algorithm_testing.h"

// int main()
// {
//     printf("** Start of tests **\n\n");
//     test_insertion();
//     test_read();
//     printf("** End of tests **\n");
// }

int init_processes(Processes *processes)
{
    processes->head = NULL;
    processes->tail = NULL;
    processes->size = 0;
    return 0;
}

int insert_node(Processes *processes, Process *process)
{
    Process *current = (Process*) malloc(sizeof(Process));

    processes->size += 1;
    if (processes->head == NULL)
    {
        //If the linked list is not yet populated, populate it with the first element
        processes->head = process;
        processes->tail = process; //only on first insert will the tail be same as the head
    }
    else
    {
        processes->tail->next = process; //make insertion O(1)
        processes->tail = process; //update tail to reflect new tail
    }
    return 0;
}

int test_insertion()
{
    /* Test function */
    Processes * processes = (Processes *) malloc(sizeof(Processes));

    init_processes(processes);
    
    Process *process_1 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_2 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_3 = (Process*) calloc(1,(sizeof(Process)));
    Process *process_4 = (Process*) calloc(1,(sizeof(Process)));

    process_1->arrival_time = 30;
    process_2->arrival_time = 33;
    process_3->arrival_time = 36;
    process_4->arrival_time = 39;
    
    insert_node(processes, process_1);
    insert_node(processes, process_2);
    insert_node(processes, process_3);
    insert_node(processes, process_4);

    Process *current = processes->head;

    printf("\tSize of list: %d\n", processes->size);
    while (current != NULL)
    {
        printf("\tArrival time %d\n", current->arrival_time);
        current = current->next;
    }

    return 0;
}

int read_file(const char *filename, char ** file_contents) {
    // Open file for reading
    FILE* file = fopen(filename, "r");

    // Check if file opened correctly
    if (!file) {
        return FILE_READ_FAILED;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    
    // Reset file pointer to point 
    // at start of file
    fseek(file, 0, SEEK_SET);

    char *contents_read = (char *)calloc(file_size + 1, sizeof(char));

    if (!contents_read) {
        return MEM_ALLOC_FAILED;
    }

    fread(contents_read, file_size, 1, file);

    // Close the file
    fclose(file);

    *file_contents = (char *)calloc(file_size + 1, sizeof(char));
    strcpy(*file_contents, contents_read);

    return EXIT_SUCCESS;
}

int parse_file_contents(const char *file_contents, Processes **processes) {
    char *line = strtok((char *)file_contents, "\n");

    *processes = (Processes *)calloc(1, sizeof(Processes));
    init_processes(*processes);

    while (line != NULL) {
        int pid_read;
        int burst_time_read;
        int arrival_time_read;
        int priority_read;

        sscanf(line, "%d,%d,%d,%d", &pid_read, &burst_time_read, &arrival_time_read, &priority_read);

        Process *process = (Process *)calloc(1, sizeof(Process));
        if (!process) {
            return MEM_ALLOC_FAILED;
        }

        process->pid = pid_read;
        process->burst_time = burst_time_read;
        process->cpu_time = burst_time_read;
        process->arrival_time = arrival_time_read;
        process->priority = priority_read;

        insert_node(*processes, process);
        
        line = strtok(NULL, "\n");
    }
}

int get_processes(const char *filename, Processes **processes) {
    char *file_contents = NULL;
    int read_file_status = read_file(filename, &file_contents);

    if (read_file_status != EXIT_SUCCESS) {
        return read_file_status;
    }

    return parse_file_contents(file_contents, processes);
}

void test_read() {
    Processes *processes = NULL;
    get_processes("./processes.txt", &processes);
    Process *current = processes->head;

    printf("\tSize of list: %d\n", processes->size);
    while (current != NULL)
    {
        printf("\tBurst time %d\n", current->burst_time);
        printf("\tArrival time %d\n", current->arrival_time);
        current = current->next;
    }
}