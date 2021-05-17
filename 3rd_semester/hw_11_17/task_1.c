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

/* pr1 arg1 arg2 | pr2; pr3 >> f.res */
void func1(int argc, char **argv) {
    if (argc != 7) {
        fprintf(stderr, "You should have 7 arguments\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);

    pid_t pid = fork();

    switch (pid) {
        case -1: {
            perror("System error with pid");
            exit(2);
        }
        case 0: {
            dup2(fd[1], 1);
            close(fd[1]);
            close(fd[0]);

            execlp(argv[1], argv[1], argv[2], argv[3], (char *)0);

            perror("Error with exec pr1");
            exit(3);
        }
        default: {
            break;
        }
    }

    pid_t pid2 = fork();
    switch (pid2) {
        case -1: {
            perror("System error with pid2");
            exit(4);
        }
        case 0: {
            dup2(fd[0], 0);
            close(fd[1]);
            close(fd[0]);

            execlp(argv[4], argv[4], (char *)0);

            perror("Error with exec pr2");
            exit(5);
        }
        default: {
            break;
        }
    }

    close(fd[1]);
    close(fd[0]);

    while (wait(NULL) != -1)
        ;

    pid_t pid3 = fork();
    switch (pid3) {
        case -1: {
            perror("System error with pid3");
            exit(6);
        }
        case 0: {
            int file_out;
            if ((file_out = open(argv[6], O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1) {
                perror(argv[6]);
                exit(7);
            }
            dup2(file_out, 1);
            close(fd[1]);
            close(fd[0]);
            close(file_out);

            execlp(argv[5], argv[5], (char *)0);

            perror("Error with exec pr3");
            exit(8);
        }
        default: {
            break;
        }
    }

    wait(NULL);
}

/* pr1 < f.dat | pr2 > f.res; pr3 */
void func2(int argc, char **argv) {
    if (argc != 6) {
        fprintf(stderr, "You should have 6 arguments\n");
        exit(1);
    }

    int fd[2];
    pipe(fd);

    pid_t pid = fork();
    switch (pid) {
        case -1: {
            perror("System error with pid");
            exit(2);
            break;
        }
        case 0: {
            int file_in;
            if ((file_in = open(argv[2], O_RDONLY | O_CREAT, 0777)) == -1) {
                perror(argv[2]);
                exit(3);
            }
            dup2(file_in, 0);
            dup2(fd[1], 1);
            close(fd[1]);
            close(fd[0]);

            execlp(argv[1], argv[1], (char *)0);

            perror("Error with exec pr1");
            exit(4);
            break;
        }
        default: {
            break;
        }
    }

    pid_t pid2 = fork();
    switch (pid2) {
        case -1: {
            perror("System error with pid2");
            exit(5);
            break;
        }
        case 0: {
            int file_out;
            if ((file_out = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
                perror(argv[4]);
                exit(6);
            }
            dup2(fd[0], 0);
            dup2(file_out, 1);
            close(fd[1]);
            close(fd[0]);

            execlp(argv[3], argv[3], (char *)0);

            perror("Error with exec pr2");
            exit(6);
            break;
        }
        default: {
            break;
        }
    }

    close(fd[1]);
    close(fd[0]);

    while (wait(NULL) != -1)
        ;

    execlp(argv[5], argv[5], (char *)0); /* exec pr3 */
}

/* pr1 | pr2 | ... | pr_n */
void func3(int argc, char **argv) {
    int i = 0;
    pid_t pid;
    int fd[2];

    while (argv[++i] != NULL) {
        pipe(fd);
        switch (pid = fork()) {
            case -1: {
                perror("System error with pid_i");
                exit(1);
                break;
            }
            case 0: {
                if (i + 1 != argc) {
                    dup2(fd[1], 1);
                }
                close(fd[1]);
                close(fd[0]);

                execlp(argv[i], argv[i], (char *)0);

                exit(2);
                break;
            }
            default: {
                break;
            }
        }
        dup2(fd[0], 0);

        close(fd[1]);
        close(fd[0]);
    }

    while (wait(NULL) != -1)
        ;
}

/* pr1 arg1 > f.res; pr2 | pr3 | pr4 >> f.res */
void func4(int argc, char **argv) {
    if (argc != 8) {
        fprintf(stderr, "You should have 8 arguments\n");
        exit(1);
    }

    pid_t pid = fork();
    switch (pid) {
        case -1: {
            perror("System error with pid");
            exit(2);
            break;
        }
        case 0: {
            int file_out;
            if ((file_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1) {
                perror(argv[3]);
                exit(3);
            }
            dup2(file_out, 1);

            execlp(argv[1], argv[1], argv[2], (char *)0);

            perror("Error with exec pr1");
            exit(4);
            break;
        }
        default: {
            break;
        }
    }
    while (wait(NULL) != -1)
        ;

    int fd[2];
    pipe(fd);

    pid_t pid2;

    for (int i = 4; i < 6; i++) {
        pipe(fd);
        switch (pid2 = fork()) {
            case -1: {
                perror("System error with pid2");
                exit(1);
                break;
            }
            case 0: {
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd[0]);

                execlp(argv[i], argv[i], (char *)0);

                perror("Error with exec pr_i");
                exit(2);
                break;
            }
            default: {
                break;
            }
        }
        dup2(fd[0], 0);

        close(fd[1]);
        close(fd[0]);
    }

    pid_t pid3 = fork();
    switch (pid3) {
        case -1: {
            perror("System error with pid3");
            exit(5);
            break;
        }
        case 0: {
            int file_out;
            if ((file_out = open(argv[7], O_WRONLY | O_CREAT | O_APPEND, 0777)) == -1) {
                perror(argv[7]);
                exit(6);
            }

            dup2(file_out, 1);
            close(fd[1]);
            close(fd[0]);

            execlp(argv[6], argv[6], (char *)0);

            perror("Error with exec pr4");
            exit(6);
            break;
        }
        default: {
            break;
        }
    }

    close(fd[1]);
    close(fd[0]);

    while (wait(NULL) != -1)
        ;
}

int main(int argc, char **argv) {
    func4(argc, argv);

    return 0;
}
