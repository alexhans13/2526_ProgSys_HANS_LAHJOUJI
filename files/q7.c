#include "redir.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

//execvp must not receive the tokens “>” and “filelist.txt”
//remove() remove this two tokens: 

static void remove(char *argv[], int i) {
    
    for (int j = i; argv[j] != NULL; j++) {
        argv[j] = argv[j + 2];
    }
}

int redirections(char *argv[]) {
    for (int i = 0; argv[i] != NULL; i++) {

        if (strcmp(argv[i], "<") == 0) {
            if (argv[i + 1] == NULL) return -1; 

            int fd = open(argv[i + 1], O_RDONLY); //opens the file in read mode
            if (fd < 0) return -1; //fd < 0 means that the file does not exist

            if (dup2(fd, STDIN_FILENO) < 0) // the default entry (STDIN_FILENO=0) = the file
             { close(fd); return -1; }
            close(fd);

            remove(argv, i); 
            i--; 
        }

        else if (strcmp(argv[i], ">") == 0) {
            if (argv[i + 1] == NULL) return -1; 

            int fd = open(argv[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) return -1;

            if (dup2(fd, STDOUT_FILENO) < 0) { close(fd); return -1; }
            close(fd);

            remove(argv, i);
            i--;
        }
    }

    return 0;
}
