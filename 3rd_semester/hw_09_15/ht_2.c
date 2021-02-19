#include <stdio.h>
#include <stdlib.h>

/* The program should output the longest string */
int main() {
    /* Numb - length of the current string, ch - current character */
    int max_len = 0, numb = 0, size = 0, ch = 0, i;
    char *tmp = NULL; /* current line */
    char *res = NULL; /* the longest line */
    
    while (ch != EOF) {
        while (((ch = getchar()) != '\n') && (ch != EOF)) {
            if (numb == size) {
                tmp = realloc(tmp, 2 * size + 1);
                size = 2 * size + 1;
            }
            tmp[numb] = ch;
            numb++;
        }
        if (numb > max_len) {
            max_len = numb;
            res = realloc(res, size);
            for (i = 0; i < numb; i++) {
                res[i] = tmp[i];
            }
        }
        if (tmp != NULL) {
            free(tmp);
            tmp = NULL;
        }
        numb = size = 0;
    }
    
    if (res != NULL) {
        printf("\nThe longest line: ");
        for (i = 0; i < max_len; i++) {
            printf("%c", res[i]);
        }
        printf("\n");
        free(res);
    } else {
        printf("\nNo lines\n");
    }
    return 0;
}
