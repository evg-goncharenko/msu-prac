#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* pr1 | pr2 | pr3 */
void func1(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "You should have 4 arguments\n");
        exit(1);
    }
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        /*
            Identified standard output with a file descriptor
            channel intended for recording from 1 to 2:
        */
        dup2(fd[1], 1);
        /* Close unnecessary descriptors: */
        close(fd[1]);
        close(fd[0]);
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    
    int fd2[2];
    pipe(fd2);
    
    if (fork() == 0) {
        /*
            Identified standard output with a file descriptor
            channel intended for recording from 2 to 3:
        */
        dup2(fd2[1], 1);
        /*
            Identified standard input with a file descriptor
            channel intended for reading from 1 to 2:
        */
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[2], argv[2], (char*)0);
        exit(1);
    }
    
    close(fd[0]);
    close(fd[1]);
    
    if (fork() == 0) {
        /*
            Identified standard input with a file descriptor
            channel intended for reading from 2 to 3:
        */
        dup2(fd2[0], 0);
        close(fd2[1]);
        close(fd2[0]);
        execlp(argv[3], argv[3], (char*)0);
        exit(1);
    }
    
    close(fd2[0]);
    close(fd2[1]);
    
    while (wait(NULL) != -1);
}

/* pr1 | pr2 > f.res */
void func2(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "You should have 4 arguments\n");
        exit(1);
    }
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        /*
            Identified standard output with a file descriptor
            channel intended for recording from 1 to 2:
        */
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    
    if (fork() == 0) {
        int file_out;
        if ((file_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            perror(argv[3]);
            exit(1);
        }
        /*
            Identified standard input with a file descriptor
            channel intended for reading from 1 to 2:
        */
        dup2(fd[0], 0);
        
        /*
            Identifying standard output from 2 with a file:
        */
        dup2(file_out, 1);
        close(fd[1]);
        close(fd[0]);
        close(file_out);
        execlp(argv[2], argv[2], (char*)0);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    
    while (wait(NULL) != -1);
}

/* pr1 arg11 arg12 < f.dat | pr2 arg21 agr22 */
void func3(int argc, char **argv) {
    if (argc != 8) {
        fprintf(stderr, "You should have 8 arguments\n");
        exit(1);
    }
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        int file_in;
        if ((file_in = open(argv[4], O_RDONLY | O_CREAT, 0777)) == -1) {
            perror(argv[4]);
            exit(1);
        }
        /*
            Identified the file with standard input:
        */
        dup2(file_in, 0);
        /*
            Identified standard output with a file descriptor
            channel intended for recording:
        */
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        close(file_in);
        execlp(argv[1], argv[1], argv[2], argv[3], (char*)0);
        exit(1);
    }
    
    if (fork() == 0) {
        /*
            Identified standard input with a file descriptor
            channel intended for reading:
        */
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[5], argv[5], argv[6], argv[7], (char*)0);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    
    while (wait(NULL) != -1);
}

/* pr1 < f.dat > f.res */
void func4(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "You should have 4 arguments\n");
        exit(1);
    }
    
    if (fork() == 0) {
        int file_in;
        
        if ((file_in = open(argv[2], O_RDONLY | O_CREAT, 0777)) == -1) {
            perror(argv[2]);
            exit(1);
        }
        
        /*
            Identified the file with standard input:
        */
        dup2(file_in, 0);
        
        int file_out;
        
        if ((file_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            perror(argv[3]);
            exit(1);
        }
        
        /*
            Identified standard output with a file:
        */
        dup2(file_out, 1);
        
        close(file_out);
        close(file_in);
        
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    
    while (wait(NULL) != -1);
}

/* pr1 < f.dat | pr2 | pr3 > f.res */
void func5(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "You should have 6 arguments\n");
        exit(1);
    }
    
    int fd1[2];
    pipe(fd1);
    
    if (fork() == 0) {
        int file_in;
        if ((file_in = open(argv[2], O_RDONLY | O_CREAT, 0777)) == -1) {
            perror(argv[2]);
            exit(1);
        }
        
        dup2(file_in, 0);
        dup2(fd1[1], 1);
        
        close(file_in);
        close(fd1[1]);
        close(fd1[0]);
        
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    
    int fd2[2];
    pipe(fd2);
    
    if (fork() == 0) {
        dup2(fd1[0], 0);
        dup2(fd2[1], 1);
        
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        
        execlp(argv[3], argv[3], (char*)0);
        
        exit(1);
    }
    
    close(fd1[0]);
    close(fd1[1]);
    
    if (fork() == 0) {
        int file_out;
        if ((file_out = open(argv[5], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            perror(argv[5]);
            exit(1);
        }
        
        dup2(fd2[0], 0);
        dup2(file_out, 1);
        
        close(fd2[0]);
        close(fd2[1]);
        close(file_out);
        
        execlp(argv[4], argv[4], (char*)0);
        
        exit(1);
    }
    close(fd2[0]);
    close(fd2[1]);
    
    while (wait(NULL) != -1);
}

/* pr1 | pr2 >> f.res */
void func6(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "You should have 4 arguments\n");
        exit(1);
    }
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        /*
            Identified standard output with a file descriptor
            channel intended for recording from 1 to 2:
        */
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    
    if (fork() == 0) {
        int file_out;
        if ((file_out = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1) {
            perror(argv[3]);
            exit(1);
        }
        /*
            Identified standard input with a file descriptor
            channel intended for reading from 1 to 2:
        */
        dup2(fd[0], 0);
        /*
            Identified standard output with file from 2:
        */
        dup2(file_out, 1);
        close(fd[1]);
        close(fd[0]);
        close(file_out);
        execlp(argv[2], argv[2], (char*)0);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
}

/* pr1; pr2 | pr3 > f.res */
void func7(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "You should have 5 arguments\n");
        exit(1);
    }
    
    if (fork() == 0) {
        execlp(argv[1], argv[1], (char*)0);
        exit(1);
    }
    fflush(stdout);
    wait(NULL);
    
    int fd[2];
    pipe(fd);
    
    if (fork() == 0) {
        /*
            Identified standard output with a file descriptor
            channel intended for recording from 1 to 2:
        */
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[2], argv[2], (char*)0);
        exit(1);
    }
    
    if (fork() == 0) {
        int file_out;
        if ((file_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
            perror(argv[4]);
            exit(1);
        }
        /*
            Identified standard input with a file descriptor
            channel intended for reading from 1 to 2:
        */
        dup2(fd[0], 0);
        /*
            Identified standard output with file from 2:
        */
        dup2(file_out, 1);
        close(fd[1]);
        close(fd[0]);
        close(file_out);
        execlp(argv[3], argv[3], (char*)0);
        exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    
    while (wait(NULL) != -1);
}

int main(int argc, char **argv) {
    func7(argc, argv);
    return 0;
}
