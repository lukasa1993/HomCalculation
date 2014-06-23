//
//  sqlite_util.c
//  HomCalculation
//
//  Created by Luka Dodelia on 6/23/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "sqlite_util.h"

sqlite3* db;
bool isOpen = false;

sqlite3* Sqlite_Init()
{
    int retval;
    char* sql = "CREATE TABLE IF NOT EXISTS 'hom_table' (	 'id' INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,	 'k' integer NOT NULL,	 'v1' integer NOT NULL,	 'complex' text NOT NULL,	CONSTRAINT 'k_v1' UNIQUE (k, v1));CREATE INDEX IF NOT EXISTS  'comp_index' ON 'hom_table' ('complex' COLLATE RTRIM);";
    if (!isOpen) {
        do {
            retval = sqlite3_open("./hom.db", &db);
        } while (retval > 0);
        
        
        CALL_SQLITE(exec(db, sql, 0, 0, 0));
        isOpen = true;
    }
    
    return db;
}


void Sqlite_Insert(int k, int v1, char* complex)
{
    sqlite3* db = Sqlite_Init();
    char* sql = "insert into 'hom_table' ( 'k', 'v1', 'complex') values ( ?, ?, ?)";
    sqlite3_stmt* stmt;

    CALL_SQLITE (prepare_v2 (db, sql, (int) strlen (sql) + 1, & stmt, NULL));
    CALL_SQLITE (bind_int(stmt, 1, k));
    CALL_SQLITE (bind_int(stmt, 2, v1));
    CALL_SQLITE (bind_text(stmt, 3, complex, (int) strlen(complex) + 1, SQLITE_STATIC));
    CALL_SQLITE_EXPECT (step (stmt), DONE);
    CALL_SQLITE(finalize(stmt));
    
}

char* Sqlite_Get(int k, int v1)
{
    sqlite3_stmt* stmt;
    sqlite3* db = Sqlite_Init();
    char* sql   = "select complex from 'hom_table' where k = ? and v1 = ?";
    char* text  = calloc(2 * sizeof(char), 0);
    text[0] = '[';
    text[1] = ']';
    
    CALL_SQLITE (prepare_v2 (db, sql, (int) strlen (sql) + 1, & stmt, NULL));
    CALL_SQLITE (bind_int(stmt, 1, k));
    CALL_SQLITE (bind_int(stmt, 2, v1));
    
    int s = sqlite3_step (stmt);
    if (s == SQLITE_ROW) {
        char *  tmp  = (char*)sqlite3_column_text(stmt, 0);
        text = realloc(text, (strlen(tmp) + 1) * sizeof(char));
        strcpy(text, tmp);
    }
    
//    CALL_SQLITE_EXPECT (step (stmt), DONE);
    CALL_SQLITE(finalize(stmt));
    
    
    return text;
}