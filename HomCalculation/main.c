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
//    scanf("%s", firstComplex);
    firstComplex = "[[1, 2], [2, 3], [1, 3]]";
    printf("\n");
    
    printf("Second Complex Please:\t");
//    scanf("%s", secondComplex);
    secondComplex = "[[1, 2], [2, 3]]";
    printf("\n---- As We Saw it ---- \n");
    
    Complex* complex1 = literalToComplex(firstComplex);
    Complex* complex2 = literalToComplex(secondComplex);
    printf("First  Complex: %s\n", complexToLiteral(complex1, true));
    printf("Second Complex: %s\n\n", complexToLiteral(complex2, true));
    

    Calculate_Hom(complex1, complex2);
    
//    int initV = 9;
//    Complex* sim = FSI(complex1, complex2, 1, initV);
//    printf("\n%d -> %s \n", initV, complexToLiteral(sim, true));
    
//    int V = 17, vertecTotal = 0, i = 0;
//    Complex* A = complex1;
//    
//    for ( ; i < A->simplexCount; i++) {
//        Simplex* sim = A->simplexes[i];
//        int prevVertecTotal = vertecTotal;
//        vertecTotal += sim->verticesCount;
//        int expV = (1 << vertecTotal) - 1;
//        if (V > prevVertecTotal && expV > V) {
//            V = V - ((1 << prevVertecTotal) - 1);
//            break;
//        }
//    }
//    printf("%d %d\n", i, V);
//    
//    Calculate_Hom(complex1, complex2);
    
    printf("\nThe End!\n");
    return 0;
}
