#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define CON_TH_NUM 5
#define PRO_TH_NUM 2
#define CON_MIN 5
#define PRO_ADD 8
#define CON_WAIT 6
#define PRO_WAIT 9

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

int buf = 0;

// create conditional vaible/mutex object
pthread_cond_t cond_buf_empty;
pthread_cond_t cond;
pthread_cond_t cond_buf_full;
pthread_mutex_t mutex;

void *add_in_buf(void *arg)
{
    int finish = 0;
    while (finish != 1)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            for (int i = 0; i < PRO_ADD; i++)
            {
                buf = buf + 1;
                printf("buf = %d\n", buf);
                pthread_mutex_unlock(&mutex);
                // after unlock send a signal
                if(i==PRO_ADD-1){
                    finish = 1;
                }
                pthread_cond_broadcast(&cond);
            }
        }
        else
        {
            sleep(1);
            finish = 0;
        }
        
    }
    //printf("a_thread buf = %d\n", buf);
}

void *minus_from_buf(void *arg)
{
    pthread_mutex_lock(&mutex);

    while (buf < CON_MIN)
    {
        sleep(1);
        pthread_cond_wait(&cond, &mutex);
    }

    buf = buf - CON_MIN;
    pthread_mutex_unlock(&mutex);
    printf("m_thread buf = %d\n", buf);
}

int main(int argc, char *argv[])
{

    // create a container to hold thread
    pthread_t th[PRO_TH_NUM + CON_TH_NUM];

    // initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // initialize con_v
    pthread_cond_init(&cond_buf_empty, NULL);
    pthread_cond_init(&cond_buf_full, NULL);
    pthread_cond_init(&cond, NULL);

    // initialize thread
    //  1. pointer to pthread 2. NULL 3. the function thread should do 4. the function arg()
    int i;
    for (i = 0; i < CON_TH_NUM; i++)
    {
        if (pthread_create(&th[i], NULL, &minus_from_buf, NULL) != 0)
        {
            printf("fail to create minus_from_buf thread");
            return 1;
        }
    }

    for (i = 0; i < PRO_TH_NUM; i++)
    {
        if (pthread_create(&th[i], NULL, &add_in_buf, NULL) != 0)
        {
            printf("fail to create add_in_buf thread");
            return 1;
        }
    }

    // wait for the thread to finish
    // 1. thread 2. the return value
    for (i = 0; i < CON_TH_NUM + PRO_TH_NUM; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            printf("fail to join thread");
            return 1;
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_cond_destroy(&cond_buf_empty);
    pthread_cond_destroy(&cond_buf_full);
    return 0;
}