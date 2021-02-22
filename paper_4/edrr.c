/**
 * EDRR = Efficient Dynamic Round Robin
 * 
 * Based on Paper: An Efficient Dynamic Round Robin Algorithm for CPU Scheduling
 * 
 * Authors of the paper:
 *      - Muhammad Umar Farooq
 *      - Aamna Shakoor
 *      - Abu Bakar Siddique
 * The authors are from the 
 *      University of Engineering and Technology, Lahore
 * 
 * Source of paper: https://ieeexplore.ieee.org/document/7918936
 */

#include "../test_related/algorithm_testing.h"
#include <ctype.h>

#define TRUE 1
#define FALSE 0

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

/**
 * The queue state of processes
 */
#define NEW 0
#define READY 1
#define WAITING 2
#define TERMINATED 3

#define MAX_FILE_PATH 4096
#define MAX_NUM_BUF 5

typedef struct edrr_process {
    int pid;
    int burst_time;
    int cpu_time;
    int waiting_time;
    int arrival_time;
    int turnaround_time;
    int response_time;
    int priority;
    int queue;
    struct edrr_process *next;
} EDRRProcess;

// Process (Process and EDRRProcess) list related

// From https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/?ref=lbp
/**
 * Adds an EDRRProcess to a linked list of EDRRProcess-es
 * 
 * **list: Pointer to the list pointer that points to a list of EDRRProcess
 * *process: Pointer to EDRRProcess node
 */
void add_process(EDRRProcess **list, EDRRProcess *process) {
    if (!*list) {
        // If list is empty,
        // initialise with process
        //
        // (i.e.) process will be the first node in list
        *list = process;
    } else {
        EDRRProcess *current = *list;
        
        // Traverse till end of list
        while (current->next) {
            current = current->next;
        }

        // Add process to end of list
        current->next = process;
    }
}

void update_process_value(int pid, int turnaround_time, int waiting_time, int response_time, Process **processes) {
    if (!processes) return;
    
    Process *head = *processes;

    while(*processes && (*processes)->pid != pid) {
        (*processes) = (*processes)->next;
    }

    (*processes)->turnaround_time = turnaround_time;
    (*processes)->waiting_time = waiting_time;
    (*processes)->response_time = response_time;

    *processes = head;
}

/**
 * Copies back updated values from local list of processes to global one
 * 
 * - i.e. convert EDRRProcess* to Process*
 */
void copy_list_back(Process **head, EDRRProcess *edrr_head) {
    if (!head || !edrr_head) return;

    while(edrr_head) {
        update_process_value(
            edrr_head->pid, 
            edrr_head->turnaround_time, 
            edrr_head->waiting_time, 
            edrr_head->response_time, 
            head
        );
        edrr_head = edrr_head->next;
    }
}

// From https://www.geeksforgeeks.org/c-program-to-create-copy-of-a-singly-linked-list-using-recursion/#:~:text=Allocate%20the%20new%20Node%20in,and%20the%20duplicate%20linked%20list.
/**
 * Creates a local copy of the list of processes read from file
 * 
 * - The local copy will contain more attributes that are only needed for the local logic
 * - i.e. convert Process* to EDRRProcess*
 */
EDRRProcess *copy_list(Process *head) {
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
    new_process->response_time = head->response_time;
    new_process->priority = head->priority;

    new_process->next = copy_list(head->next);

    return new_process;
}

// From https://www.geeksforgeeks.org/c-program-bubble-sort-linked-list/
/* function to swap data of two nodes a and b*/
/**
 * Swap two processes in a list (for sorting)
 */
void swap(EDRRProcess *a, EDRRProcess *b) {
    EDRRProcess a_saved = *a;
    EDRRProcess *b_next_saved = b->next;

    // Swap the pointers such that 
    //      before_a -> a -> b -> after_b
    // becomes
    //      before_a -> b -> a -> after_b
    *a = *b;
    *b = a_saved;
    b->next = b_next_saved;
    a->next = b;
}

// From https://www.javatpoint.com/program-to-sort-the-elements-of-the-singly-linked-list
void sort_processes_based_on_arrival_time(EDRRProcess **list) {
    EDRRProcess *current = *list, *next = NULL;

    if (!current) {
        return;
    }

    while (current) {
        // "next" will keep track of the next process to the current
        next = current->next;
        while (next) {
            // If current's arrival_time is 
            // greater than next's, swap
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
        // Next will keep track of the next process to the current
        next = current->next;
        while (next) {
            // If current's pid is 
            // greater than next's, swap
            if (current->pid > next->pid) {
                swap(current, next);
            }
            next = next->next;
        }
        current = current->next;
    }
}

/**
 * Frees the memory used to store list of processes 
 * created.
 * 
 * *head: list pointer pointing to the processes in memory
 */
void free_process_list(Process *head) {
    Process *prev = head;
    Process *current = head;
    while (current) {
        prev = current;
        current = prev->next;
        free(prev);
    }
}

// Validation related

/**
 * Checks whether a given string contains only 
 * numbers -- the string is safe to be converted 
 * into a number in the future using calls to atoi() 
 * or similar.
 * 
 * *str: character pointer to the string that is to be checked
 */
int is_number(const char *str) {
    if (!str) {
        return FALSE;
    }

    if (strlen(str) == 0) {
        return FALSE;
    }

    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i]) == 0) {
            return FALSE;
        }
    }
    
    return TRUE;
}

