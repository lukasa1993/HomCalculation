//
//  file_util.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/5/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "file_util.h"

#define command "";

char *concat(char *s1, char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *formatedTime() {
    time_t timer;
    char *buffer = malloc(25 * sizeof(char));
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 25, "_%Y-%m-%d_%H-%M-%S", tm_info);
    puts(buffer);

    return buffer;
}

LD_File *Init_file_util(char *path, bool clear) {

    LD_File *file = (LD_File *) malloc(sizeof(LD_File));

    file->path = path;
    file->closed = true;

    if (clear) {
        fclose(fopen(file->path, "w+")); // clear file on path
    }
    return file;
}

LD_File *Init_file_util_ext(char *path, char *ext, bool clear) {
    char *time = formatedTime();
    char *file_path0 = concat(path, time);
    char *file_path1 = concat(file_path0, ".");
    char *file_path2 = concat(file_path1, ext);

    free(time);
    free(file_path0);
    free(file_path1);

    return Init_file_util(file_path2, clear);
}

void wrtieLine(LD_File *file, const char *line, bool skipLine) {
    FILE *fp = NULL;

    while (fp == NULL) {
        fp = fopen(file->path, "a+");
    }

    fputs(line, fp);
    if (!skipLine) {
        fprintf(fp, "\n");
    }
    fclose(fp);
}

char *readLine(LD_File *file) {
    FILE *fp = NULL;
    while (fp == NULL) {
        fp = fopen(file->path, "a+");
    }
    char *line = (char *) malloc(1048 * sizeof(char));
    fscanf(fp, "%s", line);
    fclose(fp);

    return line;
}


char *readFile(LD_File *file) {
    char *buffer = NULL;
    long string_size, read_size;
    FILE *handler = fopen(file->path, "r");

    if (handler) {
        //seek the last byte of the file
        fseek(handler, 0, SEEK_END);
        //offset from the first to the last byte, or in other words, filesize
        string_size = ftell(handler);
        //go back to the start of the file
        rewind(handler);

        //allocate a string that can hold it all
        buffer = (char *) malloc(sizeof(char) * (string_size + 1));
        //read it all in one operation
        read_size = fread(buffer, sizeof(char), string_size, handler);
        //fread doesnt set it so put a \0 in the last position
        //and buffer is now officialy a string
        buffer[string_size + 1] = '\0';

        if (string_size != read_size) {
            //something went wrong, throw away the memory and set
            //the buffer to NULL
            free(buffer);
            buffer = NULL;
        }
    }

    return buffer;
}

void Destroy_file(LD_File *file) {
    free(file->path);
    free(file);
}