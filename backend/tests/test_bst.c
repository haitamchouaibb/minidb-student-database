#include <stdio.h>
#include "bst.h"

void print_student(const Student *student, void *context){
    (void)context;

    printf("%d | %s %s | GPA : %.2f\n", student->id,student->first_name,student->last_name,student->gpa);
}

int main(void) {

    BST tree;

    bst_init(&tree);

    Student s1, s2, s3, s4, s5;

    student_init(&s1, 1005, "Haitam", "Chouaib",
                 "haitam@example.com", 22, "Computer Engineering", 27.5f);

    student_init(&s2, 1002, "Leonardo", "Maria del Vecchio",
                 "ahmed@example.com", 24, "Computer Engineering", 25.0f);

    student_init(&s3, 1008, "Giuseppe", "Floriano",
                 "youssef@example.com", 21, "Mathematics", 28.0f);

    student_init(&s4, 1001, "Anas", "Khan",
                 "omar@example.com", 23, "Physics", 26.0f);

    student_init(&s5, 1003, "Will", "Smith",
                 "adam@example.com", 20, "Engineering", 29.0f);

    printf("Inserting students...\n");

    printf("Student 1: %d\n", bst_insert(&tree, &s1)); // Begins from the root and checks recursively for a NULL space to allocate the new Node
    printf("Student 2: %d\n", bst_insert(&tree, &s2));
    printf("Student 3: %d\n", bst_insert(&tree, &s3));
    printf("Student 4: %d\n", bst_insert(&tree, &s4));
    printf("Student 5: %d\n", bst_insert(&tree, &s5));

Student *found = bst_search(&tree, 1003); // bst_search functions return pointer to Student struct

if (found != NULL) {
    student_print(found);
} else {
    printf("Student not found.\n");
}

found = bst_search(&tree, 9999); // returns NULL if not found

if (found != NULL) {
    student_print(found);
} else {
    printf("Student 9999 not found.\n");
}

bst_inorder(&tree, print_student, NULL);

    printf("\nDatabase size: %d\n", tree.size);

    printf("Root ID: %d\n", tree.root->data.id);

    printf("Left child: %d\n", tree.root->left->data.id);

    printf("Right child: %d\n", tree.root->right->data.id);

    int id_N = 1002;
    bst_search(&tree,id_N);
    

    return 0;
}
    
