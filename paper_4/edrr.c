#include "../test_related/algorithm_testing.h"

#define TRUE 1
#define FALSE 0

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define NEW 0
#define READY 1
#define WAITING 2
#define TERMINATED 3

typedef struct edrr_process {
    int pid;
    int burst_time;
    int cpu_time;
    int waiting_time;
    int arrival_time;
    int turnaround_time;
    int priority;
    int queue;
    struct edrr_process *next;
} EDRRProcess;

EDRRProcess *copy_list(Process *head) {
    // Check if head is NULL
    if (!head) {
        return NULL;
    }

    EDRRProcess *new_process = (EDRRProcess *)calloc(1, sizeof(EDRRProcess));

    new_process->pid = head->pid;
    new_process->burst_time = head->burst_time;
    new_process->cpu_time = head->cpu_time;
    new_process->waiting_time = head->waiting_time;
    new_process->arrival_time = head->arrival_time;
    new_process->turnaround_time = head->turnaround_time;
    new_process->priority = head->priority;
    new_process->queue = NEW;

    new_process->next = copy_list(head->next);

    return new_process;
}

// From https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
/* function to swap data of two nodes a and b*/
void swap(EDRRProcess *a, EDRRProcess *b) {
    int temp_pid = a->pid;
    int temp_burst_time = a->burst_time;
    int temp_cpu_time = a->cpu_time;
    int temp_waiting_time = a->waiting_time;
    int temp_arrival_time = a->arrival_time;
    int temp_turnaround_time = a->turnaround_time;
    int temp_priority = a->priority;
    int temp_queue = a->queue;

    a->pid = b->pid;
    a->burst_time = b->burst_time;
    a->cpu_time = b->cpu_time;
    a->waiting_time = b->waiting_time;
    a->arrival_time = b->arrival_time;
    a->turnaround_time = b->turnaround_time;
    a->priority = b->priority;
    a->queue = b->queue;

    b->pid = temp_pid;
    b->burst_time = temp_burst_time;
    b->cpu_time = temp_cpu_time;
    b->waiting_time = temp_waiting_time;
    b->arrival_time = temp_arrival_time;
    b->turnaround_time = temp_turnaround_time;
    b->priority = temp_priority;
    b->queue = temp_queue;
}

// From https://www.javatpoint.com/program-to-sort-the-elements-of-the-singly-linked-list
void sort_processes_based_on_arrival_time(EDRRProcess **list) {
    EDRRProcess *current = *list, *next = NULL;

    if (!current) {
        return;
    }

    while (current) {
        // Node next will point to node next to current
        next = current->next;
        while (next) {
            // If current node's data is 
            // greater than next's data,
            // swap data between them
            if (current->arrival_time > next->arrival_time) {
                swap(current, next);
            }
            next = next->next;
        }
        current = current->next;
    }
}

// From https://www.javatpoint.com/program-to-sort-the-elements-of-the-singly-linked-list
void sort_processes_based_on_pid(EDRRProcess **list) {
    EDRRProcess *current = *list, *next = NULL;

    if (!current) {
        return;
    }

    while (current) {
        // Node next will point to node next to current
        next = current->next;
        while (next) {
            // If current node's data is 
            // greater than next's data,
            // swap data between them
            if (current->pid > next->pid) {
                swap(current, next);
            }
            next = next->next;
        }
        current = current->next;
    }
}

int there_are_processes_ready(EDRRProcess *process) {
    if (!process) return FALSE;

    while (process) {
        if (process->queue == READY) {
            return TRUE;
        }
        process = process->next;
    }

    return FALSE;
}

int there_are_processes_waiting(EDRRProcess *process) {
    if (!process) return FALSE;

    while (process) {
        if (process->queue == WAITING) {
            return TRUE;
        }
        process = process->next;
    }

    return FALSE;
}

int there_are_new_processes(EDRRProcess *process) {
    if (!process) return FALSE;

    while (process) {
        if (process->queue == NEW) {
            return TRUE;
        }
        process = process->next;
    }

    return FALSE;
}

int get_maximum_burst_time(EDRRProcess *process) {
    int max_burst_time = 0;

    if (!process) return max_burst_time;

    while (process) {
        if (process->burst_time > max_burst_time && 
            process->queue != TERMINATED && process->queue != NEW) {
            max_burst_time = process->burst_time;
        }
        process = process->next;
    }

    return max_burst_time;
}

