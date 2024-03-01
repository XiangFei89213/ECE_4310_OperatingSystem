// This program is a lottery scheduler in C for Seminar 2
// Carlos Zarco-ECE 4310

#include <stdlib.h> // for malloc function
#include <stdio.h>
#include <time.h> // used for random number generation (srand to seed the time)

int gtickets = 0; // global number of tickets for all processes

// Define the process structure
typedef struct _process {
    int pid;
    char name;
    int length; // how long the process will run for (how many time units)
    int tickets; // number of tickets the process has for the lottery
    struct _process* next; // each node of the list has a link to the next process in the list
} process;

// Define the list structure
typedef struct _list {
    int count; // how many elements are in the list
    process* first; // public member variable first of type process* in the structure (class) List
} list;

// Function to create and initialize a new list
list* create_list() {
    list* result = (list*)malloc(sizeof(list)); // Allocate memory for the list
    result->count = 0; // Initialize the count to 0
    result->first = NULL; // Initialize the first element to NULL
    return result;
}

// Function to create and initialize a new process
process* create_process(int pid, char name, int length) {
    process* result = (process*)malloc(sizeof(process)); // Allocate memory for the process
    result->length = length; // Set the process length
    result->name = name;
    result->pid = pid;
    result->next = NULL; // Initialize the next pointer to NULL
    return result;
}

// Function to add a new process to the end of the list
int add(list* my_list, process* new_process, int tickets) {

    ++my_list->count; // Increment the count of processes
    new_process->tickets = tickets; // Assign the number of tickets to the new process
    
    // If the list is empty, set the new process as the first one
    if (my_list->first == NULL) {
        my_list->first = new_process; 
    } else {
        process* current = my_list->first;
        while (current->next != NULL) {
            current = current->next; // Find the last process in the list
        }
        current->next = new_process; // Add the new process to the end of the list
    }

    return 0; // Success
}

// Function to print the list
void print_list(list* my_list) {
    if (my_list == NULL) {
        printf("List is NULL.\n");
        return;
    }

    if (my_list->count == 0) {
        printf("List is empty.\n");
        return;
    }

    process* current = my_list->first;
    printf("List Contents:\n");
    while (current != NULL) {
        printf("Process PID: %d, Name: %c, Length: %d, Tickets: %d\n",
            current->pid, current->name, current->length, current->tickets);
        current = current->next;
    }
}

//remove current process
void remove_process(list* my_list, process* current_process) {
    if (my_list == NULL || my_list->count == 0 || current_process == NULL) {
        printf("Error, the list is empty");
        return; 
    }

    // If the current process is the first one in the list
    if (my_list->first == current_process) {
        printf("process %d has been removed\n", current_process->pid);
        my_list->first = current_process->next;
        free(current_process);
        my_list->count--;
        
        return; 
    }

    // If the current process is not the first one, find its predecessor
    process* prev_process = my_list->first;
    while (prev_process != NULL && prev_process->next != current_process) {
        prev_process = prev_process->next;
        
    }

    // If the current process was found in the list
    if (prev_process != NULL) {
        printf("process %d has been removed\n", current_process->pid);
        prev_process->next = current_process->next;
        free(current_process);
        my_list->count--;
        return; 
    }

}

// Lottery Scheduler function
int lottery(list* my_list, int time_slice) {
    if (my_list == NULL) { //list is empty
        printf("No processes in the list to schedule.\n");
        return ; // Error, no processes in the list
    }

    // Calculate the total number of tickets
    process* current = my_list->first;
    while (current != NULL) { // there are processes
        gtickets += current->tickets;
        current = current->next;
    }

    // initial random nember 
    srand(time(NULL)); 

    // Generate a random ticket between 1 and total tickets
    int winning_ticket = (rand() % gtickets) + 1;

    // Find the winning process based on the winning ticket
    current = my_list->first;
    process* prev_process = NULL;

    while (current != NULL) {
        if (winning_ticket <= current->tickets) { // Winning process
            printf("\nProcess %c won and is selected to run (length = %d)\n", current->name, current->length);
            print_list(my_list);
            current->length -= time_slice;

            current->tickets -= 1;

            // Decrease the global ticket count by 1
            gtickets -= 1;

            if (time_slice > current->length) {
                gtickets -= current->tickets;
                current->length = 0;
                remove_process(my_list, current); // process that is currently running is REMOVED from the queue
            }

            if (current->length == 0) {
                if (prev_process == NULL) {
                    my_list->first = current->next;
                } else {
                    prev_process->next = current->next;
                }
                free(current);
                my_list->count--;
            }
            return time_slice;
        } else {
            winning_ticket -= current->tickets;
            prev_process = current;
            current = current->next;
        }
    }

    return 0;
}


int main(int argc, char** argv) {

    list* my_list = create_list();
    add(my_list, create_process(1, 'a', 10), 100);
    add(my_list, create_process(2, 'b', 7), 50);
    add(my_list, create_process(3, 'c', 3), 200);

    int total_execution_time = 0;

    while (my_list->count > 0) {

            lottery(my_list, 5);
        
        // int result = lottery(my_list, 5);
        // if (result == -1) {
        //     printf("No processes in the list to schedule.\n");
        //     break;
        // }
        // total_execution_time += result;
    }

    print_list(my_list);
    // printf("Total Execution Time: %d\n", total_execution_time);
    free(my_list);

    return 0;
}

