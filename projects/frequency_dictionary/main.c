#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CONST_I "-i"
#define CONST_O "-o"

#define INPUT_ERROR 1
#define OUTPUT_ERROR 2

struct node {
    char *val; /* слово */
    int numb_occur; /* число вхождений слова */
    struct node *left, *right;
}*tree;

/* Печать всех слов, хранящихся в дереве: */
void print_bin_tree(struct node *root)
{
    if(!root) return;
    
    print_bin_tree(root->left);
    printf("val: '%s' - numb_occur: %d\n", root->val, root->numb_occur);
    print_bin_tree(root->right);
}

/* Вставка нового элемента: */
struct node *add_elem_bin_tree(struct node *root, struct node *r, char *n, int *max_numb_occur)
{
    if(!r){
        r = (struct node*) malloc(sizeof(struct node));
        if(!r){
            fprintf(stderr, "Not enough memory!\n");
            exit(0);
        }
        r->val = malloc(sizeof(n));
        strcpy(r->val, n);
        r->numb_occur = 1;
        r->left = NULL;
        r->right = NULL;
        if(!root) return r; /* первый вход */
        if(strcmp(n, root->val) < 0){
            root->left = r; /* n < root->val */
        } else{
            root->right = r; /* n >= root->val */
        }
        return r;
    }
    if(!strcmp(n, root->val)){ /* если такое слово уже существует */
        r->numb_occur++;
        if((r->numb_occur) > *max_numb_occur) *max_numb_occur = (r->numb_occur);
    } else if(strcmp(n, root->val) < 0){
        add_elem_bin_tree(r, root->left, n, max_numb_occur); /* n < root->val */
    } else{
        add_elem_bin_tree(r, root->right, n, max_numb_occur); /* n >= root->val */
    }
    return root;
}

/* Итоговая печать: */
void print_from_bin_tree(struct node *root, int max_numb_occur, int total_amount, FILE *file_out)
{
    if(!root) return;
    if(root->numb_occur == max_numb_occur){
        /* frequency - частота вхождений слова */
        long double frequency = 1.0 * (root->numb_occur) / total_amount;
        fprintf(file_out, "%s %d %Lf\n", root->val, root->numb_occur, frequency);
    }
    
    print_from_bin_tree(root->left, max_numb_occur, total_amount, file_out);
    print_from_bin_tree(root->right, max_numb_occur, total_amount, file_out);
}

/* Удаление дерева: */
void delete_bin_tree(struct node *root)
{
    if(!root) return;
    
    delete_bin_tree(root->left);
    delete_bin_tree(root->right);
    
    root->left = NULL;
    root->right = NULL;
    free(root->val);
    free(root);
}

int main(int argc, char **argv)
{
    struct node *tree = NULL;
    long total_amount = 0; /* суммарное кол-во слов */
    int max_numb_occur = -1; /* максимальное число вхождений слова */
    
    int flag_i = 0; /* флаг на наличие опции '-i' */
    int flag_o = 0; /* флаг на наличие опции '-o' */
    int out_position; /* индекс имени file_out в argv */
    
    FILE *file_in;
    FILE *file_out;
    
    /* Проверка на наличие опций: */
    for(int i = 1; i < argc; i++){
        if(!strcmp(argv[i], CONST_I)) flag_i = 1;
        if(!strcmp(argv[i], CONST_O)){
            flag_o = 1;
            out_position = i + 1;
        }
    }
    
    /* Инициализация ввода: */
    if(flag_i){
        if ((file_in = fopen(argv[2], "r")) == NULL){
            fprintf(stderr, "File '%s' not found\n", argv[2]);
            
            return INPUT_ERROR;
        }
        printf("file '%s' reading:\n", argv[2]);
    } else{
        file_in = stdin;
        printf("stdin reading:\n");
    }
    
    /* Инициализация вывода: */
    if(flag_o){
        if ((file_out = fopen(argv[out_position], "w")) == NULL){
            fprintf(stderr, "File '%s' not found\n", argv[out_position]);
            
            return OUTPUT_ERROR;
        }
        printf("file '%s' writing:\n", argv[out_position]);
    } else{
        file_out = stdout;
        printf("stdout writing:\n");
    }
    
    /* Добавление элементов в дерево: */
    char *tmp = NULL; /* слово на добавление */
    int tmp_size = 0; /* максимальный размер массива tmp*/
    int tmp_numb = 0; /* кол-во символов в массиве tmp */
    char c;
    do {
        c = fgetc(file_in);
        if ((c != '\n') && (c != ' ') && (c != EOF) && isalpha(c)){
            if(tmp_numb == tmp_size){ /* необходимо увеличить размерность */
                tmp = realloc(tmp, 2 * tmp_size + 1);
                tmp_size = 2 * tmp_size + 1;
            }
            tmp[tmp_numb] = c;
            tmp_numb++;
        } else if(tmp_numb){ /* пора добавлять непустое слово в дерево */
            tmp[tmp_numb] = 0;
            tree = add_elem_bin_tree(tree, tree, tmp, &max_numb_occur);
            free(tmp);
            tmp = NULL;
            tmp_size = tmp_numb = 0;
            total_amount++;
        }
        /* Добавляем слова вида: ".", "-", "!", "?" ... */
        if(!isalpha(c) && (c != '\n') && (c != ' ') && (c != EOF)){
            tree = add_elem_bin_tree(tree, tree, &c, &max_numb_occur);
        }
    } while(c != EOF);
    
    /* Печать в file_out: */
    while(max_numb_occur > 0){
        print_from_bin_tree(tree, max_numb_occur, total_amount, file_out);
        max_numb_occur--;
    }
    
    fclose(file_in);
    fclose(file_out);
    
    delete_bin_tree(tree);
    return 0;
}
