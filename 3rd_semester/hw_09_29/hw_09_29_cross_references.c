/*
    The "cross-reference" table printing program outputs all words in the document
    and specifies for each of them the line numbers where it occurred.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tree {
    int numb_lines; /* number of words encountered */
    char *word;     /* encountered word */
    int *lines;     /* line numbers where the word is encountered */
    struct Tree *left, *right;
    int size;
} Tree;

/* Adding an element to the tree: */
Tree *add_elem(Tree *r, char *tmp_word, int line) {
    if (!r) {
        r = (Tree *)malloc(sizeof(Tree));
        r->size = 1;
        r->lines = (int *)malloc(sizeof(int));
        r->word = malloc(strlen(tmp_word) + 1);
        strcpy(r->word, tmp_word);

        r->numb_lines = 1;
        r->lines[0] = line;
        r->left = NULL;
        r->right = NULL;
        return r;
    }

    /* If that word already exists: */
    if (!strcmp(tmp_word, r->word)) {
        /* If the word is found on a different line:  */
        if (r->lines[r->numb_lines - 1] != line) {
            /* It's time to increase the dimension */
            if (r->size == r->numb_lines) {
                r->size *= 4;
                r->lines = realloc(r->lines, r->size * sizeof(int));
            }
            r->lines[r->numb_lines] = line;
            r->numb_lines++;
        }
    } else if (strcmp(tmp_word, r->word) < 0) { /* if tmp_word < r->word */
        r->left = add_elem(r->left, tmp_word, line);
    } else {
        r->right = add_elem(r->right, tmp_word, line);
    }

    return r;
}

/* Printing elements: */
void print_elem(Tree *t) {
    int i;
    if (t) {
        print_elem(t->left);
        printf("%s: ", t->word);

        for (i = 0; i < t->numb_lines; i++) {
            printf("%d ", t->lines[i]);
        }
        printf("\n");
        print_elem(t->right);
    }
}

/* Deleting a tree: */
void delete_tree(Tree *root) {
    if (!root) return;
    delete_tree(root->left);
    delete_tree(root->right);

    free(root->lines);
    free(root->word);
    free(root);
}

int main(int argc, char **argv) {
    Tree *t = NULL;
    char *tmp_word = NULL;
    char tmp_c;
    int lines = 1;
    int size_word = 1;
    int index_word = 0;

    FILE *file_in;
    if ((file_in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "File '%s' not found\n", argv[2]);
        return 1;
    }

    while ((tmp_c = getc(file_in)) != EOF) {
        /* Creating a word: */
        while ((!isspace(tmp_c)) && (tmp_c != EOF)) {
            if ((index_word + 1) == size_word) {
                size_word *= 2;
                tmp_word = realloc(tmp_word, size_word);
            }

            tmp_word[index_word] = tmp_c;
            index_word++;
            tmp_c = getc(file_in);
        }

        if (tmp_word) {
            tmp_word[index_word] = 0; /* don't forget to set the end of line flag */
            t = add_elem(t, tmp_word, lines);

            free(tmp_word);
            tmp_word = NULL;
            index_word = 0;
            size_word = 1;
        }
        if (tmp_c == '\n') lines++;
    }
    printf("End of reading\n");
    printf("Words:\n");
    print_elem(t);

    delete_tree(t);
    fclose(file_in);
    return 0;
}
