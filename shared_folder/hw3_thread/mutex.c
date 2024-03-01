//create and use a mutex to solve chasing problem
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

/*mutex
is like a lock to privent two threads to modify the same varible at the same time.
step1 create mutex object
step2 initialize mutex
stpe3 lock the function
step4 unlock the function
step5 destroy mutex
*/
int mails = 0;

//create mutex object
pthread_mutex_t mutex;

void* routine(){
    int i;
    for(i=0;i<10000000;i++){
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
        
    }

}

int main(int argc, char *argv[]){
    
    pthread_t p1,p2,p3,p4;
    //initialize mutex
    pthread_mutex_init(&mutex, NULL);

    if(pthread_create(&p1, NULL, &routine, NULL) != 0){
        return 1;
    }

    if(pthread_create(&p2, NULL, &routine, NULL) != 0){
        return 1;
    }

    if(pthread_create(&p3, NULL, &routine, NULL) != 0){
        return 1;
    }

    if(pthread_create(&p4, NULL, &routine, NULL) != 0){
        return 1;
    }

    if(pthread_join(p1,NULL)!=0){
        return 3;
    }

    if(pthread_join(p2,NULL)!=0){
        return 4;
    }
    
    if(pthread_join(p3,NULL)!=0){
        return 3;
    }

    if(pthread_join(p4,NULL)!=0){
        return 4;
    }

    pthread_mutex_destroy(&mutex);
    printf("number of mails: %d\n", mails);

    return 0;
}