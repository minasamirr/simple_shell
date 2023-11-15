#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 64
#define MAX_ARGS 16

void display_prompt() {
    printf("#cisfun$ ");
    fflush(stdout);
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_ARGS];
    pid_t pid;
    char *token;

    while (1) {
	int i;
	    display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        token = strtok(input, " ");
        i = 0;

        while (token != NULL && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execvp(args[0], args) == -1) {
                perror("exec");
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                fprintf(stderr, "./shell: %s: command not found\n", args[0]);
            }
        }
    }

    return 0;
}
