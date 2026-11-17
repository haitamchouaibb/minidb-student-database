#ifndef DATABASE_H
#define DATABASE_H

#include "bst.h"

typedef struct {
    BST tree;
} Database;

void db_init(Database *db);
void db_destroy(Database *db);


int db_add(Database *db, const Student *student);

Student *db_get(Database *db, int id);


int db_remove(Database *db, int id);

int db_update(Database *db, const Student *student);

int db_count(const Database *db);

void db_for_each(const Database *db, BSTVisitFn visit, void *context);


int db_save(const Database *db, const char *path);
int db_load(Database *db, const char *path);

#endif