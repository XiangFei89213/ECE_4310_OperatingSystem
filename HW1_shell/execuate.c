#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARG_LEN 100 // Maximum length of each part (adjust as needed)

int main() {
    char input[MAX_ARG_LEN]; // Input buffer

    while (1) {
        // Display the prompt
        printf("Enter a command (or 'exit' to quit): ");
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit the loop on EOF (Ctrl+D)
        }
        
        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Check if the user wants to exit
        if (strcmp(input, "exit") == 0) {
            break;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            execlp(input, input, (char *)NULL);

            // If execlp() fails, it returns here
            perror("Execution failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
            printf("Child process exited with status %d\n", status);
        }
    }

    return 0;
}
