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

int count = 1;

void sig_handler(int s) {
    count++;
    if (count == 7) {
        signal(SIGINT, SIG_DFL);
    } else {
        if (count > 2 && count < 4) {
            signal(SIGTRAP, sig_handler);
        } else {
            signal(SIGINT, sig_handler);
        }
    }
}

int main() {
    signal(SIGINT, sig_handler);
    for (;;)
        ;
    return 0;
}
