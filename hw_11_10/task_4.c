/*
    The program simulates the SHELL command: pr1; pr2; ... ; pr_n.

    Start all processes in parallel, wait for them to complete,
    and output the number of processes that completed normally:
    - from the programmer's point of view;
    - from the OS's point of view, but not from the programmer's point of view;
    - the number of processes that failed
    (the result is the sum of these 3 number of running processes)

    Process names pr1, pr2, ... , pr_n are set on the command line.
*/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int status, result;
    int errors = 0;
    int success_user = 0;
    int success_comp = 0;
    
    pid_t pid;
    
    /* Running all processes in parallel: */
    for(int i = 1; i < argc; i++)
    {
        if((pid = fork()) == 0)
        {
            execlp(argv[i], argv[i], (char*)0);
            exit(1);
        }
    }
    
    for(int i = 1; i < argc; i++)
    {
        wait(&status);
        /* Completed normally from the programmer's point of view: */
        if(WIFEXITED(status) && !WEXITSTATUS(status))
        {
            success_user++;
        }
        /*
            Completed normally from the OS's point of view,
            but not from the programmer's point of view:
        */
        else if(WIFEXITED(status))
        {
            success_comp++;
        }
        /* The process ended abnormally: */
        else if(!WEXITSTATUS(status))
        {
            errors++;
        }
    }
    
    result = success_user + success_comp + errors;
    printf("%d\n", result);
    return 0;
}
