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
int random_in_range(unsigned int min, unsigned int max) {
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range = max - min,
            remainder = RAND_MAX % range,
            bucket = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) {
        return min + base_random / bucket;
    } else {
        return random_in_range(min, max);
    }
}

Complex *_generateComplex(int facetCount, int dimension, int maxPoint) {
    Complex *comp = Init_Complex();
    facetCount = random_in_range(1, (unsigned int) facetCount);
    for (int i = 0; i < facetCount; ++i) {
        Simplex *simp = Init_Simplex();

        int facetDim = random_in_range(2, (unsigned int) (dimension + 1));
        while (simp->elementCount < facetDim) {
            SimplexElem elem = random_in_range(1, (unsigned int) (maxPoint + 1));

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


Complex *generateComplex(int facetCount, int dimension, int maxPoint) {
    bool allPoints = true;
    Complex *comp = NULL;
    srand((unsigned int) time(NULL));
    int tryCount = 0;
    do {
        if (tryCount > 10) {
            tryCount = 0;
            srand((unsigned int) time(NULL));
        }
        comp = _generateComplex(facetCount, dimension, maxPoint);
        for (int i = 1; i <= maxPoint; i++) {
            SimplexElem elem = i;

            bool simplexContains = false;
            for (int j = 0; j < comp->simplexCount; ++j) {
                Simplex *simp = getSimpexAt(comp, j);

                if (containsElement(simp, elem)) {
                    simplexContains = true;
                    break;
                }
            }
            if (!simplexContains) {
                allPoints = false;
                break;
            }
        }

        if (!allPoints) {
            Dest_Complex(comp);
        }

        tryCount++;
    } while (!allPoints);

    return comp;
}

