#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>

#include "student.h"
#include <cjson/cJSON.h>

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

static int csv_write_field(
    char *buffer,
    size_t size,
    size_t *used,
    const char *value
) {
    if (*used + 2 >= size) {
        return 0;
    }

    buffer[(*used)++] = '"';

    for (size_t i = 0; value[i] != '\0'; i++) {
        if (value[i] == '"') {
            if (*used + 2 >= size) {
                return 0;
            }
            buffer[(*used)++] = '"';
            buffer[(*used)++] = '"';
        } else {
            if (*used + 1 >= size) {
                return 0;
            }
            buffer[(*used)++] = value[i];
        }
    }

    if (*used + 2 > size) {
        return 0;
    }

    buffer[(*used)++] = '"';
    buffer[(*used)++] = ',';

    return 1;
}

static int csv_read_field(
    const char **cursor,
    char *output,
    size_t capacity
) {
    const char *p = *cursor;
    size_t used = 0;

    if (*p == '"') {
        p++;

        while (*p != '\0') {
            if (*p == '"') {
                if (p[1] == '"') {
                    if (used + 1 >= capacity) return 0;
                    output[used++] = '"';
                    p += 2;
                } else {
                    p++;
                    break;
                }
            } else {
                if (used + 1 >= capacity) return 0;
                output[used++] = *p++;
            }
        }

        if (p[-1] != '"') return 0;

        if (*p != ',' && *p != '\n' && *p != '\r' && *p != '\0') {
            return 0;
        }
    } else {
        while (*p != ',' && *p != '\n' &&
               *p != '\r' && *p != '\0') {
            if (used + 1 >= capacity) return 0;
            output[used++] = *p++;
        }
    }

    output[used] = '\0';

    if (*p == ',') p++;

    *cursor = p;
    return 1;
}


int student_to_csv(
    const Student *student,
    char *buffer,
    size_t size
) {
    if (student == NULL || buffer == NULL || size == 0) {
        return -1;
    }

    size_t used = 0;
    int written;

    written = snprintf(buffer, size, "%d,", student->id);
    if (written < 0 || (size_t)written >= size) return -1;
    used = (size_t)written;

    if (!csv_write_field(buffer, size, &used, student->first_name) ||
        !csv_write_field(buffer, size, &used, student->last_name) ||
        !csv_write_field(buffer, size, &used, student->email)) {
        return -1;
    }

    written = snprintf(
        buffer + used,
        size - used,
        "%d,",
        student->age
    );

    if (written < 0 || (size_t)written >= size - used) return -1;
    used += (size_t)written;

    if (!csv_write_field(buffer, size, &used, student->course)) {
        return -1;
    }

    written = snprintf(
        buffer + used - 1,
        size - used + 1,
        "%.2f",
        student->gpa
    );

    if (written < 0 || (size_t)written >= size - used + 1) return -1;

    return (int)(used - 1 + (size_t)written);
}

int student_from_csv(Student *student, const char *csv) {
    if (student == NULL || csv == NULL) {
        return 0;
    }

    char fields[7][COURSE_SIZE + 1];
    const char *cursor = csv;

    for (int i = 0; i < 7; i++) {
        if (!csv_read_field(
            &cursor,
            fields[i],
            sizeof(fields[i])
        )) {
            return 0;
        }
    }

    while (*cursor == '\r' || *cursor == '\n' || *cursor == ' ') {
        cursor++;
    }

    if (*cursor != '\0') return 0;

    char *end;
    errno = 0;
    long id = strtol(fields[0], &end, 10);
    if (errno || end == fields[0] || *end != '\0' || id <= 0 || id > 2147483647L) {
        return 0;
    }

    errno = 0;
    long age = strtol(fields[4], &end, 10);
    if (errno || end == fields[4] || *end != '\0' || age <= 0 || age > 120) {
        return 0;
    }

    errno = 0;
    float gpa = strtof(fields[6], &end);
    if (errno || end == fields[6] || *end != '\0' || gpa < 0.0f || gpa > 30.0f) {
        return 0;
    }

    return student_init(
        student,
        (int)id,
        fields[1],
        fields[2],
        fields[3],
        (int)age,
        fields[5],
        gpa
    );
}



int student_to_json(
    const Student *student,
    char *buffer,
    size_t size
) {
    if (student == NULL || buffer == NULL || size == 0) {
        return -1;
    }

    cJSON *object = cJSON_CreateObject();

    if (object == NULL) {
        return -1;
    }

    cJSON_AddNumberToObject(object, "id", student->id);
    cJSON_AddStringToObject(object, "first_name", student->first_name);
    cJSON_AddStringToObject(object, "last_name", student->last_name);
    cJSON_AddStringToObject(object, "email", student->email);
    cJSON_AddNumberToObject(object, "age", student->age);
    cJSON_AddStringToObject(object, "course", student->course);
    cJSON_AddNumberToObject(object, "gpa", student->gpa);

    char *json = cJSON_PrintUnformatted(object);

    cJSON_Delete(object);

    if (json == NULL) {
        return -1;
    }

    size_t length = strlen(json);

    if (length + 1 > size) {
        cJSON_free(json);
        return -1;
    }

    memcpy(buffer, json, length + 1);
    cJSON_free(json);

    return (int)length;
}
