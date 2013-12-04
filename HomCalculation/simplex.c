//
//  simplex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include <stdio.h>
#include "simplex.h"

Simplex* Init(int vertices, int facets) {
    Simplex* sim  = malloc(sizeof(Simplex));
    sim->vertices = vertices;
    sim->facets   = facets;
    
    sim->simplexElemArray = malloc(facets * sizeof(SimplexElem));
    return sim;
}