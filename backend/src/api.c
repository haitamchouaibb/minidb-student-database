#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "api.h"

static int finish(size_t size, int written){
    if (written < 0 || (size_t)written >= size)
    {
        return -1;
    }
    return written;
}

static int api_error(char *buffer, size_t size, const char *message){
    return finish(size, snprintf(buffer, size, "{\"ok\":false,\"error\":\"%s\"}", message));
}

static int api_ok_student(char *buffer, size_t size, const Student *student){
    char item[512];
    if (student_to_json(student, item, sizeof(item)) < 0)
    {
        return api_error(buffer, size, "internal error");
    }
    return finish(size, snprintf(buffer, size, "{\"ok\":true,\"data\":%s}", item));
}

static int parse_id(const char *text, int *id){
    char *end;
    long value = strtol(text, &end, 10);

    if (end == text || *end != '\0' || value <= 0 || value > INT_MAX)
    {
        return 0;
    }
    *id = (int)value;
    return 1;
}

typedef struct {
    char *buffer;
    size_t size;
    size_t used;
    int count;
    int overflow;
} ListContext;

static void list_visit(const Student *student, void *context){
    ListContext *list = context;
    char item[512];

    if (list->overflow)
    {
        return;
    }

    int length = student_to_json(student, item, sizeof(item));
    size_t separator = (list->count > 0) ? 1 : 0;

    /* keep 3 bytes free for the closing "]}" and the terminating '\0' */
    if (length < 0 || list->used + separator + (size_t)length + 3 > list->size)
    {
        list->overflow = 1;
        return;
    }

    if (separator)
    {
        list->buffer[list->used++] = ',';
    }
    memcpy(list->buffer + list->used, item, (size_t)length);
    list->used += (size_t)length;
    list->count++;
}

static int api_list(Database *db, char *buffer, size_t size){
    int prefix = snprintf(buffer, size, "{\"ok\":true,\"count\":%d,\"data\":[", db_count(db));
    if (prefix < 0 || (size_t)prefix + 3 > size)
    {
        return -1;
    }

    ListContext list = { buffer, size, (size_t)prefix, 0, 0 };
    db_for_each(db, list_visit, &list);

    if (list.overflow)
    {
        return -1;
    }

    buffer[list.used++] = ']';
    buffer[list.used++] = '}';
    buffer[list.used] = '\0';
    return (int)list.used;
}

int api_handle_request(Database *db, const char *request, char *response, size_t size){
    if (db == NULL || request == NULL || response == NULL || size == 0)
    {
        return -1;
    }

    char line[600];
    if (strlen(request) >= sizeof(line))
    {
        return api_error(response, size, "request too long");
    }
    strcpy(line, request);
    line[strcspn(line, "\r\n")] = '\0';

    /* Split "COMMAND arguments" */
    char *args = strchr(line, ' ');
    if (args != NULL)
    {
        *args++ = '\0';
        while (*args == ' ')
        {
            args++;
        }
    }
    else
    {
        args = line + strlen(line);
    }

    for (char *p = line; *p != '\0'; p++)
    {
        *p = (char)toupper((unsigned char)*p);
    }

    if (line[0] == '\0')
    {
        return api_error(response, size, "empty request");
    }

    Student student;
    int id;

    if (strcmp(line, "ADD") == 0)
    {
        if (!student_from_csv(&student, args))
        {
            return api_error(response, size, "invalid student data");
        }
        int result = db_add(db, &student);
        if (result == 1)
        {
            return api_ok_student(response, size, &student);
        }
        return api_error(response, size, result == 0 ? "duplicate id" : "internal error");
    }

    if (strcmp(line, "UPDATE") == 0)
    {
        if (!student_from_csv(&student, args))
        {
            return api_error(response, size, "invalid student data");
        }
        int result = db_update(db, &student);
        if (result == 1)
        {
            return api_ok_student(response, size, &student);
        }
        return api_error(response, size, result == 0 ? "student not found" : "internal error");
    }

    if (strcmp(line, "GET") == 0)
    {
        if (!parse_id(args, &id))
        {
            return api_error(response, size, "invalid id");
        }
        Student *found = db_get(db, id);
        if (found == NULL)
        {
            return api_error(response, size, "student not found");
        }
        return api_ok_student(response, size, found);
    }

    if (strcmp(line, "DELETE") == 0)
    {
        if (!parse_id(args, &id))
        {
            return api_error(response, size, "invalid id");
        }
        int result = db_remove(db, id);
        if (result == 1)
        {
            return finish(size, snprintf(response, size, "{\"ok\":true}"));
        }
        return api_error(response, size, result == 0 ? "student not found" : "internal error");
    }

    if (strcmp(line, "COUNT") == 0)
    {
        return finish(size, snprintf(response, size, "{\"ok\":true,\"count\":%d}", db_count(db)));
    }

    if (strcmp(line, "LIST") == 0)
    {
        return api_list(db, response, size);
    }

    return api_error(response, size, "unknown command");
}