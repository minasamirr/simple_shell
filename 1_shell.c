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

void execute_command(char *command) {
    char *empty_env[] = {NULL};

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {
        /* Child process */
        char *args[2];
        args[0] = command;
        args[1] = NULL;

        if (execve(command, args, empty_env) == -1) {
            perror("execve");
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
            execute_command(input);
        }
    }

    return 0;
}
