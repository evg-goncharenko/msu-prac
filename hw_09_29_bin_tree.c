/*
    Реализовать двоичное дерево поиска для целых (без балансировки)
    с операциями вставки, удаления, поиска и обхода
*/
#include <stdio.h>
#include <stdlib.h>

struct node {
    int val;
    struct node *left, *right;
}*tree;

/* Печать всех чисел, хранящихся в дереве: */
void print_bin_tree(struct node *root)
{
    if(!root) return;
    
    print_bin_tree(root->left);
    printf("%d ", root->val);
    print_bin_tree(root->right);
}

/* Вставка нового элемента: */
struct node *add_elem_bin_tree(struct node *root, struct node *r, int n)
{
    if(!r){
        r = (struct node*) malloc(sizeof(struct node));
        if(!r){
            fprintf(stderr, "Недостаточно памяти!\n");
            exit(0);
        }
        r->val = n;
        r->left = NULL;
        r->right = NULL;
        if(!root) return r; /* первый вход */
        if(n < root->val){
            root->left = r;
        } else{
            root->right = r;
        }
        return r;
    }
    if(n < root->val){
        add_elem_bin_tree(r, root->left, n);
    } else {
        add_elem_bin_tree(r, root->right, n);
    }
    return root;
}

/* Обход дерева в ширину: */
void width_traversing_bin_tree(struct node *root)
{
    if(!root) return;
    
    if(root->val) printf("%d ", root->val);
    width_traversing_bin_tree(root->left);
    width_traversing_bin_tree(root->right);
}

/* Обход дерева в глубину: */
void depth_traversing_bin_tree(struct node *root)
{
    if(!root) return;
    
    depth_traversing_bin_tree(root->left);
    depth_traversing_bin_tree(root->right);
    if(root->val) printf("%d ", root->val);
}

/* Поиск элемента в дереве (NULL - если такой вершины нет): */
struct node *find_elem_bin_tree(struct node *root, int numb)
{
    if(!root) return root; /* пустое дерево */
    while(root->val != numb){
        if(numb < root->val){
            root = root->left;
        } else{
            root = root->right;
        }
        if(root == NULL) break;
    }
    return root;
}

/* Удаление элемента numb в дереве: */
struct node *delete_elem_bin_tree(struct node *root, int numb)
{
    struct node *p,*p2;
    
    if(!root) return root; /* вершина не найдена */

    if(root->val == numb) { /* удаление корня */
    /* это означает пустое дерево */
    if(root->left == root->right){
        free(root);
        return NULL;
    } else if(root->left == NULL){ /* или если одно из поддеревьев пустое */
        p = root->right;
        free(root);
        return p;
    } else if(root->right == NULL){
        p = root->left;
        free(root);
        return p;
    } else{ /* или есть оба поддерева */
        p2 = root->right;
        p = root->right;
        while(p->left){ p = p->left; }
        
        p->left = root->left;
        free(root);
        return p2;
        }
    }
    
    if(root->val < numb){
        root->right = delete_elem_bin_tree(root->right, numb);
    } else{
        root->left = delete_elem_bin_tree(root->left, numb);
    }
    return root;
}

int main()
{
    int numb;
    scanf("%d", &numb);
    
    /* Создание дерева: */
    for(int i = 0; i < numb; i++){
        int tmp;
        scanf("%d", &tmp);
        tree = add_elem_bin_tree(tree, tree, tmp);
    }
    
    tree = delete_elem_bin_tree(tree, 2);
    print_bin_tree(tree);
    printf("\n");
    return 0;
}
