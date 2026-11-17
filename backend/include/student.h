#ifndef STUDENT_H
#define STUDENT_H
#include <stddef.h>

#define FIRST_NAME_SIZE 50
#define LAST_NAME_SIZE 50
#define EMAIL_SIZE 100
#define COURSE_SIZE 100

typedef struct {
    int id;
    char first_name[FIRST_NAME_SIZE];
    char last_name[LAST_NAME_SIZE];
    char email[EMAIL_SIZE];
    int age;
    char course[COURSE_SIZE];
    float gpa;
} Student;

int student_init(
    Student *student, // Pointer to a Student structure -> allows to modify the original student rather than a temporary copy
    int id,
    const char *first_name,
    const char *last_name,
    const char *email,
    int age,
    const char *course,
    float gpa
);

void student_print(const Student *student);

int student_to_csv(const Student *student, char *buffer, size_t size);
int student_from_csv(Student *student, const char *csv);
int student_to_json(const Student *student, char *buffer, size_t size);
#endif;