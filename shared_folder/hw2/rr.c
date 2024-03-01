#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//define a struct name _process
typedef struct _process{
    
    int pid;
    char name;
    int length;
    
    //queue link to the next process
    struct _process * next;
    
} process;
   
//define a structure name _queue
typedef struct _queue{
    
    //keep track of how many process are in the queue
    int count;
    //pointer 
    process * first;
    
    //pointer
    process * last;
    
} queue;


queue * create_queue(){
    
    //cast 強制轉換
    queue* result = (queue *)malloc(sizeof(queue));
    result->count = 0;
    result->first = NULL;
    result->last = NULL;
    
    return result;
}

process* create_process(int pid, char name, int length){
    process* result = (process *)malloc(sizeof(process));
    result->length = length;
    result->name = name;
    result->pid = pid;
    result->next = NULL;
    
    return result;
    
}

int add_to_queue(queue* my_queue, process* new_process){
    if(my_queue == NULL)
        return -1;
    
    ++my_queue->count;
    
    if(my_queue->first == NULL)
    {
        //fail
        my_queue->first = new_process;
        my_queue->last = new_process;
    } else{
        my_queue->last->next = new_process;
        my_queue->last = new_process;
    }
    
    
    
}
// always remove the first one in the list
process* remove_from_queue(queue* my_queue) {
    if(my_queue == NULL)
        return NULL;
    if(my_queue->first == NULL)
        return NULL;
    
    --my_queue->count;
    
    process* result = my_queue->first;
    
    my_queue->first = result->next;
    
    if(my_queue->count == 0)
        my_queue->last = NULL;
    
    result->next = NULL;
    return result;
    
}

// return tota; execution time
int rr(queue* my_queue, int time_slice){
    int total = 0;
    if(my_queue == NULL)
        return -1;

    while (my_queue->count >0)
    {
        process* run_process = remove_from_queue(my_queue);
        total+=1;
        
        printf("\nprocess %c is selected to run (length = %d)",run_process->name, run_process->length );
        run_process -= time_slice;

        //still has process to do
        if(run_process->length>0)
            add_to_queue(my_queue, run_process);
    }
    


}
int main(int argc, char** argv){
    queue* my_q = create_queue();
    add_to_queue(my_q, create_process(1,'a',30));
    add_to_queue(my_q, create_process(2,'b',70));
    add_to_queue(my_q, create_process(3,'c',10));
    

    rr(my_q, 5);
    return 0;
}
