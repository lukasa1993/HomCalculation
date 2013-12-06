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

typedef int SimplexElem;

//typedef SimplexElem* Simplex;

typedef struct Simplex {
    int verticesCount;
    SimplexElem* simplexRel;
} Simplex;

Simplex* Init_Simplex(int vertices, bool fill);
void Dest_Simplex(Simplex* simplex);

Simplex** AllSubSimplexses(Simplex* simplex);

#endif