int add_to_ready_queue(EDRRProcess **process, int time_elapsed) {
    int added_to_ready_queue = FALSE;
    
    if (!(*process)) return added_to_ready_queue;

    EDRRProcess *processes_ptr = *process;
    
    while(*process) {
        if ((*process)->arrival_time <= time_elapsed &&
            (*process)->queue == NEW) {
            added_to_ready_queue = TRUE;
            (*process)->queue = READY;
        }
        *process = (*process)->next;
    }

    *process = processes_ptr;
    
    return added_to_ready_queue;
}

int main(int argc, char const *argv[]) {
    Processes *processes = NULL;
    EDRRProcess *edrr_processes = NULL;
    EDRRProcess *edrr_processes_head = NULL;
    int time_elapsed = 0;
    int added_to_ready_queue = FALSE;
    int maximum_burst_time = 0;
    float time_quantum = 0.0;

    char *filename = NULL;

    if (argc == 2) {
        filename = (char *)argv[1];
    }

    printf("\n");

    if (!filename) {
        // No filename given
        // Give users the option of either 
        // entering file containing process params
        // or enter these params directly to program
        int user_option = 0;
        do {
            printf("\tDo you wish to:\n");
            printf("\n\t\t1. Enter path to file containing process parameters\n");
            printf("\t\t2. Enter process parameters directly into program\n");

            printf("\n\tEnter your option: ");

            user_option = getchar();
            if (user_option != '1' && user_option != '2') {
                printf("\n\t[-] Please enter 1 or 2.\n\n");
            }
            if (user_option != '\n') {
                getchar();
            }
        } while (user_option != '1' && user_option != '2');
        
        printf("\n\t[+] You have selected '%c'.\n", user_option);

        if (user_option == '1') {
            // read in filename
        } else {
            // read in params
        }
        return 0;
    }

    if (filename) {
        int get_process_status = get_processes(filename, &processes);

        if (get_process_status == MEM_ALLOC_FAILED) {
            printf("[-] get_process_status: Unable to allocate memory for reading file.\n");
            return EXIT_FAILURE;
        }

        if (get_process_status == FILE_READ_FAILED) {
            printf("[-] get_process_status: Unable to read %s.\n", filename);
            return EXIT_FAILURE;
        }

        if (processes->size == 0) {
            printf("[-] main: There are no processes to be scheduled.\n");
            return EXIT_FAILURE;
        }

        edrr_processes = copy_list(processes->head);
    }

    sort_processes_based_on_arrival_time(&edrr_processes);
    edrr_processes_head = edrr_processes;

    // Loop till end of list
    while (edrr_processes) {
        added_to_ready_queue = add_to_ready_queue(&edrr_processes, time_elapsed);
        
        if (added_to_ready_queue) {
            maximum_burst_time = get_maximum_burst_time(edrr_processes_head);
            time_quantum = 0.8 * maximum_burst_time;
            edrr_processes = edrr_processes_head;
            continue;
        }

        if (edrr_processes->queue == READY) {
            if (edrr_processes->burst_time <= time_quantum) {
                edrr_processes->turnaround_time = time_elapsed + edrr_processes->burst_time - edrr_processes->arrival_time;
                edrr_processes->waiting_time = edrr_processes->turnaround_time - edrr_processes->burst_time;

                time_elapsed += edrr_processes->burst_time;
                
                edrr_processes->burst_time = 0;
                edrr_processes->queue = TERMINATED;
            } else {
                edrr_processes->queue = WAITING;
            }
        } else if (edrr_processes->queue == WAITING) {
            // if (!there_are_processes_ready(edrr_processes_head)) {
            if (edrr_processes->burst_time <= time_quantum) {
                edrr_processes->turnaround_time = time_elapsed + edrr_processes->burst_time - edrr_processes->arrival_time;
                edrr_processes->waiting_time = edrr_processes->turnaround_time - edrr_processes->burst_time;

                time_elapsed += edrr_processes->burst_time;
                
                edrr_processes->burst_time = 0;
                edrr_processes->queue = TERMINATED;
            } else {
                edrr_processes->queue = WAITING;
            }
            // }
        }

        edrr_processes = edrr_processes->next;
        if (!edrr_processes && there_are_processes_waiting(edrr_processes_head)) {
            edrr_processes = edrr_processes_head;
            time_quantum = maximum_burst_time;
        }
    }

    edrr_processes = edrr_processes_head;

    printf("Process\t\tBurst Time\tWaiting Time\tTurn around time\n");
    printf("-------\t\t----------\t------------\t----------------\n");

    while (edrr_processes) {
        printf("%d\t\t%d\t\t%d\t\t%d\n", edrr_processes->pid, edrr_processes->cpu_time, edrr_processes->waiting_time, edrr_processes->turnaround_time);
        edrr_processes = edrr_processes->next;
    }

    return EXIT_SUCCESS;
}