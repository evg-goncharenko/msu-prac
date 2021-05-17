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
    pid_t pid;

    for (int i = 1; i < argc; i++) {
        if ((pid = fork()) == 0) {
            execlp(argv[i], argv[i], (char *)0);
            exit(1);
        }
        wait(NULL);
    }
    return 0;
}
