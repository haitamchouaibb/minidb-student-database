#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include "bst.h"

void bst_init(BST *tree){
    if (tree == NULL)
    {
        return;     
    }

    tree->root = NULL;
    tree->size = 0;
    
}

static Node *node_create(const Student *student){
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL)
    {
        return;
    }
    
    new_node->data = *student; // puntatore a struct Student
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

static int insert_recursive(Node **current, const Student *student){
    if (*current == NULL)
    {
        *current = node_create(student);
        if (*current == NULL)
        {
            return -1;
        }
        return 1;
        
    }

    if (student->id < (*current)->data.id)
    {
        return insert_recursive(&(*current)->left,student);
    }
    if (student->id > (*current)->data.id)
    {
        return insert_recursive(&(*current)->right,student);
    }
    
    return 0;
}

int bst_insert(BST *tree, const Student *student){
    if (tree == NULL || student == NULL)
    {
        return -1;
    }

    int result = insert_recursive(&tree->root,student);
    if (result == 1)
    {
        tree->size++;
    }

    return result;
}

static Node *search_recursive(Node *current, int id){
    if (current == NULL)
    {
        return NULL;
    }
    
    if (id == current->data.id)
    {
        return current;
    }

    if (id < current->data.id)
    {
        return search_recursive(current->left,id);
    }

        return search_recursive(current->right,id);

}

Student *bst_search(BST *tree,int id){
    if (tree == NULL || id <= 0)
    {
        return NULL;
    }
    
    Node *result = search_recursive(tree->root,id);

    if (result == NULL)
    {
        return NULL;
    }

    printf("Student found ! Name associated with ID %d is %s\n", id, &result->data.first_name);
    return &result->data;
    
}

static void inorder_recursive(const Node *current, BSTVisitFn visit, void *context){
    if (current == NULL)
    {
        return;
    }

    inorder_recursive(current->left,visit,context);
    visit(&current->data, context);
    inorder_recursive(current->right,visit,context);
    
}

void bst_inorder(const BST *tree, BSTVisitFn visit, void *context){
    if (tree == NULL || visit == NULL)
    {
        return;
    }

    inorder_recursive(tree->root, visit,context);
    
}