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

Simplex** AllSubSimplexses(Simplex* simplex)
{
    int i, j, a = 1 << simplex->verticesCount;
    Simplex** subSimplexes = malloc(a * sizeof(Simplex*));
    for (i = 0; i < a; ++i) {
        int tmpA = i, numberOfElemes = 0;
        Simplex* simpl = Init_Simplex(simplex->verticesCount  * sizeof(SimplexElem), false);
        for (j = 0; j < (8 * sizeof(int)); ++j) {
            if (tmpA & 1 && j < simplex->verticesCount) {
                simpl->simplexRel[numberOfElemes] = simplex->simplexRel[j];
                numberOfElemes++;
            }
            tmpA >>= 1;
        }
        simpl->simplexRel    = realloc(simpl->simplexRel, numberOfElemes * sizeof(SimplexElem));
        simpl->verticesCount = numberOfElemes;
        subSimplexes[i]      = simpl;
    }

    return subSimplexes;
}



