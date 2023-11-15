#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void display_prompt() {
    printf("#cisfun$ ");
    fflush(stdout);
}

int main() {
    char input[MAX_INPUT_SIZE];
    pid_t pid;

    while (1) {
        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (execlp(input, input, (char *)NULL) == -1) {
                perror("exec");
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                fprintf(stderr, "./shell: %s: command not found\n", input);
            }
        }
    }

    return 0;
}

