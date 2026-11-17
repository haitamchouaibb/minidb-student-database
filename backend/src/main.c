#include <stdio.h>
#include <string.h>
#include "api.h"

#define DATA_PATH "data/students.csv"

/* Reads one command per line from stdin and prints one JSON response per line.
   Extra commands handled here: SAVE, EXIT. The database is also saved on exit. */
int main(void){
    static char response[65536];
    char request[600];

    Database db;
    db_init(&db);

    if (db_load(&db, DATA_PATH) < 0)
    {
        fprintf(stderr, "Warning: could not read %s\n", DATA_PATH);
    }

    while (fgets(request, sizeof(request), stdin) != NULL)
    {
        request[strcspn(request, "\r\n")] = '\0';

        if (request[0] == '\0')
        {
            continue;
        }
        if (strcmp(request, "EXIT") == 0 || strcmp(request, "exit") == 0)
        {
            break;
        }
        if (strcmp(request, "SAVE") == 0 || strcmp(request, "save") == 0)
        {
            if (db_save(&db, DATA_PATH) >= 0)
            {
                printf("{\"ok\":true}\n");
            }
            else
            {
                printf("{\"ok\":false,\"error\":\"save failed\"}\n");
            }
            fflush(stdout);
            continue;
        }

        if (api_handle_request(&db, request, response, sizeof(response)) < 0)
        {
            printf("{\"ok\":false,\"error\":\"response too large\"}\n");
        }
        else
        {
            printf("%s\n", response);
        }
        fflush(stdout);
    }

    if (db_save(&db, DATA_PATH) < 0)
    {
        fprintf(stderr, "Error: could not save %s (does the data/ folder exist?)\n", DATA_PATH);
    }

    db_destroy(&db);
    return 0;
}