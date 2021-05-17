/*
    Implement a binary search tree for integers (without balancing)
    with insert, delete, search, and crawl operations
*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int val;
    struct Node *left, *right;
} * tree;

/* Print all numbers stored in the tree: */
void print_bin_tree(struct Node *root) {
    if (!root) return;

    print_bin_tree(root->left);
    printf("%d ", root->val);
    print_bin_tree(root->right);
}

/* Inserting a new element: */
struct Node *add_elem_bin_tree(struct Node *root, struct Node *r, int n) {
    if (!r) {
        r = (struct Node *)malloc(sizeof(struct Node));
        if (!r) {
            fprintf(stderr, "Enough memory\n");
            exit(0);
        }
        r->val = n;
        r->left = NULL;
        r->right = NULL;
        if (!root) return r; /* the first entrance */

        if (n < root->val) {
            root->left = r;
        } else {
            root->right = r;
        }
        return r;
    }
    if (n < root->val) {
        add_elem_bin_tree(r, root->left, n);
    } else {
        add_elem_bin_tree(r, root->right, n);
    }
    return root;
}

/* Traversing the tree in width: */
void width_traversing_bin_tree(struct Node *root) {
    if (!root) return;

    if (root->val) printf("%d ", root->val);
    width_traversing_bin_tree(root->left);
    width_traversing_bin_tree(root->right);
}

/* Traversing the tree in depth: */
void depth_traversing_bin_tree(struct Node *root) {
    if (!root) return;

    depth_traversing_bin_tree(root->left);
    depth_traversing_bin_tree(root->right);
    if (root->val) printf("%d ", root->val);
}

/* Search for an element in the tree (NULL - if there is no such vertex): */
struct Node *find_elem_bin_tree(struct Node *root, int numb) {
    if (!root) return root; /* empty tree */
    while (root->val != numb) {
        if (numb < root->val) {
            root = root->left;
        } else {
            root = root->right;
        }

        if (root == NULL) break;
    }
    return root;
}

/* Deleting the numb element in the tree: */
struct Node *delete_elem_bin_tree(struct Node *root, int numb) {
    struct Node *p, *p2;
    if (!root) return root; /* vertex wasn't found */

    /* Removing the root: */
    if (root->val == numb) {
        if (root->left == root->right) {
            free(root);
            return NULL;
        } else if (root->left == NULL) { /* if one of the subtrees is empty */
            p = root->right;
            free(root);
            return p;
        } else if (root->right == NULL) {
            p = root->left;
            free(root);
            return p;
        } else { /* there are both subtrees */
            p2 = root->right;
            p = root->right;

            while (p->left) {
                p = p->left;
            }

            p->left = root->left;
            free(root);
            return p2;
        }
    }

    if (root->val < numb) {
        root->right = delete_elem_bin_tree(root->right, numb);
    } else {
        root->left = delete_elem_bin_tree(root->left, numb);
    }
    return root;
}

int main() {
    int numb;
    scanf("%d", &numb);

    /* Creating a tree: */
    for (int i = 0; i < numb; i++) {
        int tmp;
        scanf("%d", &tmp);
        tree = add_elem_bin_tree(tree, tree, tmp);
    }

    tree = delete_elem_bin_tree(tree, 2);
    print_bin_tree(tree);
    printf("\n");
    return 0;
}
