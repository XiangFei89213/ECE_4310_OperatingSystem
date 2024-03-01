// shell homework

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
// linux library

#define MAX_INPUT_SIZE 1028
#define MAX_PARAMETOR_SIZE 772
#define MAX_PARA 10 // Maximum number of Parameter 
#define MAX_SINGLE_PARA_SIZE 70


int main(int argc, char* argv[]){
//?? parametor 

    char input[MAX_INPUT_SIZE];
    char* command[MAX_PARA];
    int i;

    while(1){
        //step1// read input command //
        printf("Shell>");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting the shell.\n");
        break;
        }

         
        //step2// Parse the User Input into command and arguments
        char *token;
        int argCount = 0;
        token = strtok(input, " ");

        // Initialize the arguments array for the next time input
        for (int i = 0; i < MAX_PARA; i++) {
            command[i] = NULL;
        }

        // Parse the tokens
        while (token != NULL && argCount < MAX_PARA) {
            command[argCount] = token;
            token = strtok(NULL, " ");
            argCount++;
        }

        // Check if the user entered a command
        if (argCount > 0) {
            // The first element (arguments[0]) is the command
            printf("Command: %s\n", command[0]);

            // The remaining elements are parameters
            printf("Parameters:\n");
            for (int i = 1; i < argCount; i++) {
                printf("- %s\n", command[i]);
            }
        }


        //step3//create a child process and exe
        int pid = fork();

        if (pid != 0) {
            printf("fail to create process");
            break;
        }
    } 
    return 0;
}