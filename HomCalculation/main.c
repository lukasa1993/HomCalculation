//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include <stdio.h>
#include "simplex.h"

int main(int argc, const char * argv[])
{
    int n = 4, m = 3, i, j;
    int **lists = (int**) malloc(n * sizeof(int *));
    
    for(i = 0; i < n; i++)
        lists[i] = (int*) calloc(m, sizeof(int));
    
    for(i = 0; i < n; i++) {
        for(j = 0; j < m; j++) {
            printf("%i", lists[i][j]);
        }
        printf("\n");
    }
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}

