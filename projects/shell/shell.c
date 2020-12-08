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

#define ERROR_CODE 0
#define SUCCESS_CODE 1

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
const char separate_special_characters[4] = ";()\0";

tree *main_tree = NULL;
node *main_list = NULL;
node *processed_proc = NULL; /* processes processed in the background */
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
                    word = NULL;
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
        else if(strchr(separate_special_characters, tmp_c))
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
    if(word != NULL)
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
    if(arr == NULL)
    {
        return;
    }
    
    int i = 0;
    while(arr[i] != NULL)
    {
        free(arr[i]);
        arr[i] = NULL;
        i++;
    }
    free(arr[i]);
    arr[i] = NULL;
    free(arr);
    arr = NULL;
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
    if(l != NULL)
    {
        if(l->next != NULL)
        {
            delete_list(l->next);
        }
        if(l->elem != NULL)
        {
            free(l->elem);
            l->elem = NULL;
        }
        free(l);
        l = NULL;
    }
}

void delete_tree(tree *t)
{
    if(t != NULL)
    {
        if(t->left != NULL)
        {
            delete_tree(t->left);
        }
        if(t->right != NULL)
        {
            delete_tree(t->right);
        }
        delete_list(t->key);
        t->key = NULL;
        free(t);
        t = NULL;
    }
}


