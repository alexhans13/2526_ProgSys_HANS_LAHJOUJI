#define _POSIX_C_SOURCE 199309L  //it activate clock_gettime()

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>

#define BUF_SIZE 1024 //standard size for buffer

int main(void) {
    const char *welcome =
        "Bienvenue dans le Shell ENSEA.\n"
        "Pour quitter, tapez 'exit'.\n";
    const char *bye = "Bye bye...\n";

    char buffer[BUF_SIZE];

   
    int  has_status   = 0;     //flag
    int  last_exit    = 0;     //it stores the return code 
    int  last_sign    = 0;     //it stores signal number
    long last_time_ms = 0;     //it stores the execution time of the previous command
    char prompt[64];     //buffer for prompt     

    write(STDOUT_FILENO, welcome, strlen(welcome));

    while (1) {
       //If no command has been issued yet
        if (!has_status) {
            strcpy(prompt, "enseash % ");
        } 
       // if the previous command ended with a signal
            
        else if (last_sign != 0) {
            snprintf(prompt, sizeof(prompt),
                     "enseash [sign:%d|%ldms] %% ",
                     last_sign, last_time_ms);
        } 
        //otherwise she ended up normally
        
        else {
            snprintf(prompt, sizeof(prompt),
                     "enseash [exit:%d|%ldms] %% ",
                     last_exit, last_time_ms);
        }
        
        write(STDOUT_FILENO, prompt, strlen(prompt));
       

       //Read what the user types:
        ssize_t n = read(STDIN_FILENO, buffer, BUF_SIZE - 1); // n : number of characters read.

        //ctrl +D
        if (n == 0) { 
            write(STDOUT_FILENO, bye, strlen(bye));
            break;
        }

        // if read() failed
        if (n < 0) {  
            break;
        }

        buffer[n] = '\0'; // to indicate the end of the character string


        // cleaning of \n, spaces, and tabs
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
        }

        
        int i = strlen(buffer) - 1;
        while (i >= 0 && (buffer[i] == ' ' || buffer[i] == '\t')) {
            buffer[i] = '\0';
            i--;
        }

        
        if (buffer[0] == '\0') {
            continue;
        }

       // exit command:
        // strcmp for comparison 
        if (strcmp(buffer, "exit") == 0) {
            write(STDOUT_FILENO, bye, strlen(bye));
            break;
        }

        //measurement of execution time
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        

        pid_t pid = fork(); // creation of the fork

        //child process:
        if (pid == 0) {
           
            char *argv[2];
            argv[0] = buffer;
            argv[1] = NULL;

            execvp(buffer, argv); // replaces the child program with the user's command 

            
            _exit(127); // exit with 127 if execvp failed
            
        } 
        
        //Parent process
        
        else if (pid > 0) {
            int status;

            //it waits for the end of the child process with waitpid
            if (waitpid(pid, &status, 0) > 0) {
                
                clock_gettime(CLOCK_MONOTONIC, &end);
                long sec  = end.tv_sec  - start.tv_sec;
                long nsec = end.tv_nsec - start.tv_nsec;
                last_time_ms = sec * 1000 + nsec / 1000000;
                if (last_time_ms < 0) last_time_ms = 0;
                

                has_status = 1;
                last_exit  = 0;
                last_sign  = 0;

                //it retrieves the return code/signal: 
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

