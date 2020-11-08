/*
    The program simulates the SHELL command: pr1 && pr2
    (execute command pr1; if command pr1 completes successfully run command pr2,
    else terminate the program).
    Process names are set on the command line.
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
    int status;
    pid_t pid;
    
    if((pid = fork()) == 0)
    {
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    wait(&status);
    if(WIFEXITED(status) && !WEXITSTATUS(status))
    {
        if((pid = fork()) == 0)
        {
            execlp(argv[2], argv[2], (char*)0);
            exit(1);
        }
    }
    wait(NULL);
    
    return 0;
}
