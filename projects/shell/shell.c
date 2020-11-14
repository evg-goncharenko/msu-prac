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

#define COLOR_RED "\e[1;31m"
#define RESET "\e[m"

typedef struct node
{
    char *elem;
    struct node *next;
} node;

typedef struct tree
{
    node *key;
    struct tree *right;
    struct tree *left;
} tree;

int flag_quotes = 0;
int flag_eof = 0;
int flag_command = 0;

tree *t = NULL;
node *l = NULL;

node *add_to_list(node *head, char *str)
{
    if(!head)
    {
        head = (node*) malloc(sizeof(node));
        head->next = NULL;
        head->elem = str;
        return head;
    }
    node *last = (node*) malloc(sizeof(node));
    node *tmp = head;
    
    while(tmp->next)
    {
        tmp = tmp->next;
    }
    
    last->elem = str;
    last->next = NULL;
    tmp->next = last;
    
    return head;
}

char *read_word()
{
    char *word = NULL;
    int word_size = 1;
    int word_index = 0;
    char tmp_c;
    
    while((tmp_c = getchar()) != EOF)
    {
        
        if(flag_quotes)
        {
            if(tmp_c == '"')
            {
                flag_quotes = 0;
            }
            else
            {
                if((word_index + 1) == word_size)
                {
                    word_size *= 2;
                    word = (char*) realloc(word, word_size);
                }
                word[word_index] = tmp_c;
                word_index++;
            }
        }
        else if(tmp_c == '"')
        {
            flag_quotes = 1;
        }
        else if(isspace(tmp_c))
        {
            if(tmp_c == '\n')
            {
                flag_command = 1;
            }
            if(word) word[word_index] = 0;
            return word;
        }
        else
        {
            if((word_index + 1) == word_size)
            {
                word_size *= 2;
                word = realloc(word, word_size);
            }
            word[word_index] = tmp_c;
            word_index++;
        }
    }
    
    if(word)
    {
        word[word_index] = 0;
    }
    
    flag_eof = 1;
    return word;
}

void print_list(node *l)
{
    while(l)
    {
        printf("%s ", l->elem);
        l = l->next;
    }
}

void delete_list(node *l)
{
    node *tmp;
    
    while(l)
    {
        tmp = l;
        l = l->next;
        free(tmp->elem);
        free(tmp);
    }
}

tree *make_tree(node *l)
{
    tree *res = (tree*)malloc(sizeof(tree));
    
    if(!l)
    {
        return NULL;
    }
    
    res->key = l;
    res->left = NULL;
    res->right = NULL;
    
    return res;
}

void delete_tree(tree *t)
{
    if(t)
    {
        delete_tree(t->left);
        delete_tree(t->right);
        delete_list(t->key);
        free(t);
    }
}

char **list_to_array(node *l)
{
    char **res = NULL;
    int i = 0;
    
    while(l)
    {
        res = (char**)realloc(res, sizeof(char*) * (i + 2));
        res[i] = (char*)malloc(sizeof(char) * (strlen(l->elem) + 1));
        strcpy(res[i], l->elem);
        i++;
        l = l->next;
    }
    res[i] = NULL;
    
    return res;
}

void delete_array(char **arr)
{
    int i = 0;
    while(arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

int is_cd(char **arr)
{
    if(!strcmp(arr[0], "cd"))
    {
        if(arr[1] == NULL)
        {
            chdir(getenv("HOME"));
        }
        else if((arr[2] != NULL) || chdir(arr[1]))
        {
            printf("Error in changing directory\n");
            return -1;
        }
        return 1;
    }
    
    return 0;
}


void command_execution(char **arr, node *l)
{
    int pid;
    pid = fork();
    
    if(pid == -1)
    {
        fprintf(stderr, "Process creation error\n");
    }
    else if(!pid)
    {
        execvp(*arr, arr);
        perror("Invalid command\n");
        free(arr);
        delete_tree(t);
        
        exit(1);
    }
   
    while(wait(NULL) != -1);
}

void work(node *l)
{
    char **arr = list_to_array(l);
    if(!is_cd(arr))
    {
        command_execution(arr, l);
    }
    
    delete_array(arr);
}

void work_tree(tree *t)
{
    if(!t)
    {
        return;
    }
    
    /* TODO: recursive work */
    
    if(!strcmp(t->key->elem, "exit"))
    {
        delete_tree(t);
        printf("End of the program\n");
        exit(0);
    }
    
    work(t->key);
}

int main(int argc, char **argv)
{
    char *word = NULL;
    
    while(!flag_eof)
    {
        printf("%s> %s", COLOR_RED, RESET);
        flag_command = 0;
        l = NULL;
        t = NULL;
        while(!flag_eof && !flag_command)
        {
            word = read_word();
            if(word)
            {
                l = add_to_list(l, word);
            }
        }
        
        if(flag_quotes)
        {
            flag_quotes = 0;
            delete_list(l);
            fprintf(stderr, "\nOdd quotes\n");
            continue;
        }
        
        if(!flag_eof && l)
        {
            t = make_tree(l);
            work_tree(t);
        }
        delete_tree(t);
    }
    return 0;
}
