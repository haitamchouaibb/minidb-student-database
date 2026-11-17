#ifndef API_H
#define API_H

#include <stddef.h>
#include "database.h"
int api_handle_request(Database *db, const char *request, char *response, size_t size);

#endif