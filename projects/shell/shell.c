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
    int spec_symb;  /* 1 - ">", ";", '|", ">>", etc */
    char *elem;
    struct node *next;
} node;

typedef struct tree
{
    int read;       /* -1 default */
    int write;      /* -1 default */
    node *key;
    struct tree *right;
    struct tree *left;
} tree;

int flag_single_quotes = 0;
int flag_double_quotes = 0;
int flag_apostrophe = 0;
int flag_eof = 0;
int flag_new_line = 0;
int flag_spec_char = 0; /* flag for adding special characters to a separate cell of the array */
int flag_is_command = 0;
int flag_is_spec_symb = 0;
int flag_exec_in_background = 0; /* 1 - if in the background */

const char special_characters[5] = "&|<>\0";

tree *main_tree = NULL;
node *main_list = NULL;
node *processed_proc = NULL;
node *deleted_proc = NULL;

void add_to_sequence_of_quotes(char **sequence, int *index, int *flag, int tmp_c, int end_symbol)
{
    if(tmp_c == end_symbol)
    {
        *flag = 0;
    }
    else
    {
        *sequence = (char*)realloc(*sequence, (*index + 2) * sizeof(char));
        (*sequence)[*index] = tmp_c;
        (*index)++;
    }
}

char *read_word()
{
    flag_is_command = 0;
    flag_is_spec_symb = 0;
    char tmp_c = 0;
    int i = 0;
    char *word = (char*)malloc(sizeof(char));
    word[i] = 0; /* if EOF is immediately encountered */
    
    while((tmp_c = getchar()) != EOF)
    {
        if(flag_single_quotes)
        {
            add_to_sequence_of_quotes(&word, &i, &flag_single_quotes, tmp_c, '\'');
        }
        else if(flag_double_quotes)
        {
            add_to_sequence_of_quotes(&word, &i, &flag_double_quotes, tmp_c, '"');
        }
        else if(flag_apostrophe)
        {
            add_to_sequence_of_quotes(&word, &i, &flag_apostrophe, tmp_c, '`');
        }
        else if(flag_spec_char)
        {
            flag_spec_char = 0;
            if(tmp_c == word[i-1])
            {
                word = (char*)realloc(word, (i + 2) * sizeof(char));
                word[i] = tmp_c;
                i++;
                word[i] = 0;
                return word;
            }
            else
            {
                ungetc(tmp_c, stdin);
                word[i] = 0;
                return word;
            }
        }
        else if(isspace(tmp_c))
        {
            if(flag_is_command) /* command hasn't already formed */
            {
                if(tmp_c == '\n')
                {
                    flag_new_line = 1;
                }
                word[i] = 0;
                return word;
            }
            else /* there hasn't generated command yet */
            {
                if(tmp_c == '\n')
                {
                    flag_new_line = 1;
                    free(word);
                    return NULL;
                }
                continue; /* ignore it and move on */
            }
        }
        else if(tmp_c == '\'')
        {
            flag_is_command = 1;
            flag_single_quotes = 1;
        }
        else if(tmp_c == '"')
        {
            flag_is_command = 1;
            flag_double_quotes = 1;
        }
        else if(tmp_c == '`')
        {
            flag_is_command = 1;
            flag_apostrophe = 1;
        }
        else if(strchr(special_characters, tmp_c)) /* if characters are encountered: '&', '|', '<', '>' */
        {
            if(flag_is_command)
            {
                ungetc(tmp_c, stdin); /* making a return to the stream */
                word[i] = 0;
                return word;
            }
            else /* if the team doesn't exist yet */
            {
                flag_spec_char = 1;
                flag_is_spec_symb = 1;
                word = (char*)realloc(word, (i + 2) * sizeof(char));
                word[i] = tmp_c;
                i++;
            }
        }
        else if(tmp_c == ';')
        {
            if(flag_is_command) /* '; ' at the end of the command */
            {
                ungetc(tmp_c, stdin); /* making a return to the stream */
                word[i] = 0;
                return word;
            }
            else /* after ungetc (), the new command isn't created */
            {
                word = (char*)realloc(word, (i + 2) * sizeof(char));
                word[i] = tmp_c;
                i++;
                flag_is_spec_symb = 1; /* for example, word[0] = ';' */
                word[i] = 0;
                return word;
            }
        }
        else /* just add to the word */
        {
            flag_is_command = 1; /* command started */
            word = (char*)realloc(word, (i + 2) * sizeof(char));
            word[i] = tmp_c;
            i++;
        }
    }
    
    flag_eof = 1;
    if(!strlen(word))
    {
        free(word);
        return NULL;
    }
    else
    {
        word[i] = 0;
        return word;
    }
}

void delete_array(char **arr)
{
    int i = 0;
    while(arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr[i]);
    free(arr);
}

