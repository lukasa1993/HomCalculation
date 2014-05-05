//
//  file_util.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/5/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "file_util.h"

#define command "";

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

LD_File* Init_file_util(char* path, char* ext)
{
    LD_File* file = (LD_File*) malloc(sizeof(LD_File));
    
    file->path   = malloc( (strlen(path) + 1 + strlen(ext)) * sizeof(char));
    file->closed = true;
    
    file->path = concat(path, ".");
    file->path = concat(file->path, ext);
    
    fclose(fopen(file->path, "w+")); // clear file on path
    
    return file;
}


void wrtieLine(LD_File* file, const char* line)
{
    FILE* fp = fopen(file->path, "a+");
    fputs(line, fp);
    fprintf(fp, "\n");
    fclose(fp);
}

char* readLine(LD_File* file)
{
    FILE* fp = fopen(file->path, "a+");
    
    char* line = (char*) malloc(1048 * sizeof(char));
    fscanf(fp, "%s", line);
    fclose(fp);
    
    return line;
}


void Destroy_file(LD_File* file)
{
    free(file->path);
    free(file);
}