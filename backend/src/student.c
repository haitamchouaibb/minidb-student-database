#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "student.h"

int student_init(Student *student, int id, const char *first_name, const char *last_name, const char *email, int age, const char *course, float gpa) {
    if (student == NULL || first_name == NULL || last_name == NULL || email == NULL || course == NULL)
    {
        return 0;
    }

    if (id <= 0 || age <= 0 || age > 120 || gpa < 0.0f || gpa > 30.0f) {
        return 0;
    }

    if (strlen(first_name) >= FIRST_NAME_SIZE ||
        strlen(last_name) >= LAST_NAME_SIZE ||
        strlen(email) >= EMAIL_SIZE ||
        strlen(course) >= COURSE_SIZE) {
        return 0;
    }

    student->id = id;
    student->age = age;
    student->gpa = gpa;

    snprintf(student->first_name, FIRST_NAME_SIZE, "%s", first_name);
    snprintf(student->last_name, LAST_NAME_SIZE, "%s", last_name);
    snprintf(student->email, EMAIL_SIZE, "%s", email);
    snprintf(student->course, COURSE_SIZE, "%s", course);

    return 1;
    
}

void student_print(const Student *student) {

    if (student == NULL) {
        return;
    }

    printf("ID:         %d\n", student->id);
    printf("First name: %s\n", student->first_name);
    printf("Last name:  %s\n", student->last_name);
    printf("Email:      %s\n", student->email);
    printf("Age:        %d\n", student->age);
    printf("Course:     %s\n", student->course);
    printf("GPA:        %.2f\n", student->gpa);
}