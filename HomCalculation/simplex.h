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

typedef Simplex* Facet;

typedef struct SimplicialComplex {
    int vertices;
    int fasets;
    Facet* Facets;
} SimplicialComplex;



#endif
