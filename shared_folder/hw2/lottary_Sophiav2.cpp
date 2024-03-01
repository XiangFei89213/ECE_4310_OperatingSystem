#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct process {
    int pid;
    int length;
    int ticket;
	struct process * next;
} process;
typedef struct queue {
    int count;
    process * first;
    process * last;
} queue;

queue * create_queue() {

    // the memory that result point to is sizeof(queue)
    queue* result = (queue *)malloc(sizeof(queue));
    result->count = 0;
    result->first = NULL;
    result->last = NULL;

    return result;
}

void print_queue(queue* my_queue){
	
	int i=1;
	process* ptr = my_queue ->first;
	while(i<=my_queue->count){
		
		printf("\nPID = %d , length = %d, ticket = %d",ptr->pid, ptr->length, ptr->ticket );
		ptr = ptr->next;
		i++;
	}	
}

process* create_process(int ticket, int length, int pid){
	
	process* result = (process *)malloc(sizeof(process));
    result->length = length;
    result->pid = pid;
    result->next = NULL;
    result->ticket = ticket;

    return result;
}

queue* add_process_to_queue(process* new_process, queue* my_queue ){
	
	//queue is empty
	if(my_queue->count == 0){
		//printf("queue is empty\n");
		my_queue->first = new_process;
		my_queue->last = new_process;
		my_queue->count++;
	}
	
	//queue is not empty
	else if(my_queue->count != 0){
		
		my_queue->last->next = new_process;
		my_queue->last = new_process;
		my_queue->count++;
		
	}
	return my_queue;
}

int main(){
	int pid=1, total_process=0;
	int i;
	queue* my_queue;
	process* new_process;
	process* selected_process;
	
	//create queue
	my_queue = create_queue();
	new_process = create_process(6,13,1);
	add_process_to_queue(new_process, my_queue );
	print_queue(my_queue);

	free(my_queue);
	free(new_process);
	printf("hello world");

	return 0;
}