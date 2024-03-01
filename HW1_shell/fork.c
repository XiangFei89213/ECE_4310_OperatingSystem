// fork practice

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// linux library

int main(int argc, char* argv[] ){

    printf("this is before fork, only exe once\n");

    int id = fork();    //child process get build
                        //chikd process will execute along the way when the main process execute
                        //child process id will always be zero
                        //execute n times fork will create 2^n process
    
    // printf("hello world from id = %d\n", id);

    if (id == 0){
        printf(" This is in the child process");
        printf("hello from the child process id = %d \n", id);
    } else {
        printf("hello from the main process id = %d \n", id);
    }

    return 0;
}