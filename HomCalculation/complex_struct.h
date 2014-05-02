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
    int          elementCapacity;
    SimplexElem* elements;
    
} Simplex;

typedef struct Complex {
    int       simplexIndex;
    int       simplexCount;
    int       simplexCapacity;
    Simplex** simplexes;
    
} Complex;

// Simplex Manipulations
Simplex* Init_Simplex();
void     Dest_Simplex(Simplex* simplex);

void addElement(Simplex* simp, SimplexElem elem);
SimplexElem getElementAt(Simplex* simp, int index);

char* simplexToLiteral(Simplex* simplex);

// Complex Manipulations
Complex* Init_Complex();
void Dest_Complex(Complex* complex);

void addSimplex(Complex* comp, Simplex* simp);
Simplex* getSimpexAt(Complex* comp, int index);

Complex* literalToComplex(char*    complexLiteral);
char* complexToLiteral(Complex* complex, bool pretty);

#endif
