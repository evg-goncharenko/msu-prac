#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONST_I "-i"
#define CONST_O "-o"

#define INPUT_ERROR 1
#define OUTPUT_ERROR 2

struct Node {
    char *val;      /* word */
    int numb_occur; /* number of occurrences of a word */
    struct Node *left, *right;
} * tree;

/* Print all words stored in the tree: */
void print_bin_tree(struct Node *root) {
    if (!root) return;

    print_bin_tree(root->left);
    printf("val: '%s' - numb_occur: %d\n", root->val, root->numb_occur);
    print_bin_tree(root->right);
}

/* Inserting a new element: */
struct Node *add_elem_bin_tree(struct Node *root, struct Node *r, char *n, int *max_numb_occur) {
    if (!r) {
        r = (struct Node *)malloc(sizeof(struct Node));

        if (!r) {
            fprintf(stderr, "Not enough memory!\n");
            exit(0);
        }
        r->val = malloc(sizeof(n));
        strcpy(r->val, n);
        r->numb_occur = 1;
        r->left = NULL;
        r->right = NULL;

        if (!root) return r; /* the first entrance */

        if (strcmp(n, root->val) < 0) {
            root->left = r; /* n < root->val */
        } else {
            root->right = r; /* n >= root->val */
        }
        return r;
    }

    /* if such a word already exists */
    if (!strcmp(n, root->val)) {
        r->numb_occur++;
        if ((r->numb_occur) > *max_numb_occur) {
            *max_numb_occur = (r->numb_occur);
        }
    } else if (strcmp(n, root->val) < 0) {
        add_elem_bin_tree(r, root->left, n, max_numb_occur); /* n < root->val */
    } else {
        add_elem_bin_tree(r, root->right, n, max_numb_occur); /* n >= root->val */
    }
    return root;
}

/* The final print: */
void print_from_bin_tree(struct Node *root, int max_numb_occur, int total_amount, FILE *file_out) {
    if (!root) return;

    if (root->numb_occur == max_numb_occur) {
        long double frequency = 1.0 * (root->numb_occur) / total_amount;
        fprintf(file_out, "%s %d %Lf\n", root->val, root->numb_occur, frequency);
    }

    print_from_bin_tree(root->left, max_numb_occur, total_amount, file_out);
    print_from_bin_tree(root->right, max_numb_occur, total_amount, file_out);
}

void delete_bin_tree(struct Node *root) {
    if (!root) return;

    delete_bin_tree(root->left);
    delete_bin_tree(root->right);

    root->left = NULL;
    root->right = NULL;
    free(root->val);
    free(root);
}

int main(int argc, char **argv) {
    struct Node *tree = NULL;
    long total_amount = 0;   /* total number of words */
    int max_numb_occur = -1; /* maximum number of occurrences of a word */

    int flag_i = 0;   /* flag for the '-i' option */
    int flag_o = 0;   /* flag for the '-o' option */
    int out_position; /* index of the file_out name in argv */

    FILE *file_in;
    FILE *file_out;

    /* Checking for options: */
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], CONST_I)) flag_i = 1;

        if (!strcmp(argv[i], CONST_O)) {
            flag_o = 1;
            out_position = i + 1;
        }
    }

    /* Initializing input: */
    if (flag_i) {
        if ((file_in = fopen(argv[2], "r")) == NULL) {
            fprintf(stderr, "File '%s' not found\n", argv[2]);
            return INPUT_ERROR;
        }
        printf("file '%s' reading:\n", argv[2]);
    } else {
        file_in = stdin;
        printf("stdin reading:\n");
    }

    /* Initializing output: */
    if (flag_o) {
        if ((file_out = fopen(argv[out_position], "w")) == NULL) {
            fprintf(stderr, "File '%s' not found\n", argv[out_position]);
            return OUTPUT_ERROR;
        }
        printf("file '%s' writing:\n", argv[out_position]);
    } else {
        file_out = stdout;
        printf("stdout writing:\n");
    }

    /* Adding elements to the tree: */
    char *tmp = NULL; /* word to add */
    int tmp_size = 0; /* maximum size of the tmp array */
    int tmp_numb = 0; /* number of characters in the tmp array */
    char c;

    do {
        c = fgetc(file_in);
        if ((c != '\n') && (c != ' ') && (c != EOF) && isalpha(c)) {
            /* Need to increase the dimension: */
            if (tmp_numb == tmp_size) {
                tmp = realloc(tmp, 2 * tmp_size + 1);
                tmp_size = 2 * tmp_size + 1;
            }
            tmp[tmp_numb] = c;
            tmp_numb++;
        } else if (tmp_numb) {
            /* It's time to add a non-empty word to the tree */
            tmp[tmp_numb] = 0;
            tree = add_elem_bin_tree(tree, tree, tmp, &max_numb_occur);
            free(tmp);
            tmp = NULL;
            tmp_size = tmp_numb = 0;
            total_amount++;
        }

        /* Adding words like: ".", "-", "!", "?" ... */
        if (!isalpha(c) && (c != '\n') && (c != ' ') && (c != EOF)) {
            tree = add_elem_bin_tree(tree, tree, &c, &max_numb_occur);
        }
    } while (c != EOF);

    /* Print to file_out: */
    while (max_numb_occur > 0) {
        print_from_bin_tree(tree, max_numb_occur, total_amount, file_out);
        max_numb_occur--;
    }

    fclose(file_in);
    fclose(file_out);

    delete_bin_tree(tree);
    return 0;
}
