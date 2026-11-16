#ifndef BST_H
#define BST_H
#include "student.h"

typedef struct Node {
    Student data;

    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    Node *root;
    int size;
} BST;

void bst_init(BST *tree);

int bst_insert(BST *tree, const Student *student);

Student *bst_search(BST *tree, int id);

typedef void (*BSTVisitFn)(const Student *student, void *context);

void bst_inorder(const BST *tree, BSTVisitFn visit, void *context);

#endif