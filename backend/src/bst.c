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

static Node *find_min(Node *current){
    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
    
}

static Node *delete_recursive(Node *current, int id, int *deleted){
    if (current == NULL)
    {
        return NULL;
    }

    if (id < current->data.id)
    {
        current->left = delete_recursive(current->left,id,deleted);
        return current;
    }

    if (id > current->data.id)
    {
        current->right = delete_recursive(current->right,id,deleted);
        return current;
    }

    *deleted = 1;

    if (current->left == NULL)
    {
        Node *right = current->right;
        free(current);
        return right;
    }
    if (current->right == NULL)
    {
        Node *left = current->left;
        free(current);
        return left;
    }
    Node *successor = find_min(current->right);
    current->data = successor->data;

    int dummy = 0;
    current->right = delete_recursive(current->right,successor->data.id,&dummy);
    return current;

}

int bst_delete(BST *tree,int id){
    if (tree == NULL || id <= 0)
    {
        return -1;
    }

    int deleted = 0;
    tree->root = delete_recursive(tree->root,id,&deleted);

    if (deleted)
    {
        tree->size--;
        return 1;
    }
    return 0;
    
    
}

static void destroy_recursive(Node *current){
    if (current == NULL)
    {
        return;
    }

    destroy_recursive(current->left);
    destroy_recursive(current->right);
    free(current);
    
}

void bst_destroy(BST *tree){
    if (tree == NULL)
    {
        return;
    }

    destroy_recursive(tree->root);
    tree->root = NULL;
    tree->size = 0;
    
}

static void preorder_recursive(const Node *current, BSTVisitFn visit,void *context){
    if (current == NULL)
    {
        return;
    }

    visit(&current->data,context);
    preorder_recursive(current->left,visit,context);
    preorder_recursive(current->right,visit,context);
    
}

void bst_preorder(const BST *tree, BSTVisitFn visit, void *context){
    if (tree == NULL || visit == NULL)
    {
        return;
    }

    preorder_recursive(tree->root,visit,context);
    
}