// EDRR Implementation related

/**
 * Gets the minimum arrival time of all new processes in a list
 */
int get_minimum_arrival_time(EDRRProcess * process) {
    int minimum_arrival_time = -1;

    if (!process) return minimum_arrival_time;

    // Set minimum_arrival_time to the arrival_time of the first new process in list
    while (process) {
        if (process->queue == NEW) {
            minimum_arrival_time = process->arrival_time;
            break;
        }
        process = process->next;
    }

    // Loop through the processes and check their arrival_time 
    // against minimum_arrival_time.
    // Update minimum_arrival_time if necessary.
    while (process) {
        if (process->arrival_time < minimum_arrival_time && 
            process->queue == NEW) {
            minimum_arrival_time = process->arrival_time;
        }
        process = process->next;
    }

    return minimum_arrival_time;
}

/**
 * Gets the maximum burst time of all ready and waiting processes
 */
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

/**
 * Receives a list of processes and "add"s all the new 
 * processes that has arrived 
 *  (by checking its arrival_time against time_elapsed)
 * into ready list
 * 
 * Returns a boolean (1 or 0) indicating if any new processes 
 * were added to ready list of processes
 * 
 * When we say "add", it just means to toggle the queue flag of 
 * processes from NEW to READY
 */
int add_to_ready_queue(EDRRProcess **process, int time_elapsed) {
    int added_to_ready_queue = FALSE;
    
    if (!(*process)) return added_to_ready_queue;

    EDRRProcess *processes_ptr = *process;
    
    // Loop through processes
    while(*process) {
        // if a new process has arrived,
        // "add" it to ready queue
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

/**
 * Returns a boolean indicating if there are processes 
 * in the ready queue
 */
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

/**
 * Returns a boolean indicating if there are processes 
 * in the waiting queue
 */
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

/**
 * Returns a boolean indicating if there are 
 * new processes
 */
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

int main(int argc, char const *argv[]) {
    Processes *processes = NULL;
    EDRRProcess *edrr_processes = NULL;
    EDRRProcess *edrr_processes_head = NULL;
    int time_elapsed = 0;
    int added_to_ready_queue = FALSE;
    int maximum_burst_time = 0;
    float time_quantum = 0.0;

    char *filename = NULL;

    // Filename can be entered as an argument
    if (argc == 2) {
        filename = (char *)argv[1];
    }

    printf("\n");

    if (!filename) {
        // No filename given
        // Give users the option of either 
        // 1. entering file containing process params
        // 2. enter these params directly to program
        // 3. exit
        int user_option = 0;
        do {
            printf("\tDo you wish to:\n\n");
            printf("\t\t1. Enter path to file containing process parameters\n");
            printf("\t\t2. Enter process parameters directly into program\n");
            printf("\t\t3. Exit program\n");

            printf("\n\tEnter your option: ");

            user_option = getchar();
            if (user_option != '1' && user_option != '2' && user_option != '3') {
                printf("\n\t[-] Please enter 1, 2 or 3.\n\n");
            }
            if (user_option != '\n') {
                getchar();
            }
        } while (user_option != '1' && user_option != '2' && user_option != '3');
        
        printf("\n\t[+] You have selected '%c'.\n", user_option);
        
        if (user_option == '1') {

            filename = (char *)calloc(1, MAX_FILE_PATH + 2);
            do {
                printf("\n\tEnter path to file (press 'q' to exit): ");
                fgets(filename, MAX_FILE_PATH, stdin);

                if (filename[strlen(filename) - 1] == '\n') {
                    filename[strlen(filename) - 1] = '\0';
                }

                if (strncmp(filename, "q", 1) == 0) {
                    printf("\n\t[i] User pressed 'q'. Exiting program...\n\n");
                    return 0;
                }
                if (strlen(filename) == 0)
                    printf("\n\t[-] Please enter a valid file path.\n");
            } while(strlen(filename) == 0);

            printf("\n\t[+] You have entered '%s'", filename);

        } else if (user_option == '2') {

            // read in params
            int num_of_processes = -1;
            int burst_time_entered = -1, 
                arrival_time_entered = -1;
            do {
                printf("\n\tEnter the number of processes to be scheduled ('0' to exit): ");
                scanf("%d", &num_of_processes);
                if (num_of_processes == 0) {
                    printf("\n\t[i] User entered '0'. Exiting program...\n\n");
                    return 0;
                }
                if (num_of_processes == -1) {
                    printf("\n\tPlease enter a number.\n");
                }
            } while (num_of_processes < 0);
            printf("\n\tNumber of processes: %d", num_of_processes);
            printf("\n");
            processes = (Processes *)calloc(1, sizeof(Processes));
            for (int i = 0; i < num_of_processes; i++) {
                printf("\n\tEnter the burst time of Process %d: ", i+1);
                scanf("%d", &burst_time_entered);
                printf("\tEnter the arrival time of Process %d: ", i+1);
                scanf("%d", &arrival_time_entered);
                
                Process *process = (Process *)calloc(1, sizeof(Process));
                if (!process) {
                    printf("\n\t[-] main: Unable to allocate memory");
                    return MEM_ALLOC_FAILED;
                }
                process->pid = i+1;
                process->burst_time = burst_time_entered;
                process->cpu_time = burst_time_entered;
                process->arrival_time = arrival_time_entered;
                insert_node(processes, process);

                EDRRProcess *edrr_process = (EDRRProcess *)calloc(1, sizeof(EDRRProcess));
                if (!edrr_process) {
                    printf("\n\t[-] main: Unable to allocate memory");
                    return MEM_ALLOC_FAILED;
                }
                edrr_process->pid = i+1;
                edrr_process->burst_time = burst_time_entered;
                edrr_process->cpu_time = burst_time_entered;
                edrr_process->arrival_time = arrival_time_entered;
                add_process(&edrr_processes, edrr_process);
            }

        } else {
            if (user_option == '3') {
                printf("\n\t[i] User pressed '3'. Exiting program...\n\n");
            } else {
                printf("\n\t[-] User pressed unknown option ''. Exiting program...\n\n", user_option);
            }
            return 0;
        }
    }

    if (filename) {
        int get_process_status = get_processes(filename, &processes);

        if (get_process_status == MEM_ALLOC_FAILED) {
            printf("\n\t[-] get_process_status: Unable to allocate memory for reading file.\n\n");
            return EXIT_FAILURE;
        }

        if (get_process_status == FILE_READ_FAILED) {
            printf("\n\t[-] get_process_status: Unable to read %s.\n\n", filename);
            return EXIT_FAILURE;
        }

        if (processes->size == 0) {
            printf("\n\t[-] main: There are no processes to be scheduled.\n\n");
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

        if (edrr_processes->queue == READY || edrr_processes->queue == WAITING) {
            if (edrr_processes->burst_time <= time_quantum) {
                edrr_processes->response_time = time_elapsed - edrr_processes->arrival_time;
                edrr_processes->turnaround_time = time_elapsed + edrr_processes->burst_time - edrr_processes->arrival_time;
                edrr_processes->waiting_time = edrr_processes->turnaround_time - edrr_processes->burst_time;

                time_elapsed += edrr_processes->burst_time;
                
                edrr_processes->burst_time = 0;
                edrr_processes->queue = TERMINATED;
            } else {
                edrr_processes->queue = WAITING;
            }
        }

        edrr_processes = edrr_processes->next;
        if (!edrr_processes && !there_are_processes_waiting(edrr_processes_head) && 
            !there_are_processes_ready(edrr_processes_head) && 
            there_are_new_processes(edrr_processes_head)) {
            
            time_elapsed = get_minimum_arrival_time(edrr_processes_head);
            edrr_processes = edrr_processes_head;
            continue;
        }
        if (!edrr_processes && there_are_processes_waiting(edrr_processes_head)) {
            edrr_processes = edrr_processes_head;
            time_quantum = maximum_burst_time;
        }
    }

    edrr_processes = edrr_processes_head;

    sort_processes_based_on_pid(&edrr_processes_head);

    printf("\n\tProcess Pid\tArrival Time\tBurst Time\tWaiting Time\tTurn around time\tResponse time\n");
    printf("\t-----------\t------------\t----------\t------------\t----------------\t-------------\n");

    while (edrr_processes) {
        printf("\t%-11d\t%-12d\t%-10d\t%-12d\t%-16d\t%-13d\n", edrr_processes->pid, edrr_processes->arrival_time, edrr_processes->cpu_time, edrr_processes->waiting_time, edrr_processes->turnaround_time, edrr_processes->response_time);
        edrr_processes = edrr_processes->next;
    }

    printf("\n");

    edrr_processes = edrr_processes_head;
    Process *process = processes->head;

    copy_list_back(&process, edrr_processes);

    printf("\n\tProcess Pid\tArrival Time\tBurst Time\tWaiting Time\tTurn around time\tResponse time\n");
    printf("\t-----------\t------------\t----------\t------------\t----------------\t-------------\n");

    while (process) {
        // Original format string: "\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n"
        printf("\t%-11d\t%-12d\t%-10d\t%-12d\t%-16d\t%-13d\n", process->pid, process->arrival_time, process->cpu_time, process->waiting_time, process->turnaround_time, process->response_time);
        process = process->next;
    }

    return EXIT_SUCCESS;
}