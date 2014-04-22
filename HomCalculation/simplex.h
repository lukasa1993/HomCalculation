//
//  simplex.h
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_simplex_h
#define HomCalculation_simplex_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "strmap.h"

typedef int SimplexElem;

typedef struct Simplex {
    int          verticesCount;
    int*         isomorphizmsEnumeration;
    SimplexElem* simplexRel;
    
} Simplex;

typedef struct Complex {
    int       simplexCount;
    Simplex** simplexes;
    
} Complex;

Simplex* Init_Simplex(int vertices, bool fill);
void     Dest_Simplex(Simplex* simplex);

Complex* AllSubSimplexses(Simplex* simplex);
Simplex* simplexByExp(Simplex* simplex, int exp);

Complex* literalToComplex(char*    complexLiteral);
char*    complexToLiteral(Complex* complex, bool pretty);

char*    simplexToLiteral(Simplex* simplex);

// calculation functions

Complex* FSI(Complex* A, Complex* B, int K, int V);
void Calculate_Hom(Complex* A, Complex* B);

#endif
