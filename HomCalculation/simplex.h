//
//  simplex.h
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_simplex_h
#define HomCalculation_simplex_h

#include "strmap.h"
#include "complex_struct.h"


Complex* AllSubSimplexses(Simplex* simplex);
Simplex* simplexByExp(Simplex* simplex, int exp);

// calculation functions

Complex* FSI(Complex* A, Complex* B, int K, int V);
void Calculate_Hom(Complex* A, Complex* B);

#endif
