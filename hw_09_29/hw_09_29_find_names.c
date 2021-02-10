/*
    The program reads the text of C program and prints in alphabetical
    order all groups of variable names in which the first n = 6 characters match,
    but the subsequent ones differ in some way.
    Doesn't process the inside of quoted strings and comments.

    Run: ./run test.c 6

    The complexity of the problem increases, because there are correct
    constructions of the form:
        1) "int (i);"
        2) "int
        i
        ;"
        3) "int main(){}"

    "#define" and "typedef" are not considered
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE "define"
#define INCLUDE "include"
#define TYPEDEF "typedef"

typedef struct Node {
    struct Node *prev;
    struct Node *next;
    char *elem; /* variable name */
} Node;

/* Variable name list: */
Node name_list;

/*
    Checking the system word:
    result: 0 - if there is
*/
int is_system(char *word, char **system_words, int numb_system_words) {
    for (int i = 0; i < numb_system_words; i++) {
        if (!strcmp(word, system_words[i])) {
            return 1;
        }
    }
    return 0;
}

/* Creating an array of system words: */
void create_words(char ***system_words, int *numb_system_words) {
    *numb_system_words = 10;
    *system_words = (char**)malloc(*numb_system_words * sizeof(char*));
    (*system_words)[0] = (char*) calloc(4, sizeof(char)); strcpy((*system_words)[0], "int");
    (*system_words)[1] = (char*) calloc(5, sizeof(char)); strcpy((*system_words)[1], "long");
    (*system_words)[2] = (char*) calloc(5, sizeof(char)); strcpy((*system_words)[2], "char");
    (*system_words)[3] = (char*) calloc(6, sizeof(char)); strcpy((*system_words)[3], "float");
    (*system_words)[4] = (char*) calloc(7, sizeof(char)); strcpy((*system_words)[4], "double");
    (*system_words)[5] = (char*) calloc(5, sizeof(char)); strcpy((*system_words)[5], "void");
    (*system_words)[6] = (char*) calloc(7, sizeof(char)); strcpy((*system_words)[6], "struct");
    (*system_words)[7] = (char*) calloc(8, sizeof(char)); strcpy((*system_words)[7], "include");
    (*system_words)[8] = (char*) calloc(7, sizeof(char)); strcpy((*system_words)[8], "string");
    (*system_words)[9] = (char*) calloc(7, sizeof(char)); strcpy((*system_words)[9], "return");
}

/* Inserting a variable name in the list: */
Node *push_elem(Node *name_list, char *word) {
    int flag_ins = 0; /* insertion test */

    /* If the list was empty: */
    if (!name_list) {
        Node *new_word = (Node*)malloc(sizeof(Node));
        new_word->elem = (char*)malloc((strlen(word) + 1) * sizeof(char));
        strcpy(new_word->elem, word);
        new_word->next = NULL;
        new_word->prev = NULL;
        return new_word;
    }
    
    struct Node *tmp = name_list;
    do {
        /* If inserted name is larger than the current one, then go next: */
        if (((strcmp(word, tmp->elem) > 0))) {
            if (tmp->next) {
                tmp = tmp->next;
            } else {
                break;
            }
        } else if (!strcmp(word, tmp->elem)) { /* this name already exists: */
            return name_list;
        } else {
            Node *new_word = (Node*)malloc(sizeof(Node));
            new_word->elem = malloc((strlen(word) + 1) * sizeof(char));
            strcpy(new_word->elem, word);
            new_word->next = tmp;
            
            /* If there is a previous list item: */
            if(tmp->prev) {
                new_word->prev = tmp->prev;
                new_word->next = tmp;
                new_word->prev->next = new_word;
                tmp->prev = new_word;
                while (tmp->prev) tmp = tmp->prev;
            } else {
                new_word->prev = NULL;
                tmp->prev = new_word;
                new_word->next = tmp;
                return new_word;
            }
            flag_ins = 1;
            break;
        }
    } while (tmp);
    
    /* Insert a name at the end: */
    if(!flag_ins) {
        Node *new_word = (Node*)malloc(sizeof(Node));
        new_word->elem = malloc(sizeof(word));
        strcpy(new_word->elem, word);
        new_word->next = NULL;
        tmp->next = new_word;
        new_word->prev = tmp;
    }
    return name_list;
}

/*
    Check that the first n characters in word1 and word2 names match:
    (it is guaranteed that the lengths of word1 and word2 >= n)
*/
int correct_words(char *word1, char *word2, int n) {
    int res = 1;
    for (int i = 0; i < n; i++) {
        if (word1[i] != word2[i]) res = 0;
    }

    return res;
}

