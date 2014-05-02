//
//  complex_struct.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/2/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "complex_struct.h"


Simplex* Init_Simplex(int vertices)
{
    Simplex* simplex      = (Simplex*) malloc(sizeof(Simplex));
    simplex->el
    simplex->elementCount = vertices;
    simplex->elements     = malloc(vertices * sizeof(SimplexElem));
    
    return simplex;
}

void Dest_Simplex(Simplex* simplex)
{
    free(simplex->elements);
    free(simplex);
}


Complex* Init_Complex(int simplexCount)
{
    Complex* complex       = (Complex*) malloc(sizeof(Complex));
    complex->simplexCount  = simplexCount;
    complex->simplexes     = malloc(simplexCount * sizeof(Simplex));
    return complex;
}

void Dest_Complex(Complex* complex)
{
    for (int i = 0; i < complex->simplexCount; ++i) {
        Dest_Simplex(complex->simplexes[i]);
    }
    free(complex->simplexes);
    free(complex);
}


char* simplexToLiteral(Simplex* simplex) {
    if (simplex == NULL) {
        return "";
    }
    
    char* literal  = malloc(255 * sizeof(char));
    int   literali = 0;
    
    literal[literali] = '[';
    literali++;
    
    for (int j = 0; j < simplex->elementCount; ++j) {
        SimplexElem elem = simplex->elements[j];
        char str[10];
        sprintf(str, "%d", elem);
        
        for (int s = 0; s < strlen(str); ++s) {
            literal[literali] = str[s];
            literali++;
        }
        
        if (j != simplex->elementCount - 1) {
            literal[literali] = ',';
            literali++;
            if (true) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }
    
    literal[literali] = ']';
    literali++;
    
    return literal;
}

char* complexToLiteral(Complex* complex, bool pretty)
{
    if (complex == NULL) {
        return "[[]]";
    }
    char* literal  = malloc(255 * sizeof(char));
    int   literali = 0;
    literal[literali] = '[';
    literali++;
    
    for (int i = 0; i < complex->simplexCount; ++i) {
        Simplex* simplex = complex->simplexes[i];
        literal[literali] = '[';
        literali++;
        
        for (int j = 0; j < simplex->elementCount; ++j) {
            SimplexElem elem = simplex->elements[j];
            char str[10];
            sprintf(str, "%d", elem);
            
            for (int s = 0; s < strlen(str); ++s) {
                literal[literali] = str[s];
                literali++;
            }
            
            if (j != simplex->elementCount - 1) {
                literal[literali] = ',';
                literali++;
                if (pretty) {
                    literal[literali] = ' ';
                    literali++;
                }
            }
        }
        
        literal[literali] = ']';
        literali++;
        
        if (i != complex->simplexCount - 1) {
            literal[literali] = ',';
            literali++;
            if (pretty) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }
    
    literal[literali] = ']';
    literali++;
    return literal;
}