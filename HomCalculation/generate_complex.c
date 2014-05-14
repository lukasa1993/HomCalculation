//
//  generate_complex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/14/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "generate_complex.h"
#include <time.h>

/* Would like a semi-open interval [min, max) */
int random_in_range (unsigned int min, unsigned int max)
{
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range       = max - min,
    remainder   = RAND_MAX % range,
    bucket      = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) {
        return min + base_random / bucket;
    } else {
        return random_in_range (min, max);
    }
}

Complex* generateComplex(int facetCount, int dimension, int maxPoint)
{
    srand((int) time(NULL));

    Complex* comp = Init_Complex();
    for (int i = 0; i < facetCount; ++i) {
        Simplex* simp = Init_Simplex();
        
        int facetDim = random_in_range(1, dimension);
        while (simp->elementCount <= facetDim) {
            SimplexElem elem = random_in_range(1, maxPoint);
            
            bool unique = true;
            for (int l = 0; l < simp->elementCount; ++l) {
                SimplexElem elem2 = getElementAt(simp, l);
                if (elem == elem2) {
                    unique = false;
                    break;
                }
            }
            
            if (unique) {
                addElement(simp, elem);
            }
        }
        
        addSimplex(comp, simp);
    }
    
    return comp;
}

