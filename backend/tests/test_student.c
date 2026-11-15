#include <stdio.h>
#include "student.h"

int main(void) {

    Student s1;

    int result = student_init(
        &s1,
        1001,
        "Haitam",
        "Chouaib",
        "haitam@example.com",
        22,
        "Computer Engineering",
        27.5f
    );

    if (result == 1) {
        printf("Student created successfully!\n");
        student_print(&s1);
    } else {
        printf("Student creation failed!\n");
    }

    return 0;
}