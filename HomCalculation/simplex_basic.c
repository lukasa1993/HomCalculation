//
//  simplex_basic.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/14/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "simplex_basic.h"


Simplex *simplexByExp(Simplex *simplex, long long exp) {
    long long tmpA = exp;
    Simplex *simpl = Init_Simplex();
    for (int j = 0; j < (8 * sizeof(int)); ++j) {
        if (tmpA & 1 && j < simplex->elementCount) {
            addElement(simpl, getElementAt(simplex, j));
        }
        tmpA >>= 1;
    }

    return simpl;
}


Complex *AllSubSimplexses(Simplex *simplex) {
    long long i, a = 1 << simplex->elementCount;
    Complex *complex = Init_Complex();
    for (i = 0; i < a; ++i) {
        Simplex *simpl = simplexByExp(simplex, i);
        addSimplex(complex, simpl);
    }

    return complex;
}




