/*create thread
step1 create a container to hold thread in main()
step2 initialize thread(thread_create())
step3 wait for the thread to finish(pthread_join())
*/

/*mutex
is like a lock to privent two threads to modify the same varible at the same time.
step1 create mutex object
step2 initialize mutex
stpe3 lock the function
step4 unlock the function
step5 destroy mutex
*/

/*conditional varible
step1 create conditional vaible
step2 initialize in main
step3 destroy cond
*/

/*
Create a consumer/producer problem.
We can have multiple consumers and multiple producers (TBD during seminar).
We should create one condition variable for stock empty, and one condition variable for stock full (100 units).
Each consumer will try to get/buy 10 units. If not enough units available, consumer waits for more supplies to complete they're order.
Each producer will supply 18 units. If not enough space available, producer waits for more space to complete they're delivery.
Print a status update each time the stock changes.
The producers show up every (TBD) sec.
The consumers show up every (TBD) sec.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CON_TH_NUM 7
#define PRO_TH_NUM 9
#define CON_MIN 10
#define PRO_ADD 18
#define CON_WAIT 2
#define PRO_WAIT 1

int buf = 0;

// create conditional vaible/mutex object
pthread_cond_t cond_buf_empty;
pthread_cond_t cond_buf_full;
pthread_mutex_t mutex;

void *thread_produce(void *arg)
{
    int done = 0;
    while (done != 1)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            while (buf > 100 - PRO_ADD)
            {
                //printf(" buf = %d, too much, waiting ...\n", buf);
                sleep(1);
                pthread_cond_wait(&cond_buf_full, &mutex);
            }
            buf += PRO_ADD;
            done = 1;
            printf("buf = %d\n", buf);
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&cond_buf_empty);
        }
        else
        {
            printf("producer wait ...\n");
            sleep(PRO_WAIT);
            done = 0;
        }
    }
}

void *thread_consume(void *arg)
{
    int done = 0;
    while (done != 1)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            while (buf < CON_MIN)
            {
                //printf("buf = %d, too less, waiting ...\n ", buf);
                sleep(1);
                // wait for another thread to send signal, (same mutex)
                pthread_cond_wait(&cond_buf_empty, &mutex);
                // printf("waiting ... buf = %d\n", buf);
            }
            buf -= CON_MIN;
            done =1;
            printf("buf = %d\n", buf);
            pthread_mutex_unlock(&mutex);
            pthread_cond_broadcast(&cond_buf_full);
        }
        else
        {
            printf("consumer wait ...\n");
            sleep(CON_WAIT);
            done =0;
        }
    }

    // printf("con_thread buf = %d\n", buf);
}

int main(int argc, char *argv[])
{

    // create a container to hold thread
    pthread_t th[PRO_TH_NUM + CON_TH_NUM];

    // initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // initialize con_v
    // pthread_cond_init(&cond, NULL);
    pthread_cond_init(&cond_buf_empty, NULL);
    pthread_cond_init(&cond_buf_full, NULL);

    // initialize thread
    //  1. pointer to pthread 
    //  2. NULL 
    //  3. the function thread should do 
    //  4. the function arg()
    int i;
    for (i = 0; i < CON_TH_NUM; i++)
    {
        if (pthread_create(&th[i], NULL, &thread_consume, NULL) != 0)
        {
            printf("fail to create thread_consume");
            return 1;
        }
        else
        {
            sleep(CON_WAIT);
        }
    }

    for (i = 0; i < PRO_TH_NUM; i++)
    {
        if (pthread_create(&th[i], NULL, &thread_produce, NULL) != 0)
        {
            printf("fail to create thread_produce");
            return 1;
        }
        else
        {
            sleep(PRO_WAIT);
        }
    }

    // wait for the thread to finish
    // 1. thread 2. the return value
    for (i = 0; i < CON_TH_NUM + PRO_TH_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("fail to join thread%d", i);
            return 2;
        }
    }

    pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond_buf_empty);
    pthread_cond_destroy(&cond_buf_full);
    return 0;
}