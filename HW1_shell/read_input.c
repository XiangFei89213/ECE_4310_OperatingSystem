//read input practice

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SCANF_TIME 2
#define FGETS_TIME 2

int main(int argc, char* argv[]){
    char message[1200];
    int i;
    // for(i=1;i<=SCANF_TIME;i++){
    //     printf("scanf method %d", i);
    //     scanf("%s", message);
    //     printf( "\nthe %d message is + %s\n" , i,message);
    // }

    for(i=1;i<=FGETS_TIME;i++){
        printf("fgets method %d", i);
        fgets(message,256,stdin);
        printf( "\n the %d message is %s\n" , i,message);
        //存哪裡，max input size，stdin
    }
    return 0;

}