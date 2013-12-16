//
//  simplex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include "simplex.h"

Simplex* Init_Simplex(int vertices, bool fill)
{
    if (vertices > ((sizeof(SimplexElem) * 8) - 2)) {
        printf("\nSimplexElem StackOverflow\n");
        exit(0);
    }
    Simplex* simplex       = (Simplex*) malloc(sizeof(Simplex));
    simplex->verticesCount = vertices;
    simplex->simplexRel    = malloc(vertices * sizeof(SimplexElem));
    if (fill) {
        for (int i = 0; i < simplex->verticesCount; i++) {
            simplex->simplexRel[i] = i + 1;
        }
    }
    return simplex;
}

void Dest_Simplex(Simplex* simplex)
{
    free(simplex->simplexRel);
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

Complex* AllSubSimplexses(Simplex* simplex)
{
    int i, j, a = 1 << simplex->verticesCount;
    Complex*  complex     = Init_Complex(a);
    for (i = 0; i < a; ++i) {
        int tmpA = i, numberOfElemes = 0;
        Simplex* simpl = Init_Simplex(simplex->verticesCount, false);
        for (j = 0; j < (8 * sizeof(int)); ++j) {
            if (tmpA & 1 && j < simplex->verticesCount) {
                simpl->simplexRel[numberOfElemes] = simplex->simplexRel[j];
                numberOfElemes++;
            }
            tmpA >>= 1;
        }
        realloc(simpl->simplexRel, numberOfElemes * sizeof(SimplexElem));
        simpl->verticesCount  = numberOfElemes;
        complex->simplexes[i] = simpl;
    }

    return complex;
}


Complex* literalToComplex(char* complexLiteral)
{
    Complex*  complex;
    
    int magicNumberSimplex    = 4;
    int magicNumberComplex    = 4;
    int magicNumberInput      = 4;
    int bracketsCount         = 0;
    int simplexi              = 0;
    int simplexElemi          = 0;
    
    char* posibleSimplexElem  = (char*) malloc(magicNumberInput * sizeof(char));
    int   posibleSimplexElemi = 0;
    
    for (int i = 0; i < strlen(complexLiteral); ++i) {
        char  aChar = complexLiteral[i];
        if (aChar == '[') {
            if (bracketsCount == 0) {
                complex  = Init_Complex(magicNumberComplex);
            } else if (bracketsCount == 1) {
                complex->simplexes[simplexi] = Init_Simplex(magicNumberSimplex, false);
                if (simplexi > magicNumberComplex) {
                    magicNumberComplex  <<= 1;
                    complex->simplexCount = magicNumberComplex;
                    realloc(complex->simplexes, complex->simplexCount * sizeof(complex));
                }
            }
            bracketsCount++;
        } else if (bracketsCount == 2 && aChar != ',' && aChar != ']') {
            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;
            if (posibleSimplexElemi > magicNumberInput) {
                magicNumberInput   <<= 1;
                realloc(posibleSimplexElem, magicNumberInput * sizeof(char));
            }
        } else if (bracketsCount == 2 && aChar == ',') {
            Simplex* simplex                  = complex->simplexes[simplexi];
            realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char));
            simplex->simplexRel[simplexElemi] = atoi(posibleSimplexElem);
            posibleSimplexElemi               = 0;
            simplexElemi++;
            if (simplexElemi > magicNumberSimplex) {
                magicNumberSimplex   <<= 1;
                simplex->verticesCount = magicNumberSimplex;
                realloc(simplex->simplexRel, simplex->verticesCount * sizeof(SimplexElem));
            }
        } else if (aChar == ']') {
            if (bracketsCount == 2) {
                Simplex* simplex       = complex->simplexes[simplexi];
                
                realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char)); // grabing last element
                simplex->simplexRel[simplexElemi] = atoi(posibleSimplexElem);
                simplexElemi++;
                
                simplex->verticesCount = simplexElemi;
                realloc(simplex->simplexRel, simplex->verticesCount * sizeof(SimplexElem));
                simplexi++;
                simplexElemi        = 0;
                posibleSimplexElemi = 0;
            } else if (bracketsCount == 1) {
                complex->simplexCount = simplexi;
                realloc(complex->simplexes, complex->simplexCount * sizeof(Simplex));
                break;
            }
            bracketsCount--;
        }
        
    }
    
    return complex;
}

char* complexToLiteral(Complex* complex, bool pretty)
{
    char* literal  = malloc(255 * sizeof(char));
    int   literali = 0;
    literal[literali] = '[';
    literali++;
    
    for (int i = 0; i < complex->simplexCount; ++i) {
        Simplex* simplex = complex->simplexes[i];
        literal[literali] = '[';
        literali++;
        
        for (int j = 0; j < simplex->verticesCount; ++j) {
            SimplexElem elem = simplex->simplexRel[j];
            char str[10];
            sprintf(str, "%d", elem);
            
            for (int s = 0; s < strlen(str); ++s) {
                literal[literali] = str[s];
                literali++;
            }
            
            if (j != simplex->verticesCount - 1) {
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