void delete_list(node *l)
{
    if(l)
    {
        delete_list(l->next);
        free(l->elem);
        free(l);
    }
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

void print_kill_proc(node *l)
{
    if(l)
    {
        printf("Process '%s' was killed (pid: %d)\n", l->elem, l->spec_symb);
        print_kill_proc(l->next);
    }
    return;
}

void kill_all_processes()
{
    node *tmp = processed_proc;
    int status;
    int pid;
    while(tmp)
    {
        pid = tmp->spec_symb;
        printf("Killing process: %s \t [pid:%d]\n", tmp->elem, tmp->spec_symb);
        kill(pid, 9);
        waitpid(pid, &status, 0);
        tmp = tmp->next;
    }
    delete_list(processed_proc);
    return;
}

node *add_to_list(node *l, char *str)
{
    if (!l)
    {
        node *tmp = (node*)malloc(sizeof(node));
        tmp->elem = (char*)malloc((strlen(str) + 1) * sizeof(char));
        tmp->spec_symb = flag_is_spec_symb;
        strcpy(tmp->elem, str);
        tmp->next = NULL;
        return tmp;
    }
    l->next = add_to_list(l->next, str);
    return l;
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

node *proc_list(node *l, int *rd, int *wr)
{
    node* res = NULL;
    while(l)
    {
        if(l->spec_symb == 1)
        {
            if(!strcmp(l->elem, ">>"))
            {
                if(*wr != -1) /* if it hasn't been closed yet */
                {
                    close(*wr);
                }
                *wr = open(l->next->elem, O_WRONLY | O_CREAT | O_APPEND, 0777);
                l = l->next;
            }
            else if(!strcmp(l->elem, ">"))
            {
                if(*wr != -1)
                {
                    close(*wr);
                }
                *wr = open(l->next->elem, O_WRONLY | O_CREAT | O_TRUNC, 0777);
                l = l->next;
            }
            else if(!strcmp(l->elem, "<"))
            {
                if(*rd != -1)
                {
                    close(*rd);
                }
                *rd = open(l->next->elem, O_RDONLY);
                l = l->next;
            }
        }
        else
        {
            flag_is_spec_symb = l->spec_symb;
            res = add_to_list(res, l->elem);
        }
        l = l->next;
    }
    return res;
}

/* fl - '1' if you don't need to work with the file and '0' if you need to */
tree *make_tree(node *l, int rd, int wr, int fl)
{
    int new_rd;
    int new_wr;
    tree * res = NULL;
    node * fend = NULL;
    node * fstart = l;
    node * sstart = NULL;
    node * parse;
    node * tmp = l;
    
    if(!l)
    {
        return NULL;
    }
    while(tmp->next)
    {
        if(!(strcmp(tmp->next->elem, ";")))
        {
            fend = tmp;
        }
        else if(!(strcmp(tmp->next->elem, "||")) || !(strcmp(tmp->next->elem, "&&")))
        {
            if(!fend || strcmp(fend->next->elem, ";"))
            {
                fend = tmp;
            }
        }
        else if(!(strcmp(tmp->next->elem, "|")))
        {
            if(!fend || !strcmp(fend->next->elem, "|"))
            {
                fend = tmp;
            }
        }
        
        tmp = tmp->next;
    }
    
    if(fend)
    {
        res = (tree*)malloc(sizeof(tree));
        res->key = fend->next;
        fend->next = NULL;
        sstart = res->key->next;
        res->key->next = NULL;
        
        res->read = rd;
        res->write = wr;
        
        res->left = make_tree(fstart, rd, wr, 0);
        res->right = make_tree(sstart, rd, wr, 0);
        
        if(rd != -1)
        {
            close(rd);
        }
        if(wr != -1)
        {
            close(wr);
        }
        return res;
    }
    
    if(!fl) /* file */
    {
        if(rd != -1)
        {
            new_rd = dup(rd);
        }
        else
        {
            new_rd = -1;
        }
        if(wr != -1)
        {
            new_wr = dup(wr);
        }
        else
        {
            new_wr = -1;
        }
        parse = proc_list(l, &new_rd, &new_wr);
    }
    else
    {
        parse = proc_list(l, &rd, &wr);
        new_rd = rd;
        new_wr = wr;
    }
    
    if(l) delete_list(l);
    
    res = (tree*)malloc(sizeof(tree));
    res->key = parse;
    res->read = new_rd;
    res->write = new_wr;
    res->left = NULL;
    res->right = NULL;
    return res;
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

int work(node *l, int rd, int wr, int *exit_code)
{
    char **argv;
    argv = list_to_array(l);
    
    if(is_cd(argv))
    {
        return 1;
    }
    
    int pid;
    if((pid = fork()) == -1)
    {
        perror("Error with fork()");
        if(rd != -1)
        {
            close(rd);
        }
        if(wr != -1)
        {
            close(wr);
        }
        return -1;
    }
    if(!pid)
    {
        if(rd != -1)
        {
            dup2(rd, 0);
            close(rd);
        }
        if(wr != -1)
        {
            dup2(wr, 1);
            close(wr);
        }
        if(flag_exec_in_background)
        {
            if(rd != -1)
            {
                rd = open("/dev/null", O_RDONLY);
                dup2(rd,0);
            }
            if(wr != -1)
            {
                wr = open("/dev/null", O_WRONLY);
                dup2(wr,1);
            }
        }
        execvp(*argv,argv);
        perror("Incorrect command");
        exit(2);
    }
    else
    {
        if(!flag_exec_in_background)
        {
            int status = 0;
            wait(&status);
            
            if(WIFEXITED(status))
            {
                *exit_code = WEXITSTATUS(status);
            }
            if(rd != -1)
            {
                close(rd);
            }
            if(wr != -1)
            {
                close(wr);
            }
        }
        else
        {
            flag_is_spec_symb = pid;
            processed_proc = add_to_list(processed_proc, argv[0]);
            
            if(rd != -1)
            {
                close(rd);
            }
            if(wr != -1)
            {
                close(wr);
            }
        }
    }
    
    delete_array(argv);
    return pid;
}

int work_tree(tree *t, int p)
{
    if(!t)
    {
        return 1;
    }
    int right_tr = 0, left_tr = 0;
    
    if(!strcmp(t->key->elem, "||"))
    {
        left_tr = work_tree(t->left, p);
        if(left_tr == -1)
        {
            right_tr = work_tree(t->right, p);
        }
        if(right_tr == -1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else if(!strcmp(t->key->elem, "&&"))
    {
        left_tr = work_tree(t->left, p);
        if(left_tr != -1)
        {
            right_tr = work_tree(t->right, p);
        }
        else
        {
            return -1;
        }
        
        if(right_tr == -1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else if(!strcmp(t->key->elem, "|"))
    {
        int fd[2];
        pipe(fd);
        if(t->left->write != -1)
        {
            close(t->left->write);
            t->left->write = -1;
        }
        if(t->right->read != -1)
        {
            close(t->right->read);
            t->right->read = -1;
        }
        
        if(!p)
        {
            t->left->write = fd[1];
            t->right->read = fd[0];
            work_tree(t->left, 1);
            work_tree(t->right, 0);
        }
        else
        {
            t->left->write = fd[1];
            t->right->read = fd[0];
            t->right->write = t->write;
            work_tree(t->left, 1);
            work_tree(t->right, 0);
        }
        
        return 1;
    }
    else if(!strcmp(t->key->elem, ";"))
    {
        work_tree(t->left, p);
        right_tr = work_tree(t->right, p);
        if(right_tr == -1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if(!strcmp(t->key->elem, "exit"))
        {
            print_kill_proc(deleted_proc);
            delete_list(deleted_proc);
            kill_all_processes();
            delete_tree(t);
            printf("End of the program\n");
            exit(0);
        }
        int exit_code = 0;
        work(t->key, t->read, t->write, &exit_code);
        if(exit_code == 2)
        {
            return -1;
        }
    }
    return 1;
}

void check_ampersand(node *l)
{
    node *tmp;
    tmp = l;
    while(tmp && strcmp((tmp->elem), "&")) /* until we meet them "&" */
    {
        l = tmp;
        tmp = tmp->next;
    }
    if(tmp)
    {
        if(tmp->next) /* because a single "&" can only be at the end */
        {
            perror("& ERROR!");
            flag_exec_in_background = -1;
        }
        else
        {
            flag_exec_in_background = 1;
            free(tmp);
            l->next = NULL;
        }
    }
    return;
}

void quotes_error(node *l)
{
    delete_list(l);
    perror("Incorrect sequence of quotes");
}

int main(int argc, char **argv)
{
    char *word = NULL;
    int is_correct_quotes = 1;
    while(!flag_eof)
    {
        printf("%s> %s ", COLOR_RED, RESET); /* color shell */
        main_list = NULL;
        main_tree = NULL;
        while(!flag_eof && !flag_new_line)
        {
            is_correct_quotes = 1;
            word = read_word();
            if(word)
            {
                main_list = add_to_list(main_list, word);
                free(word);
            }
        }
        
        if(flag_single_quotes || flag_double_quotes || flag_apostrophe)
        {
            is_correct_quotes = 0;
            quotes_error(main_list);
            
            flag_single_quotes = 0;
            flag_double_quotes = 0;
            flag_apostrophe = 0;
            
            delete_list(main_list);
            continue;
        }
        
        if(!flag_eof && is_correct_quotes && main_list && (flag_exec_in_background != -1))
        {
            check_ampersand(main_list);
            main_tree = make_tree(main_list, -1, -1, 1);
            work_tree(main_tree, 0);
            delete_tree(main_tree);
        }
        
        print_kill_proc(deleted_proc);
        delete_list(deleted_proc);
        deleted_proc = NULL;
        
        flag_exec_in_background = 0;
        flag_new_line = 0;
    }
    return 0;
}
