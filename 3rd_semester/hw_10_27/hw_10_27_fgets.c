/*
    Implementation analog of the fgets() function:
    char * mygets (int fd, char * pBuffer, size_t size);
*/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 25

char *mygets(int fd, char *pBuffer, size_t size) {
    int pos = 0;
    char c_tmp;
    ssize_t n;

    /* While we can read and the number of read < size-1: */
    while (((n = read(fd, &c_tmp, sizeof(char))) > 0) && (size > 1)) {
        pBuffer[pos] = c_tmp;

        if (c_tmp == '\n') {
            pBuffer[pos + 1] = 0;
            break;
        }

        size--;
        pos++;
    }

    /*
        If all size-1 characters were counted, but there isn't end of the line
        or the end of the line is met:
     */
    if ((size == 1) || !n) {
        pBuffer[pos] = 0;
    }

    /* An error occurred: */
    if (n == -1) exit(errno);

    /*
        If none of the characters could be read, the function doesn't change
        the contents of the array, but returns NULL:
    */
    if (!pos) return NULL;

    return pBuffer;
}

int main(int argc, char **argv) {
    int file;
    if ((file = open(argv[1], O_RDONLY)) == -1) {
        perror(argv[1]);
        exit(1);
    }
    char *tmp = NULL;
    tmp = (char *)malloc(BUF_SIZE * sizeof(char));

    char buf[BUF_SIZE];

    /* snprintf() better than strcpy(): */
    snprintf(tmp, BUF_SIZE, "%s", mygets(file, buf, BUF_SIZE));
    printf("%s", tmp);

    if (tmp) free(tmp);
    tmp = NULL;
    return 0;
}
