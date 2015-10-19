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
#include <ctype.h>

#define startingChar '['
#define endingChar   ']'

typedef int SimplexElem;

typedef struct Matrix {
    int mIndex;
    int mCount;
    int mCapacity;

    int rows;
    int columns;

    double *m;
} Matrix;


typedef struct Simplex {
    int elementIndex;
    int elementCount;
    int elementCapacity;
    SimplexElem *elements;

    // polytop phase
    struct Complex* allowedSubSimplexes;
    int dimension;

    Matrix* coodinates;
    Matrix* inequalityMatrix;
} Simplex;


typedef struct Complex {
    int simplexIndex;
    int simplexCount;
    int simplexCapacity;
    Simplex **simplexes;

} Complex;


// Simplex Manipulations
Simplex *Init_Simplex();

void Dest_Simplex(Simplex *simplex);

void addElement(Simplex *simp, SimplexElem elem);

SimplexElem getElementAt(Simplex *simp, int index);

bool containsElement(Simplex *simp, SimplexElem elem);

char *simplexToLiteral(Simplex *simplex);

// Complex Manipulations
Complex *Init_Complex();

void Dest_Complex(Complex *complex);

void Light_Dest_Complex(Complex *complex);

void addSimplex(Complex *comp, Simplex *simp);

Simplex *getSimpexAt(Complex *comp, int index);

bool containsSimplex(Complex *comp, Simplex *simp);
bool containsSubSimplex(Simplex *simp, Simplex *sub);
Simplex* getFacet(Complex *comp, Simplex *simp);

Complex *literalToComplex(char *complexLiteral);

Simplex *literalToSimplex(char *simplexLiteral);

char *complexToLiteral(Complex *complex, bool pretty);

Simplex* compySimplex(Simplex* a);


Matrix *Init_Matrix();
void Dest_Matrix(Matrix *m);
void addMElement(Matrix *matrix, double elem);
char *matrixToLiteral(Matrix *matrix);

#include "simplex_basic.h"

#endif
