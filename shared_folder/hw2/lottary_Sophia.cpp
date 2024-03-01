#include <unistd.h>
#include <stdio.h>//
#include <stdlib.h>
#include <time.h>
#define TIME_SLICE 5

int total_ticket = 0;
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

process* create_process(int ticket, int length, int pid){
	
	process* result = (process *)malloc(sizeof(process));
    result->length = length;
    result->pid = pid;
    result->next = NULL;
    result->ticket = ticket;

    return result;
}

add_process_to_queue(process* new_process, queue* my_queue ){
	
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
}

print_queue(queue* my_queue){
	
	int i=1;
	process* ptr = my_queue ->first;
	while(i<=my_queue->count){
		
		printf("\nPID = %d , length = %d, ticket = %d",ptr->pid, ptr->length, ptr->ticket );
		ptr = ptr->next;
		i++;
	}	
}

queue* remove_process(queue* my_queue, process* ptr){
	
	process* prev_ptr = (process *)malloc(sizeof(process));
	printf("in remove function: process %d should be removed\n", ptr->pid);
	my_queue->count--;
	prev_ptr = my_queue->first;

	//remove the first p
	if(my_queue->first == ptr){
		my_queue->first = ptr->next;
		ptr->next = NULL;
		total_ticket = total_ticket-ptr->ticket;
		printf("process %d is delete", ptr->pid);
		
	//remove the last p	
	}else if(my_queue->last == ptr){	
		prev_ptr->next = NULL;
		my_queue->last = prev_ptr;
		total_ticket = total_ticket-ptr->ticket;
		printf("process %d is delete", ptr->pid);
		
	//remove the middle p	
	}else{
		while(prev_ptr->next->pid != ptr->pid){
			//printf("prev = hahah");
			prev_ptr = prev_ptr->next;
		}
		prev_ptr->next = ptr->next;
		ptr->next = NULL;
		total_ticket = total_ticket-ptr->ticket;
		printf("process %d is delete\n", ptr->pid);
	}
	free(prev_ptr);
	return my_queue;
}

lottery(queue* my_queue, int time_slot, process* ptr_process){
	
	srand(time(NULL));   // Initialization
	int ticket_add, selected_ticket;
	process* count_p = (process *)malloc(sizeof(process));
	count_p = my_queue->first;

	while(my_queue->count > 0){
		// decide which p should run
		ticket_add = ptr_process->ticket;
	
		// random number 
		selected_ticket = (rand()%(total_ticket-1))+1; 
		
		//int selected_ticket = 17;
		printf("\nselected ticket is %d, ", selected_ticket);
		
		// select run process & run process
		while(selected_ticket > ticket_add){
			printf("\nselected ticket = %d, ticket add = %d", selected_ticket, ticket_add);
			ptr_process = ptr_process->next;
			ticket_add = ticket_add + ptr_process->ticket;	
		}
		ptr_process = my_queue->first;
		ptr_process->length -= TIME_SLICE;
		ptr_process->ticket--;
		total_ticket--;
		printf("process %d is select to run\n", ptr_process->pid);
		
		if(ptr_process->length<=TIME_SLICE){
			my_queue = remove_process(my_queue, ptr_process);
		}
		ptr_process = my_queue->first;
		print_queue(my_queue);
		
	}
	free(count_p);
	
}


int main(){
	
	int pid=1, total_process=0;
	int i;
	queue* my_queue;
	process* new_process;
	process* selected_process;
	
	// user input how many process
	printf("input how many process\n");
	scanf("%d", &total_process);
	
	//create queue
	my_queue = create_queue();
	total_ticket = 0;
	
	//create process and add to queue
	for(i=1;i<=total_process;i++){
		
		//ticket, length, pid
		new_process = create_process((rand()%9)+1, (rand()%15)+1, pid);
		//printf("pid = %d, length = %d, ticket = %d", new_process->pid, new_process->length, new_process->ticket);
		add_process_to_queue(new_process, my_queue );
		pid++;
		total_ticket = total_ticket + new_process->ticket;
	}
	
	
	//print queue
	print_queue(my_queue);

	//run lottery
	while(my_queue->count > 0){
		printf("\ntotal ticket = %d", total_ticket);
		lottery(my_queue, 5, my_queue->first);
	}
	
	if(my_queue->count == 0){
		printf("\nfinish , queue is empty\n");
		free(my_queue);
		free(new_process);
	}

	return 0;
}


