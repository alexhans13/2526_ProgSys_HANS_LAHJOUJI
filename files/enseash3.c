#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main(void) {
    const char *welcome =
        "Bienvenue dans le Shell ENSEA.\n"
        "Pour quitter, tapez 'exit'.\n";
    const char *bye = "Bye bye...\n";

    char buffer[BUF_SIZE];

    
    int has_status = 0;    
    int last_exit = 0;     
    int last_sign = 0;     
    char prompt[64];       

    write(STDOUT_FILENO, welcome, strlen(welcome));

    while (1) {
        
        if (!has_status) {
            strcpy(prompt, "enseash % ");
        } else if (last_sign != 0) {
            snprintf(prompt, sizeof(prompt),
                     "enseash [sign:%d] %% ", last_sign);
        } else {
            snprintf(prompt, sizeof(prompt),
                     "enseash [exit:%d] %% ", last_exit);
        }
        write(STDOUT_FILENO, prompt, strlen(prompt));
        

        
        ssize_t n = read(STDIN_FILENO, buffer, BUF_SIZE - 1);

        if (n == 0) {             
            write(STDOUT_FILENO, bye, strlen(bye));
            break;
        }
        if (n < 0) {               
            break;
        }

        buffer[n] = '\0';

        
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

        if (buffer[0] == '\0') {
            continue;
        }

        
        if (strcmp(buffer, "exit") == 0) {
            write(STDOUT_FILENO, bye, strlen(bye));
            break;
        }

        
        pid_t pid = fork();

        if (pid == 0) {
            char *argv[2];
            argv[0] = buffer;
            argv[1] = NULL;

            execvp(buffer, argv);

            
            _exit(127);
        } else if (pid > 0) {
            int status;

            if (waitpid(pid, &status, 0) > 0) {
                has_status = 1;
                last_exit = 0;
                last_sign = 0;

                if (WIFEXITED(status)) {
                    last_exit = WEXITSTATUS(status);
                } else if (WIFSIGNALED(status)) {
                    last_sign = WTERMSIG(status);
                }
            }
        } else {
            
            break;
        }
    }

    return 0;
}
