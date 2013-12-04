//
//  simplex.h
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_simplex_h
#define HomCalculation_simplex_h

typedef int SimplexElem;

typedef struct Simplex
{
    int  vertices;
    int  facets;
    SimplexElem** simplexElemArray;
} Simplex;

#endif
