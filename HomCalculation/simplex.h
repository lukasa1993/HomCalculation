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

typedef SimplexElem* Simplex;

typedef struct SimplicialComplex {
    int verticesCount;
    int fasetsCount;
    Simplex* facets;
} SimplicialComplex;



#endif