/*
    Print the names you are looking for:
    (no repetitive words guaranteed)
*/
void print_list(Node *l, int n) {
    int find = 0;
    printf("The desired variable names:\n");
    while (l->next) {
        if (correct_words(l->elem, l->next->elem, n)) {
            find = 1;
            printf("%s, %s; ", l->elem, l->next->elem);
        }
        l = l->next;
    }
    if (!find) {
        printf("There are no satisfying names.\n");
    } else {
        printf("\n");
    }

}

/* Deleting a list: */
void delete_list(Node *l) {
    Node *tmp;
    while (l) {
        tmp = l;
        l = l->next;
        free(tmp->elem);
        free(tmp);
    }
}

/* Recursive deleting a list: */
void delete_list_recur(Node *L) {
    if (L != NULL) {
        free(L->elem);
        delete_list_recur(L->next);
        free(L);
    }
}

int main(int argc, char **argv) {
    int numb_system_words;      /* number of system words */
    char **system_words = NULL; /* array of system words */

    create_words(&system_words, &numb_system_words);

    char tmp_c;
    char *word = NULL;
    int word_size = 1;
    int word_index = 0;
    Node *list = NULL;

    FILE *file_in;
    if ((file_in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "File '%s' not found \n", argv[1]);
        return 1;
    }
    
    if (!argv[2]) {
        fprintf(stderr, "Number N is not set \n");
        return 2;
    }
    
    int n; /* checking the first n characters by condition */
    sscanf(argv[2], "%d", &n);
    tmp_c = fgetc(file_in);
    
    while (tmp_c != EOF) {
        /* Processing comments: */
        if (tmp_c == '/') {
            tmp_c = fgetc(file_in);
            
            /* If a comment of the form is encountered: '//': */
            if (tmp_c == '/') {
                while (tmp_c != '\n' && tmp_c != EOF) {
                    tmp_c = fgetc(file_in);
                }
            } else if (tmp_c == '*') { /* if a 'classic' comment is encountered */
                while ((tmp_c = fgetc(file_in)) != EOF) {
                    if (tmp_c == '*') {
                        if ((tmp_c = fgetc(file_in)) == '/') {
                            tmp_c = fgetc(file_in);
                            break;
                        }
                    }
                }
            }
            continue;
        }
        
        /* Handling of double quotes: */
        if (tmp_c == '"') {
            tmp_c = fgetc(file_in);
            while (tmp_c != '"' && tmp_c != EOF) {
                tmp_c = fgetc(file_in);
            }
        }
        
        /* Creating a temporary word: */
        if (!isspace(tmp_c)) {
            if (isalpha(tmp_c)) {
                while (isalpha(tmp_c) || tmp_c == '_' || isdigit(tmp_c)) {
                    if ((word_index+1) == word_size) {
                        word_size *= 2;
                        word = realloc(word, word_size);
                    }
                    word[word_index] = tmp_c;
                    tmp_c = fgetc(file_in);
                    word_index++;
                }
                word[word_index] = 0;
            }
            if (word) {
                /* If "#include", "#define" or "typedef" is encountered" */
                if(!strcmp(word, INCLUDE) || !strcmp(word, DEFINE)
                   || !strcmp(word, TYPEDEF)) {
                    while ((tmp_c != '\n') && (tmp_c != EOF)) {
                        tmp_c = fgetc(file_in);
                    }
                } else {
                    /* If the system word is encountered: */
                    if (is_system(word, system_words, numb_system_words)) {
                        free(word);
                        word = NULL;
                        word_index = 0;
                        word_size = 1;
                        continue;
                    }
                    
                    /* Skip spaces: */
                    while (tmp_c == ' ') {
                        tmp_c = fgetc(file_in);
                    }
                    
                    /* If '('is encountered, then we have come across the function name: */
                    if ((tmp_c != '(') && (strlen(word) >= n)) {
                        list = push_elem(list, word);
                    }
                }
                /* Clearing a word: */
                free(word);
                word = NULL;
                word_index = 0;
                word_size = 1;
                continue;
            }
        }
        tmp_c = fgetc(file_in);
    }
    print_list(list, n);
    delete_list(list);
    
    /* System word memory cleaning: */
    for (int i = 0; i < numb_system_words; i++) {
        free(system_words[i]);
    }
    free(system_words);
    system_words = NULL;
    fclose(file_in);
    return 0;
}
