//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "simplex.h"

#define MAX_STRING_LEN 255


int main(int argc, const char * argv[])
{

    char* firstComplex  = (char*) malloc(MAX_STRING_LEN * sizeof(char));
    char* secondComplex = (char*) malloc(MAX_STRING_LEN * sizeof(char));
    
    printf("Complex Please:\t");
    scanf("%s", firstComplex);
    
    printf("\n");
    
    printf("Second Complex Please:\t");
    scanf("%s", secondComplex);
    
    printf("\n ---- As We Saw it ---- \n");
    
    Complex* complex1 = literalToComplex(firstComplex);
    Complex* complex2 = literalToComplex(secondComplex);
    printf("First  Complex: %s\n", complexToLiteral(complex1, true));
    printf("Second Complex: %s\n", complexToLiteral(complex2, true));
    
    printf("\nThe End!\n");
    return 0;
}
