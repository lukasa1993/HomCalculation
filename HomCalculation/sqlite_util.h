//
//  sqlite_util.h
//  HomCalculation
//
//  Created by Luka Dodelia on 6/23/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_sqlite_util_h
#define HomCalculation_sqlite_util_h

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CALL_SQLITE(f)                                          \
{                                                           \
int i;                                                  \
i = sqlite3_ ## f;                                      \
if (i != SQLITE_OK) {                                   \
fprintf (stderr, "%s failed with status %d: %s\n",  \
#f, i, sqlite3_errmsg (db));               \
exit (1);                                           \
}                                                       \
}                                                           \

#define CALL_SQLITE_EXPECT(f,x)                                 \
{                                                           \
int i;                                                  \
i = sqlite3_ ## f;                                      \
if (i != SQLITE_ ## x) {                                \
fprintf (stderr, "%s failed with status %d: %s\n",  \
#f, i, sqlite3_errmsg (db));               \
exit (1);                                           \
}                                                       \
}                                                           \

void Sqlite_Insert(int k, int v1, char* complex);
char* Sqlite_Get(int k, int v1);

#endif
