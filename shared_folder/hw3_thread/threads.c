//from teacher
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob =0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void* thread_function(void *args){
    int loops = *((int *) args);
    int ix, ret;
    //++ infront will be faster
    for(ix =0;ix<loops;++ix){
        ret = pthread_mutex_lock(&mtx);
        if(ret!=0) return NULL;

        ++glob;

        ret = pthread_mutex_lock(&mtx);
        if(ret!=0) return NULL;
    }
    return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t t1, t2;
	int loops, ret;

    //stoi : convert string into int
    //if there is a input parameter(argv[1] has a value) use the input (parameter)
    //if not use default 1000000
	loops = (argc>1) ? atoi(argv[1]) : 1000000;

    //each treaads will incease glob by 2
    ret = pthread_create(&t1, NULL, thread_function, &loops);
    ret = pthread_create(&t2, NULL, thread_function, &loops);

    //wait for the thread to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("total = %d\n", glob);
    return 0;
}

