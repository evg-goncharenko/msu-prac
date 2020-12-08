#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

/* pr1 < fname; pr2 | pr3 */
int main(int argc, char **argv)
{
    int file_in = 0;
    if(fork() == 0)
    {
        if((file_in = open(argv[4], O_RDONLY, 0777)) == -1)
        {
            perror(argv[4]);
            exit(1);
        }
        
        dup2(file_in, 0);
        
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    fflush(stdout);
    wait(NULL);
   
    int fd[2];
    pipe(fd);
    
    if(fork() == 0)
    {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[2], argv[2], (char*)0);
        exit(1);
    }
    wait(NULL);
    
    if(fork() == 0)
    {
        dup2(fd[0], 0);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[3], argv[3], (char*)0);
        exit(1);
    }
    
    close(fd[1]);
    close(fd[0]);
    
    while(wait(NULL) != -1);
    
    return 0;
}

