//this is not working 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

int main() {
    char input[MAX_INPUT_SIZE];
    
    while (1) {
        // Display a prompt
        printf("Shell> ");
        
        // Read a line of input from the user
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }
        
        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';
        
        // If the user enters "exit," exit the shell
        if (strcmp(input, "exit") == 0) {
            printf("Exiting the shell.\n");
            break;
        }
        
        // Create a child process to execute the command
        pid_t pid = fork();
        
        if (pid < 0) {
            //perror("fork");
            printf("fail to create process");
            //exit(EXIT_FAILURE);

        } else if (pid == 0) {
            // In the child process
            // Parse the input and execute the command
            // if (execvp(input, command ) < 0) {
            //     perror("execvp");
            //     exit(EXIT_FAILURE);
            // }
        } else {
            // In the parent process
            // Wait for the child process to complete
            // int status;
            // waitpid(pid, &status, 0);
            // wait(NULL);
        }
    }
    
    return 0;
}
