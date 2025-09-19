#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PACSH_INPUT 1024
#define PACSH_TOK_DELIM " \t\r\n\a"

int main(int argc, char *argv[]){
    char input[PACSH_INPUT];
    
    while (1) {
        printf("> ");
        if (fgets(input, PACSH_INPUT,stdin) == NULL) {
            break;
        }

    }
}

