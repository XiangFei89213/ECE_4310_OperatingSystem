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

int main(int argc, char *argv[])
{

    char input[MAX_INPUT_SIZE];
    char *command[MAX_PARA];
    int i;
    char parameters[MAX_PARA][MAX_SINGLE_PARA_SIZE]; // Array of parameters
    int paramCount = 0;

    while (1)
    {
        // step1// read input command //
        printf("Shell>");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting the shell.\n");
            break;
        }

        // step2


        // Remove the newline character at the end of the input
        input[strcspn(input, "\n")] = '\0';

        // Tokenize the input by space
        char *token = strtok(input, " ");

        // Check if the user entered a command
        if (token != NULL)
        {
            // Copy the first token as the command
            strncpy(command, token, MAX_INPUT_SIZE);

            // Process the rest of the tokens as parameters
            paramCount = 0;
            while ((token = strtok(NULL, " ")) != NULL && paramCount < MAX_PARA)
            {
                strncpy(parameters[paramCount], token, MAX_SINGLE_PARA_SIZE);
                paramCount++;
            }

            // Print the command and parameters
            printf("Command: %s\n", command);
            printf("Parameters:\n");
            for (int i = 0; i < paramCount; i++)
            {
                printf("- %s\n", parameters[i]);
            }
        }
    }
    return 0;
}