#include "algorithm_testing.h"

int main()
{
    printf("** Start of tests **\n\n");
    test_insertion();
    printf("** End of tests **\n");
}

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