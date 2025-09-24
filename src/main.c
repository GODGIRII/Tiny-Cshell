#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PACSH_INPUT 1024
#define PACSH_TOK_DELIM " \t\r\n\a"

int pacsh_cd(char **args);
int pacsh_help(char **args);
int pacsh_ls(char **args);
int pacsh_exit(char **agrs);
int pacsh_grep(char **args);

int main(int argc, char *agrv[]) {
    char input[PACSH_INPUT];

    while (1) {
        printf("> "); // prompt

        if (fgets(input, PACSH_INPUT, stdin) == NULL) {
            break; // Ctrl+D exits
        }

        printf(" : %s\n", input);

        if (strcmp(input, "exit") == 0) {
            break;
        }
    }
    return 0;
}
