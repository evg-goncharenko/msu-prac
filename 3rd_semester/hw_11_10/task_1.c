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

int main(int argc, char **argv) {
    int status;
    pid_t pid;
    
    if ((pid = fork()) == 0) {
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    wait(&status);
    /*
        WIFEXITED(status) - returns true if the child terminated normally.
         
        WEXITSTATUS(status) - returns the exit status of the child,
        this consists of the least significant 8 bits of the status argument
        that the child specified in a call to exit() or as the argument
        for a return statement in main().
        This macro should only be employed if WIFEXITED() returned true.
    */
    if ((WIFEXITED(status) == 0) || (WEXITSTATUS(status) != 0)) {
        if ((pid = fork()) == 0) {
            execlp(argv[2], argv[2], (char*)0);
            exit(1);
        }
    }
    wait(NULL);
    
    return 0;
}
