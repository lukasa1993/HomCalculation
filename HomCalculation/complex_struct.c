//
//  complex_struct.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/2/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "complex_struct.h"

#define startingChar '{'
#define endingChar   '}'

Simplex* Init_Simplex()
{
    Simplex* simplex         = (Simplex*) malloc(sizeof(Simplex));
    simplex->elementIndex    = -1;
    simplex->elementCount    = 0;
    simplex->elementCapacity = 1;
    simplex->elements        = malloc(simplex->elementCount * sizeof(SimplexElem));
    
    return simplex;
}

void Dest_Simplex(Simplex* simplex)
{
    free(simplex->elements);
    free(simplex);
}


Complex* Init_Complex()
{
    Complex* complex         = (Complex*) malloc(sizeof(Complex));
    complex->simplexIndex    = -1;
    complex->simplexCount    = 0;
    complex->simplexCapacity = 1;
    complex->simplexes       = malloc(complex->simplexCapacity * sizeof(Simplex));
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

void addSimplex(Complex* comp, Simplex* simp)
{
    if (comp->simplexCapacity <= comp->simplexIndex + 1) {
        comp->simplexCapacity <<= 1;
        comp->simplexes = realloc(comp->simplexes, comp->simplexCapacity * sizeof(Simplex));
    }
    
    comp->simplexCount++;
    comp->simplexes[comp->simplexIndex + 1] = simp;
    comp->simplexIndex = comp->simplexCount - 1;
}

void addElement(Simplex* simp, SimplexElem elem)
{
    if (simp->elementCapacity <= simp->elementIndex + 1) {
        simp->elementCapacity <<= 1;
        simp->elements = realloc(simp->elements, simp->elementCapacity * sizeof(SimplexElem));
    }
    
    simp->elementCount++;
    simp->elements[simp->elementIndex + 1] = elem;
    simp->elementIndex = simp->elementCount - 1;
}

Simplex* getSimpexAt(Complex* comp, int index)
{
    if (comp->simplexIndex < index) {
        return NULL;
    } else {
        return comp->simplexes[index];
    }
}

SimplexElem getElementAt(Simplex* simp, int index)
{
    if (simp->elementIndex < index) {
        return -1;
    } else {
        return simp->elements[index];
    }
}


char* simplexToLiteral(Simplex* simplex)
{
    char* literal  = malloc(2048 * sizeof(char));
    int   literali = 0;
    
    literal[literali] = startingChar;
    literali++;
    
    for (int j = 0; j < simplex->elementCount; ++j) {
        SimplexElem elem = simplex->elements[j];
        char str[100];
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
    
    literal[literali] = endingChar;
    literali++;
    
    return literal;
}

char* complexToLiteral(Complex* complex, bool pretty)
{
    char* literal  = malloc(2048 * sizeof(char));
    int   literali = 0;
    literal[literali] = startingChar;
    literali++;
    
    for (int i = 0; i < complex->simplexCount; ++i) {
        Simplex* simplex = getSimpexAt(complex, i);
        literal[literali] = startingChar;
        literali++;
        
        for (int j = 0; j < simplex->elementCount; ++j) {
            SimplexElem elem = getElementAt(simplex, j);
            char str[100];
            sprintf(str, "%d", elem);
            
            for (int s = 0; s < strlen(str); ++s) {
                literal[literali] = str[s];
                literali++;
            }
            
            if (j != simplex->elementIndex) {
                literal[literali] = ',';
                literali++;
                if (pretty) {
                    literal[literali] = ' ';
                    literali++;
                }
            }
        }
        
        literal[literali] = endingChar;
        literali++;
        
        if (i != complex->simplexIndex) {
            literal[literali] = ',';
            literali++;
            if (pretty) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }
    
    literal[literali] = endingChar;
    literali++;
    return literal;
}

Complex* literalToComplex(char* complexLiteral)
{
    Complex*  complex;
    
    int magicNumberInput      = 4;
    int bracketsCount         = 0;
    
    char* posibleSimplexElem  = (char*) malloc(magicNumberInput * sizeof(char));
    int   posibleSimplexElemi = 0;
    
    for (int i = 0; i < strlen(complexLiteral); ++i) {
        char  aChar = complexLiteral[i];
        
        if (aChar == '[') {
            aChar = startingChar;
        } else if (aChar == ']') {
            aChar = endingChar;
        }
        
        if (aChar == startingChar) {
            if (bracketsCount == 0) {
                complex  = Init_Complex();
            } else if (bracketsCount == 1) {
                addSimplex(complex, Init_Simplex());
            }
            bracketsCount++;
        } else if (bracketsCount == 2 && aChar != ',' && aChar != endingChar && aChar != '\0' && aChar != ' ') {
            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;
            if (posibleSimplexElemi > magicNumberInput) {
                magicNumberInput   <<= 1;
                posibleSimplexElem = realloc(posibleSimplexElem, magicNumberInput * sizeof(char));
            }
        } else if (bracketsCount == 2 && aChar == ',') {
            
            posibleSimplexElem = realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char));
            addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
            
            posibleSimplexElemi               = 0;
        } else if (aChar == endingChar) {
            if (bracketsCount == 2) {
                posibleSimplexElem = realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char)); // grabing last element
                
                addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
                
                posibleSimplexElemi = 0;
            } else if (bracketsCount == 1) {
                break;
            }
            bracketsCount--;
        }
        
    }
    
    return complex;
}