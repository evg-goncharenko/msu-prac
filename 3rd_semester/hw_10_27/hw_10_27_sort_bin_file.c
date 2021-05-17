/*
    The command line specifies the name of the binary file consisting of integers.
    Implementation of the file sorting program "in place",
    that is, without using additional files, dynamic memory, etc.
*/

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void create_bin_file() {
    FILE *f;
    int mas[12] = {12, 222, 6, 11111, 123, 27, 31, 1231, 2355, 1, 0, 22};

    f = fopen("bin", "w");

    for (int i = 0; i < 12; i++) {
        fwrite(&mas[i], sizeof(int), 1, f);
    }

    fclose(f);
}

void print_bin_file() {
    int tmp;
    FILE *f;
    if ((f = fopen("bin", "r")) == NULL) {
        printf("File not found\n");
        return;
    }

    while (fread(&tmp, sizeof(int), 1, f)) {
        printf("%d ", tmp);
    }
    printf("\n");
}

void swap(int *a, int *b) {
    int tmp = *a;
    if (*a > *b) {
        *a = *b;
        *b = tmp;
    }
}

int main(int argc, char **argv) {
    FILE *file;

    int tmp_l, tmp_r;
    long numb_elem;

    create_bin_file();
    print_bin_file();

    if (argc < 2) {
        fprintf(stderr, "Too few arguments command lines\n");
        return 1;
    }

    if ((file = fopen(argv[1], "r+")) == NULL) {
        perror(argv[1]);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    numb_elem = ftell(file);
    numb_elem /= sizeof(int);

    for (int i = 0; i < numb_elem; i++) {
        for (int j = i + 1; j < numb_elem; j++) {
            fseek(file, i * sizeof(int), SEEK_SET);
            fread(&tmp_l, sizeof(int), 1, file);
            fseek(file, j * sizeof(int), SEEK_SET);
            fread(&tmp_r, sizeof(int), 1, file);

            swap(&tmp_l, &tmp_r);

            fseek(file, i * sizeof(int), SEEK_SET);
            fwrite(&tmp_l, sizeof(int), 1, file);
            fseek(file, j * sizeof(int), SEEK_SET);
            fwrite(&tmp_r, sizeof(int), 1, file);
        }
    }

    printf("After sorting:\n");
    print_bin_file();
    return 0;
}
