//Code for seminar 3

#include <sys/types.h> //provides libraries for fork (getpid) and other important system level types
#include <unistd.h>// used for fork(), exec(), pipe(), close(), write(), read(), functions
#include <stdio.h> // used for printf
#include <stdlib.h> // commonly used for memory allocation functions like malloc() and free()
#include <pthread.h> // necessary definitions and functions for creating and managing threads in a multi-threaded program

//code to change the # of producers and consumers
#define NUM_CONSUMERS 5
#define NUM_PRODUCERS 5

//code to change how often the producers and consumers show up
#define PRODUCER_DELAY 2
#define CONSUMER_DELAY 2

//typedef is alias for a structure
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // initialize mutex (mutual exclusion, like a flag): 0 = unlocked, 1 = locked
                                                        // without it it could lead to race conditions or unpredictability as other threads access
                                                        // global variable simultaneously and try to write to it at the same time (intitally unlocked)

// condition variables: one for empty, one for full
static pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER; //used to notify consumers when there is stock
static pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER; // used to notify producers when stock is not full

static int ready = 0; //current stock (number ready for purchase)
static int done = 0; // total num of items produced by manufacturer and consumed by consumers

static void* thread_produce(void *args) { //input argument will be the producer ids
    int ret;
    int producer_id = *((int*)args); // (int*)args = cast from type void to int, *((int*)args) = dereference the pointer and access the ID 

    for (; done < 200;) { // while less than 200 units have been purchased keep stocking up store shelves
        ret = pthread_mutex_lock(&mtx); // if unlocked calling producer proceeds, if locked wait here until unlocked by other thread
                                        // if mtx is aquired ret = 0, else ret = -1
        if (ret != 0) return NULL; // handles errors if mtx is not aquired as expected, this line causes the current thread to exit and return null

        while (ready == 100) { // if tthe stock is full (100 = max capacity)
            ret = pthread_cond_wait(&cond_full, &mtx); //wait here until we are notified that there is space to stock (cond_full = name of waitlist)
                                                       // if u are waiting here, mutex is unlocked and calling thread is put to sleep.
                                                       // if the condition changes, then proceed. This locks the door for you after. 
            if (ret != 0) return NULL; //error handling
        }

        int units_to_supply = 18; //each producer supplies 18 units
        while (units_to_supply > 0) {
            if (ready < 100) { //check again if there is less than 100 units since another thread may have already stocked
                // 100-ready = available space in stock
                int units_added = (units_to_supply <= 100 - ready) ? units_to_supply : (100 - ready); // if there is space for 18, supply 18 else, supply what you can 
                ready += units_added; //update value of the stock
                units_to_supply -= units_added; //update units to supply to see if you need to supply any more units to reach 18
                printf("Producer %d: + (Stock: %d)\n", producer_id, ready);
                pthread_cond_signal(&cond_empty); //signal to consumers that the stock is not empty
            } else {
                ret = pthread_cond_wait(&cond_full, &mtx); //wait if there is not enough space for delivery
                if (ret != 0) return NULL;
            }
        }

        ret = pthread_mutex_unlock(&mtx); //unlock the door
        if (ret != 0) return NULL;

        // Sleep to control producer arrival timing
        
    }
    return NULL;
}


static void* thread_consume(void *args) {
    int ret;
    int consumer_id = *((int*)args); // (int*)args = cast the id from type void to int, *((int*)args) = dereference the pointer and access the id
    
    for(;done < 200;) { //same as a while loop: stop buying when consumers purchase 200 or more items
        
        ret = pthread_mutex_lock(&mtx); // go inside if unclocked then lock door, if locked wait here
        if(ret != 0) return NULL;

        while(ready < 10) {
            ret = pthread_cond_wait(&cond_empty, &mtx); // wait for 10 available units, I am waiting on 'cond_empty': name of the waitlist
                                                 // if u are waiting here, mutex is unlocked and you are placed to sleep
                                                 // if condition changes the function locks the door again for you. If they have rasberry pis continue
                                                 // even if you got notified check again if there is stock, because multiple threads may have been notified
            if(ret != 0) return NULL;
        }

        for (int i = 0; i < 10; i++ ) //buy 10 units
        {
            if (ready > 0) //extra check in case another thread beat us here and consumed units already
            {
                --ready;
                ++done;        
            }
        } 
        printf("Consumer %d: - (Stock: %d)\n", consumer_id, ready); // Print status 

        ret = pthread_mutex_unlock(&mtx); // unlock door
        if(ret != 0) return NULL;

        // Signal that the stock is not full
        pthread_cond_signal(&cond_full);

        
    }
    return NULL;
}

int main(int argc, char*argv[]) {
    ready = 0;
    //creating an array of producers and consumers of specified size
    pthread_t consumers[NUM_CONSUMERS];
    pthread_t producers[NUM_PRODUCERS];

    int consumer_ids[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];

    pthread_cond_init(&cond_empty, NULL);
    pthread_cond_init(&cond_full, NULL);

    // the following loops will create each array thread and assign the corresponding id ranging from 0 to specified number of cons/prod -1 
    int ret;
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        consumer_ids[i]= i;
        ret = pthread_create(&consumers[i], NULL, thread_consume, &consumer_ids[i] );
    }
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        producer_ids[i]= i;
        ret = pthread_create(&producers[i], NULL, thread_produce, &producer_ids[i] );
    }

    //JOIN:
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        if(pthread_join(consumers[i],NULL)!=0){
            printf("error\n");
            return 2;

        }else{
            sleep(CONSUMER_DELAY); //the delay controls how often the producers show up
        }
    }
    
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        if(pthread_join(producers[i],NULL) != 0){
            printf("error\n");
            return 2;

        }else{
            sleep(PRODUCER_DELAY); //the delay controls how often the producers show up
        }
    }
    // done = 200; //at some point we want to finish threads
       
    printf("\n"); // I added this, helped me to see the result clearer
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);
    return 0;
}
