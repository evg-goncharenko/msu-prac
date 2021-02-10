#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t pid_array[3];
pid_t pid, child_val, cur_proc;
int numb_ch, is_blocked, start;

void f_handler(int sig) {
    if (pid == getpid()) {
        if (!start) {
            kill(pid_array[numb_ch], SIGUSR1);
        }
        numb_ch = (numb_ch + 1) % 3;
        kill(pid_array[numb_ch], SIGUSR1);
        start = 0;
    }
    signal(SIGINT, f_handler);
}

void user_handler(int sig) {
    is_blocked = (is_blocked + 1) % 2;
    signal(SIGUSR1, user_handler);
}

void kill_handler(int sig) {
    for (int i = 0; i < 3; i++) {
        kill(pid_array[i], SIGKILL);
    }
    while (wait(NULL) != -1);
    exit(0);
}

int main(int argc, char **argv) {
    start = 1;
    numb_ch = -1;
    is_blocked = 1;
    
    pid = getpid();
    signal(SIGINT, f_handler);
    signal(SIGUSR1, user_handler);
    signal(SIGALRM, kill_handler);
    
    for (int i = 0; i < 3; i++) {
        if ((child_val = fork()) == 0) {
            cur_proc = getpid();
            for (;;) {
                if (!is_blocked) {
                    printf("Process PID: %d\n", cur_proc);
                }
                sleep(1);
            }
        }
        pid_array[i] = child_val;
    }
    alarm(10);
    
    for (;;) {
        pause();
    }

    return(0);
}
