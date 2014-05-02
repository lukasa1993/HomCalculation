//
//  complex_struct.h
//  HomCalculation
//
//  Created by Luka Dodelia on 5/2/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_complex_struct_h
#define HomCalculation_complex_struct_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef int SimplexElem;

typedef struct Simplex {
    int          elementIndex;
    int          elementCount;
    SimplexElem* elements;
    
} Simplex;

typedef struct Complex {
    int       simplexIndex;
    int       simplexCount;
    Simplex** simplexes;
    
} Complex;

Simplex* Init_Simplex(int vertices);
void     Dest_Simplex(Simplex* simplex);


Complex* literalToComplex(char*    complexLiteral);

char* complexToLiteral(Complex* complex, bool pretty);
char* simplexToLiteral(Simplex* simplex);

#endif
