//
//  file_util.h
//  HomCalculation
//
//  Created by Luka Dodelia on 5/5/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_file_util_h
#define HomCalculation_file_util_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct LD_File {
    char *path;
    bool closed;
} LD_File;

LD_File *Init_file_util(char *path, bool clear);

LD_File *Init_file_util_ext(char *path, char *ext, bool clear);

void wrtieLine(LD_File *file, const char *line, bool skipLine);

char *readLine(LD_File *file);

char *readFile(LD_File *file);

void Destroy_file(LD_File *file);

char *concat(char *s1, char *s2);

#endif
