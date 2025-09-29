#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define PACSH_INPUT 1024
#define PACSH_TOK_DELIM " \t\r\n\a"

//          Builtin command declarations
int pacsh_cd(char **args);
int pacsh_help(char **args);
int pacsh_exit(char **args);

//          List of builtins
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &pacsh_cd,
    &pacsh_help,
    &pacsh_exit
};

int pacsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

// ---------- Builtin implementations ----------
int pacsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "pacsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("pacsh");
        }
    }
    return 1;
}

int pacsh_help(char **args) {
    (void)args; // silence unused warning
    printf("PacShell — custom shell\n");
    printf("Builtins:\n");
    for (int i = 0; i < pacsh_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    return 1;
}

int pacsh_exit(char **args) {
    (void)args; // silence unused warning
    return 0;   // signal exit
}

// ---------- Input reader ----------
char *pacsh_read_line(void) {
    char *line = NULL;
    size_t bufsize = 0; // must be size_t for getline
    getline(&line, &bufsize, stdin);
    return line;
}

// ---------- Tokenizer ----------
char **pacsh_split_line(char *line) {
    int bufsize = 64, pos = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "pacsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, PACSH_TOK_DELIM);
    while (token != NULL) {
        tokens[pos++] = token;

        if (pos >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "pacsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, PACSH_TOK_DELIM);
    }
    tokens[pos] = NULL;
    return tokens;
}

// ---------- Process launcher ----------
int pacsh_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("pacsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("pacsh");
    } else {
        // parent process waits
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

// ---------- Command executor ----------
int pacsh_execute(char **args) {
    if (args[0] == NULL) {
        return 1; // empty command
    }

    for (int i = 0; i < pacsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return pacsh_launch(args);
}

// ---------- Main loop ----------
void pacsh_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("$ ");
        line = pacsh_read_line();
        args = pacsh_split_line(line);
        status = pacsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

// ---------- main ----------
int main(void) {
    // TODO: load config file ~/.pacrc later
    pacsh_loop();
    return EXIT_SUCCESS;
}
              
