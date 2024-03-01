//from AA
//Lottery Scheduler
#include <unistd.h>
#include <stdio.h>//
#include <stdlib.h>
#include <time.h>

typedef struct process{
    int cpu_time_count;
    int length;
    int pid;
    int tickets;
    char name;
    //int OGtickets;
    struct process *pNext;
} process_t;

process_t *create_new_process(int pid, int size, int tickets_size){
        process_t *result = (process_t*)malloc(sizeof(process_t)); //*result still in stack; address of process_t* from malloc 
        result->pid = pid; // (*result): deref, instead: (*result).pid = pid
        result->name = 'A' + pid;
        result->length = size; //pointers should always use arrow to access members in its struct
        result->cpu_time_count = 0;
        result->tickets = tickets_size;
        //result->OGtickets = tickets_size;
        result->pNext = NULL;
        return result; 
    }

int get_size()
{
    int size;
    printf("\n what is process size?\n " );
    scanf("%d", &size);
    while(size <= 0){
        printf("Please Enter a valid Size: ");
        scanf("%d", &size);
    }
    return size;
}
int get_tickets(int *max_tix)
{
    int tickets;
    printf("\n How many tickets?\n " );
    scanf("%d", &tickets);
    while(tickets <= 0){
        printf("Please Enter a valid amount of ticket: ");
        scanf("%d", &tickets);
    }
    *max_tix = *max_tix + tickets;
    return tickets;
}

int random_num(int my_max)
{
    srand(time(NULL));
    int r = rand() % my_max;
    return r; 
}

void displayList(process_t *head, int *maxtix)
{
    //printf("in displaylist");
    process_t *temp_head = NULL;
    temp_head = head;
    if(temp_head == NULL)
    {
        printf("List is Null or Complete\n");
    }
    while(temp_head != NULL)
    {
        printf("Job name:%c, Tickets Left:%d out of (%d) tickets, Length:%d \n", temp_head->name, temp_head->tickets, *maxtix, temp_head->length);
        temp_head = temp_head->pNext;
    }
}

void pop(char name, process_t ***head,int pid)
{
    process_t *cur_head = (**head)->pNext;
    process_t *prev_head = **head;
    //process_t *tmp;
    printf("Pop_Job: %c\n", name );
    if (prev_head->pid == pid)
    {
        //remove first 
        **head = (**head)->pNext;
        //tmp = cur_head;
        //free(tmp);
    }
    while(cur_head != NULL)  
    {
        if(cur_head->pid == pid) 
        {
            //remove cur
            prev_head->pNext = cur_head->pNext; 
            //tmp = prev_head; lines 97 - 100 added after submission for improved memory management
            //prev_head = cur_head;
            //free(tmp);
            //prev_head = NULL; 
            break;
        }   
        //store cur to prev
        prev_head = cur_head;
        //tranverse cur to next
        cur_head = cur_head->pNext;
    }
    

}
void run_lottery(process_t **head, int *max_tix)
{
    
    int pick = random_num(*max_tix);
    int counter = 0;
    process_t *temp_head = *head;

    
    if(*head == NULL)
    {
        printf("List Empty");
    }

    //finding the winner in the list
    while(temp_head != NULL)
    {  
        counter = counter + temp_head->tickets;
        
        if(pick < counter)
        {
            temp_head->tickets--;
            temp_head->length--;
            *max_tix = *max_tix - 1;
            printf("Job %c won! Now has %d tickets and %d length left. Total Tickets Left:%d \n", temp_head->name, temp_head->tickets, temp_head->length, *max_tix);
            if(temp_head->tickets == 0 && temp_head->length > 0)
            {
                //reset ticket
                temp_head->tickets = temp_head->length;
                //temp_head->tickets = temp_head->OGtickets;
                *max_tix = *max_tix + temp_head->tickets;
                printf("Reseted: ");
                printf("Job %c. Now has %d tickets and %d length left. Total Tickets Left:%d \n", temp_head->name, temp_head->tickets, temp_head->length, *max_tix);

            }
    
            if(temp_head->length == 0)
            {
                *max_tix = *max_tix - temp_head->tickets;
                pop(temp_head->name, &head, temp_head->pid);
                break;
            }
                    
            break;
        }

        temp_head = temp_head->pNext;

    }
}
int main(){
    int max_tix = 0;
    int pr_count = 0;
    process_t *head = NULL;
    printf("specify # of processes:");
    scanf("%d", &pr_count);
    if(pr_count <= 0){
        printf("No Processes\n");
        return 0;
    }
    
    for(int ix = 0 ; ix < pr_count; ix++){
        int size = get_size();
        int tickets = get_tickets(&max_tix);
        process_t *newpr = create_new_process(ix, size, tickets);
        if(head == NULL){
            head = newpr;
        }
        else{
            newpr->pNext = head;
        }
        head = newpr;
        //printf("%d", );       
    }
    printf("\nStarting List:\n");
    displayList(head, &max_tix);
    while(head != NULL){
        printf("\nNew Draw: ");
        run_lottery(&head, &max_tix);
        printf("\nCurrent Main list:\n");
        displayList(head, &max_tix);
    }
    
    //displayList(head, &max_tix);
    return 0;
}
