#include "tokenize.h"
#include <string.h>

int tokenize(char *line, char *argv[], int max_args) {
    int argc = 0; //argc counts how many tokens you put in argv
    char *saveptr = NULL;

    char *tok = strtok_r(line, " \t", &saveptr); //strtok_r finds the first word in line

    //Loop: put all tokens in argv:

    while (tok != NULL && argc < max_args - 1) {
        argv[argc++] = tok;
        tok = strtok_r(NULL, " \t", &saveptr);
    }

    argv[argc] = NULL;
    return argc;
}
