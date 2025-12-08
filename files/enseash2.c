#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 1024

int main(void) {
    const char *welcome =
        "Bienvenue dans le Shell ENSEA.\n"
        "Pour quitter, tapez 'exit'.\n";
    const char *prompt = "enseash % ";
    const char *bye = "Bye bye...\n";

    char buffer[BUF_SIZE];

    write(STDOUT_FILENO, welcome, strlen(welcome));

    while (1) {
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


            
            _exit(1);
        } else if (pid > 0) {
            
            waitpid(pid, NULL, 0);
        } else {
            
            break;
        }
    }

    return 0;
}
