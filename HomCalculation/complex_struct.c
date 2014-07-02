
//  complex_struct.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/2/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "complex_struct.h"

Simplex* Init_Simplex()
{
    Simplex* simplex         = (Simplex*) malloc(sizeof(Simplex));
    simplex->elementIndex    = -1;
    simplex->elementCount    = 0;
    simplex->elementCapacity = 1;
    simplex->elements        = malloc(simplex->elementCapacity * sizeof(SimplexElem));
    
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
    complex->simplexes       = malloc(complex->simplexCapacity * sizeof(Simplex*));
    
    return complex;
}

void Light_Dest_Complex(Complex* complex)
{
    free(complex->simplexes);
    free(complex);
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
        comp->simplexes = realloc(comp->simplexes, comp->simplexCapacity * sizeof(Simplex*));
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


bool containsElement(Simplex* simp, SimplexElem elem)
{
    for (int i = 0; i < simp->elementCount; ++i) {
        SimplexElem elem2 = getElementAt(simp, i);
        if(elem == elem2) {
            return true;
        }
    }
    
    return false;
}


bool containsSimplex(Complex* comp, Simplex* simp)
{
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp2       = getSimpexAt(comp, i);
        bool containsSimplex = true;
        
        for (int s1 = 0; s1 < simp->elementCount; ++s1) {
            SimplexElem elem1 = getElementAt(simp, s1);
            bool containsElem = containsElement(simp2, elem1);
            
            if (containsElem == false) {
                containsSimplex = false;
                break;
            }
        }
        
        if (containsSimplex == true) {
            return true;
        }
    }
    
    return false;
}

sds simplexToLiteral(Simplex* simplex)
{
    sds s = sdsempty();
    s = sdscat(s, "[");

    
    for (int j = 0; j < simplex->elementCount; ++j) {
        SimplexElem elem = simplex->elements[j];
        
        s = sdscat(s, sdsfromlonglong((int) elem));
        
        if (j != simplex->elementCount - 1) {
            s = sdscat(s, ",");
            
            if (true) {
                s = sdscat(s, " ");
            }
        }
    }
    
    s = sdscat(s, "]");

    return s;
}

sds complexToLiteral(Complex* complex, bool pretty)
{
    sds s = sdsempty();
    
    s = sdscat(s, "[");
    
    if (complex != NULL) {
        for (int i = 0; i < complex->simplexCount; ++i) {
            Simplex* simplex = getSimpexAt(complex, i);
            sds simpLit    = simplexToLiteral(simplex);
            
            s = sdscat(s, simpLit);
            sdsfree(simpLit);
            
            if (i != complex->simplexIndex) {
                s = sdscat(s, ",");
                if (pretty) {
                    s = sdscat(s, " ");
                }
            }
        }
    }
    
    s = sdscat(s, "]");
 
    
    return s;
}

Complex* literalToComplex(char* complexLiteral)
{
    Complex*  complex = NULL;
    
    char posibleSimplexElem[10];
    
    int  posibleSimplexElemi = 0;
    int  bracketsCount       = 0;
    
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
        } else if (bracketsCount == 2 && isdigit(aChar)) {
            
            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;
            
        } else if (bracketsCount == 2 && aChar == ',' && posibleSimplexElemi > 0) {
            
            posibleSimplexElemi = 0;
            addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
            
        } else if (aChar == endingChar) {
            if (bracketsCount == 2  && posibleSimplexElemi > 0) {
                posibleSimplexElemi = 0;
                addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
            } else if (bracketsCount == 1) {
                break;
            }
            bracketsCount--;
        }
        
    }
    
    return complex;
}

