/*create thread
step1 create a container to hold thread
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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
/*
mutex lock and trylock
threads are able to do other things/ like wait and try again later 
if using mutex
*/

pthread_mutex_t stoveMutex[4];
int stoveFuel[4] = { 100, 100, 100, 100 };

void* routine(void* args) {
    for (int i = 0; i < 4; i++) {
        if (pthread_mutex_trylock(&stoveMutex[i]) == 0) {
            int fuelNeeded = (rand() % 30);
            if (stoveFuel[i] - fuelNeeded < 0) {
                printf("No more fuel... going home\n");
            } else {
                stoveFuel[i] -= fuelNeeded;
                usleep(500000);
                printf("Fuel left %d\n", stoveFuel[i]);
            }
            pthread_mutex_unlock(&stoveMutex[i]);
            break;
        } else {
            if (i == 3) {
                printf("No stove available yet, waiting...\n");
                usleep(300000);
                //able to try again
                i = 0;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    pthread_t th[10];
    for (int i = 0; i < 4; i++) {
        pthread_mutex_init(&stoveMutex[i], NULL);
    }
    for (int i = 0; i < 10; i++) {
        if (pthread_create(&th[i], NULL, &routine, NULL) != 0) {
            perror("Failed to create thread");
        }
    }

    for (int i = 0; i < 10; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    for (int i = 0; i < 4; i++) {
        pthread_mutex_destroy(&stoveMutex[i]);
    }
    return 0;
}