#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void display_prompt() {
    printf("#cisfun$ ");
    fflush(stdout);
}

void execute_command(char *command, char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {
        /* Child process */
        if (execvp(command, args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        waitpid(pid, NULL, 0);
    }
}

int main(void) {
    char input[1024];

    while (1) {
        display_prompt();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* Handle end-of-file condition (Ctrl+D) */
            printf("\n");
            break;
        }

        /* Remove newline character from input */
        input[strcspn(input, "\n")] = '\0';

        if (input[0] != '\0') {
            /* Tokenize input into command and arguments */
            char *token;
            char *args[1024];
            int arg_count = 0;

            token = strtok(input, " ");
            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            /* Execute command with arguments */
            execute_command(args[0], args);
        }
    }

    return 0;
}

