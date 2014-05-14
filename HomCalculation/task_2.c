//
//  task_2.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/14/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "task_2.h"



bool simplexLess(Simplex* a, Simplex* b)
{
    bool answer = false;
    
    if (a->elementCount > 0 && b->elementCount > 0 && a->elementCount < b->elementCount) {
        for (int i = 0; i < a->elementCount; ++i) {
            SimplexElem aElem = getElementAt(a, i);
            for (int j = 0; j < b->elementCount; ++j) {
                SimplexElem bElem = getElementAt(b, j);
                if (aElem == bElem) {
                    answer = true;
                    break;
                }
            }
        }
    }
    
    
    return answer;
}


char* complexPosets(Complex* comp)
{
    char* result = malloc(2048 * sizeof(char));
    sprintf(result, "");
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Complex* allSubs = AllSubSimplexses(getSimpexAt(comp, i));
        
        for (int j = 0; j < allSubs->simplexCount; ++j) {
            Simplex* simp = getSimpexAt(allSubs, j);
            for (int l = 0; l < allSubs->simplexCount; ++l) {
                Simplex* simp2 = getSimpexAt(allSubs, l);
                
                if (simplexLess(simp, simp2)) {
                    sprintf(result, "%s%s -> %s\n", result, simplexToLiteral(simp), simplexToLiteral(simp2));
                }
            }
        }
    }
    
    return result;
}