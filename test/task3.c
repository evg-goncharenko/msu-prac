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

int fd[2];
int f_l_size = 1;
int flag = 0;
int pid;
int numb_files = 0;

char *read_str(FILE *fr) {
    int sz = 2;
    char *s = malloc(sz);
    s[0] = 0;
    char c = 0;
    int i = 0;
    
    while (fread(&c, 1, 1, fr)) {
        if (c == '\n') break;
        
        if ((i + 1) == sz) {
            sz *= 2;
            s = realloc(s, sz);
        }
        s[i] = c;
        i++;
    }
    s[i] = 0;
    
    if (strlen(s) == 0) {
        free(s);
        s = NULL;
    }
    return s;
}

int file_len(char *file_name) {
    int res = 0;
    int file;
    if ((file = open(file_name, O_RDONLY)) == -1) {
        perror(file_name);
        return -1;
    }

    int tmp_c;
    while ((read(file, &tmp_c, sizeof(char))) > 0) {
        res++;
    }
    close(file);
    return res;
}

void ch_handler(int sig) {
    char *fl_name = NULL;
    read(fd[0], &f_l_size, sizeof(int));
    
    if (f_l_size == 0) {
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }

    fl_name = (char*)malloc(f_l_size+1);
    read(fd[0], fl_name, f_l_size);
    fl_name[f_l_size] = 0;
    printf("file name: %s\n", fl_name);

    int sz = file_len(fl_name);
    write(fd[1], &sz, sizeof(int));
    kill(getppid(), SIGUSR2);
    return;
}

void f_handler(int sig) {
    if (f_l_size == 0) {
        write(fd[1], &f_l_size, sizeof(int));
        kill(pid, SIGUSR1);
    } else {
        int sz;
        read(fd[0], &sz, sizeof(int));
        printf("file len: %d\n", sz);
        flag = 1;
    }
}

int main(int arg, char **argv) {
    pipe(fd);
    signal(SIGUSR1, ch_handler);
    
    if ((pid = fork()) == 0) {
        for (;;);
        exit(0);
    } else {
        signal(SIGUSR2, f_handler);
        FILE *fr = fopen(argv[1], "r");
        if (!fr) {
            fprintf(stderr, "\nCan't open file: '%s'\n", argv[1]);
            exit(1);
        }
        char *cur_file;
        cur_file = read_str(fr);
        
        while (cur_file != NULL) {
            numb_files++;
            f_l_size = strlen(cur_file);
            write(fd[1], &f_l_size, sizeof(int));
            write(fd[1], cur_file, f_l_size);
            kill(pid, SIGUSR1);
            
            for (;;) {
                if (!flag) {
                    pause();
                } else {
                    flag = 0;
                    break;
                }
            }
            free(cur_file);
            cur_file = NULL;
            cur_file = read_str(fr);
        }
        f_l_size = 0;
        kill(getpid(), SIGUSR2);
        free(cur_file);
        cur_file = NULL;
        
        while (wait(NULL) != -1);
        close(fd[0]);
        close(fd[1]);
        printf("Number of processed files: %d\n", numb_files);
    }
    
    return 0;
}
