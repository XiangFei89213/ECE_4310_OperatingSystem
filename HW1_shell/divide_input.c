// divide input into command array + parametors array
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 50 // Maximum number of arguments (adjust as needed)
#define MAX_ARG_LEN 100 // Maximum length of an argument (adjust as needed)

int main() {
    char input[MAX_ARG_LEN]; // Input buffer
    char command[MAX_ARG_LEN];
    char parameters[MAX_ARGS][MAX_ARG_LEN]; // Array of parameters
    int paramCount = 0;

    while (1) {
        // Display the prompt
        printf("> ");
        
        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit the loop on EOF (Ctrl+D)
        }
        
        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input by space
        char *token = strtok(input, " ");

        // Check if the user entered a command
        if (token != NULL) {
            // Copy the first token as the command
            strncpy(command, token, MAX_ARG_LEN);

            // Process the rest of the tokens as parameters
            paramCount = 0;
            while ((token = strtok(NULL, " ")) != NULL && paramCount < MAX_ARGS) {
                strncpy(parameters[paramCount], token, MAX_ARG_LEN);
                paramCount++;
            }

            // Print the command and parameters
            printf("Command: %s\n", command);
            printf("Parameters:\n");
            for (int i = 0; i < paramCount; i++) {
                printf("- %s\n", parameters[i]);
            }
        }
    }

    return 0;
}
