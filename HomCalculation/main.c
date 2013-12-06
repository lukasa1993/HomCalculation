//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "simplex.h"


int main(int argc, const char * argv[])
{
    Simplex*  simplex = Init_Simplex(3, true);
    Simplex** subSimp = AllSubSimplexses(simplex);
    for (int i = 0; i < 1 << 3; ++i) {
        Simplex* simp = subSimp[i];
        for (int j = 0; j < simp->verticesCount; ++j) {
            printf(" %i ", simp->simplexRel[j]);
        }
        printf("\n");
    }
    printf("\nThe End!\n");
    return 0;
}
