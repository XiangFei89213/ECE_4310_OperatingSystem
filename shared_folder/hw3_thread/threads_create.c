#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

/*create thread
step1 create a container to hold thread
step2 initialize thread(thread_create())
step3 wait for the thread to finish(pthread_join()) 
*/ 

void* routine(){
    printf("TEST from threads\n");
    sleep(3);
    printf("ending thread\n");
}

int main(){

    //create a container to hold thread
    pthread_t t1, t2;

    //initialize thread
    // 1. pointer to pthread 2. NULL 3. the function thread should do 4. the function arg()
    pthread_create(&t1, NULL, &routine, NULL);
    pthread_create(&t2, NULL, &routine, NULL);

    //wait for the thread to finish 
    //1. thread 2. the return value
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    return 0;
}