void delete_all_struct()
{
    delete_tree(main_tree);
    main_tree = NULL;
    
    print_list(main_list);
    
    delete_list(main_list);
    main_list = NULL;
    delete_list(processed_proc);
    processed_proc = NULL;
    delete_list(deleted_proc);
    deleted_proc = NULL;
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
        printf("Killing process: %s [pid:%d]\n", tmp->elem, tmp->spec_symb);
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

node *pop_end(node *l)
{
    node *ret = l;
    node *tmp;
    
    if(!l)
    {
        return NULL;
    }
    if(!(l->next))
    {
        return NULL;
    }
    
    do{
        tmp = l;
        l = l->next;
    }while(l->next);
    
    free(l->elem);
    free(l);
    tmp->next = NULL;
    
    return ret;
}

node *pop_start(node *l)
{
    node *tmp = l;
    
    l = l->next;
    free(tmp->elem);
    free(tmp);
    
    return l;
}

node *proc_list(node *l, int *rd, int *wr)
{
    int sk = 0;
    node* res = NULL;
    while(l)
    {
        int fl = 1;
        
        if(!strcmp(l->elem, "("))
        {
            fl = 0;
            ++sk;
        }
        else if (!strcmp(l->elem, ")"))
        {
            fl = 0;
            --sk;
        }
        
        if((l->spec_symb == 1) && !sk && fl)
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
    int sk = 0;
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
        if (!strcmp(tmp->elem, "("))
        {
            sk++;
        }
        else if (!strcmp(tmp->elem, ")"))
        {
            sk--;
        }
        if (!sk)
        {
            
            if(!(strcmp(tmp->next->elem, ";")))
            {
                fend = tmp;
            }
            else if(!(strcmp(tmp->next->elem, "&")))
            {
                if(!fend || strcmp(fend->next->elem, ";"))
                {
                    fend = tmp;
                }
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
    
    if(l)
    {
        delete_list(l);
        l = NULL;
    }
    
    if (!strcmp(parse->elem, "("))
    {
        parse = pop_end(parse);
        parse = pop_start(parse);
        return make_tree(parse, new_rd, new_wr, 1);
    }
    
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

int work(node *l, int rd, int wr)
{
    char **argv;
    argv = list_to_array(l);
    
    if(is_cd(argv))
    {
        return SUCCESS_CODE;
    }
    
    int pid = fork();
    
    if(pid < 0)
    {
        perror("Error with fork()");
        delete_list(l);
        delete_array(argv);
        l = NULL;
        argv = NULL;
        
        if(rd != -1)
        {
            close(rd);
        }
        if(wr != -1)
        {
            close(wr);
        }
        return ERROR_CODE;
    }
    else if(pid == 0)
    {
        if(!flag_exec_in_background)
        {
            signal(SIGINT, SIG_DFL);
        }
        
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
        execvp(*argv, argv);
        perror("Incorrect command");
        
        delete_array(argv);
        delete_all_struct();
        
        exit(0);
    }
    else
    {
        int status = 0;
        int result_status = 0;
        
        if(!flag_exec_in_background)
        {
            waitpid(pid, &status, 0);
            result_status = (WIFEXITED(status) && !WEXITSTATUS(status));
            
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
            
            /* Add the command in the background to the list: */
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
        
        delete_array(argv);
        argv = NULL;
        return result_status;
    }
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
    else if(!strcmp(t->key->elem, "&"))
    {
        flag_exec_in_background = 1;
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
        //t->left->write = t->write;
        work_tree(t->left, p);
        //t->right->read = t->read;
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
            flag_eof = 1;
            print_kill_proc(deleted_proc);
            delete_list(deleted_proc);
            deleted_proc = NULL;
            kill_all_processes();
            delete_tree(t);
            t = NULL;
            printf("End of the program\n");
            exit(0);
        }
        
        if(work(t->key, t->read, t->write) == ERROR_CODE)
        {
            return -1;
        }
    }
    return 1;
}

void quotes_error(node *l)
{
    delete_list(l);
    l = NULL;
    perror("Incorrect sequence of quotes");
}

void check_bad_ampersand(node *l)
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
    }
}

node* find_pid_proc(node *l, int pid, node **buf)
{
    if(l == NULL)
    {
        return NULL;
    }
    
    if(l->spec_symb == pid) /* this process will be deleted */
    {
        *buf = l;
        node *ret = l;
        
        /* Freeing processed_proc from the current process to be deleted: */
        ret = l->next;
        return ret;
    }
    else
    {
        node *ret = l;
        node *pr = l;
        l = l->next;
        
        while(l)
        {
            if(l->spec_symb == pid) /* this process will be deleted */
            {
                /* Freeing processed_proc from the current process to be deleted: */
                pr->next = l->next;
                *buf = l;
            }
            l = l->next;
            pr = pr->next;
        }
        return ret;
    }
}

void sig_handler(int sig)
{
    int status;
    node* buf = NULL;
    
    /*
        WNOHANG - means immediate return of control
        if no child process has completed execution.
    */
    int pid = waitpid(-1, &status, WNOHANG);
    processed_proc = find_pid_proc(processed_proc, pid, &buf);
    
    if(buf)
    {
        flag_is_spec_symb = buf->spec_symb;
        deleted_proc = add_to_list(deleted_proc, buf->elem); /* adding a process to delete */
        
        free(buf->elem);
        free(buf);
    }
    return;
}

int main(int argc, char **argv)
{
    /* When the command that was running in the background has finished, processing is required: */
    signal(SIGCHLD, sig_handler);
    signal(SIGINT, SIG_IGN);
    
    char *word = NULL;
    int is_correct_quotes = 1;
    int count = 0;
    
    while(!flag_eof)
    {
        main_list = NULL;
        main_tree = NULL;
        flag_exec_in_background = 0;
        flag_new_line = 0;
        flag_is_spec_symb = 0;
        
        printf("%s> %s ", COLOR_RED, RESET); /* color shell */
        
        while(!flag_eof && !flag_new_line)
        {
            is_correct_quotes = 1;
            word = read_word();
            if(word)
            {
                main_list = add_to_list(main_list, word);
                free(word);
            }
            word = NULL;
            count++;
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
            check_bad_ampersand(main_list);
            main_tree = make_tree(main_list, -1, -1, 1);
            main_list = NULL;
            work_tree(main_tree, 0);
            delete_tree(main_tree);
            main_tree = NULL;
        }
        
        print_kill_proc(deleted_proc);
        delete_list(deleted_proc);
        deleted_proc = NULL;
        delete_list(main_list);
        main_list = NULL;
    }
    delete_all_struct();
    kill_all_processes();
    printf("End of the program\n");
    return 0;
}
