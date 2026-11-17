#include <stdio.h>
#include <errno.h>
#include "database.h"

void db_init(Database *db){
    if (db == NULL)
    {
        return;
    }
    bst_init(&db->tree);
}

void db_destroy(Database *db){
    if (db == NULL)
    {
        return;
    }
    bst_destroy(&db->tree);
}

int db_add(Database *db, const Student *student){
    if (db == NULL || student == NULL)
    {
        return -1;
    }
    return bst_insert(&db->tree, student);
}

Student *db_get(Database *db, int id){
    if (db == NULL)
    {
        return NULL;
    }
    return bst_search(&db->tree, id);
}

int db_remove(Database *db, int id){
    if (db == NULL)
    {
        return -1;
    }
    return bst_delete(&db->tree, id);
}

int db_update(Database *db, const Student *student){
    if (db == NULL || student == NULL)
    {
        return -1;
    }

    Student *current = bst_search(&db->tree, student->id);
    if (current == NULL)
    {
        return 0;
    }

    *current = *student;
    return 1;
}

int db_count(const Database *db){
    if (db == NULL)
    {
        return 0;
    }
    return db->tree.size;
}

void db_for_each(const Database *db, BSTVisitFn visit, void *context){
    if (db == NULL)
    {
        return;
    }
    bst_inorder(&db->tree, visit, context);
}

typedef struct {
    FILE *file;
    int error;
} SaveContext;

static void save_visit(const Student *student, void *context){
    SaveContext *save = context;
    char line[512];

    if (save->error)
    {
        return;
    }

    if (student_to_csv(student, line, sizeof(line)) < 0 || fprintf(save->file, "%s\n", line) < 0)
    {
        save->error = 1;
    }
}

int db_save(const Database *db, const char *path){
    if (db == NULL || path == NULL)
    {
        return -1;
    }

    /* Write to a temporary file first so a crash never leaves a half-written database. */
    char temp_path[512];
    if (snprintf(temp_path, sizeof(temp_path), "%s.tmp", path) >= (int)sizeof(temp_path))
    {
        return -1;
    }

    FILE *file = fopen(temp_path, "w");
    if (file == NULL)
    {
        return -1;
    }

    SaveContext save = { file, 0 };
    bst_preorder(&db->tree, save_visit, &save);

    if (fclose(file) != 0)
    {
        save.error = 1;
    }
    if (save.error)
    {
        remove(temp_path);
        return -1;
    }

    if (rename(temp_path, path) != 0)
    {
        /* On Windows rename fails if the target exists: remove it and retry. */
        remove(path);
        if (rename(temp_path, path) != 0)
        {
            remove(temp_path);
            return -1;
        }
    }

    return db->tree.size;
}

int db_load(Database *db, const char *path){
    if (db == NULL || path == NULL)
    {
        return -1;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        return (errno == ENOENT) ? 0 : -1;
    }

    char line[512];
    int loaded = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        Student student;
        if (student_from_csv(&student, line) && bst_insert(&db->tree, &student) == 1)
        {
            loaded++;
        }
    }

    fclose(file);
    return loaded;
}