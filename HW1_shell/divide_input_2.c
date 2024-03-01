#include <stdio.h>
#include <string.h>

#define MAX_ARGS 50 // Maximum number of arguments (adjust as needed)
#define MAX_ARG_LEN 100 // Maximum length of an argument (adjust as needed)

int main() {
    char input[MAX_ARG_LEN]; // Input buffer
    char *arguments[MAX_ARGS];

    while (1) {
        // Display the prompt
        printf("> ");
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit the loop on EOF (Ctrl+D)
        }
        
        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input string using space as the delimiter
        char *token;
        int argCount = 0;

        token = strtok(input, " ");

        // Initialize the arguments array
        for (int i = 0; i < MAX_ARGS; i++) {
            arguments[i] = NULL;
        }

        // Parse the tokens
        while (token != NULL && argCount < MAX_ARGS) {
            arguments[argCount] = token;
            token = strtok(NULL, " ");
            argCount++;
        }

        // Check if the user entered a command
        if (argCount > 0) {
            // The first element (arguments[0]) is the command
            printf("Command: %s\n", arguments[0]);

            // The remaining elements are parameters
            printf("Parameters:\n");
            for (int i = 1; i < argCount; i++) {
                printf("- %s\n", arguments[i]);
            }
        }
    }

    return 0;
}
