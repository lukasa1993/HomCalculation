//
//  simplex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include "simplex.h"

#define method1    true // unionIntersection
#define method2    false  // intersectionUnionUpper
#define methodComp false // both

StrMap *sm;

void saveComplex(Complex* comp, int k, int v) {
    char str_k[100];
    sprintf(str_k, "%d", k);
    char str_V[100];
    sprintf(str_V, "%d", v);
    
    char *key = "_";
    key = concat(str_k, key);
    key = concat(key, str_V);
    
    //    printf("\n----- Saving Complex %s for %s -----\n", complexToLiteral(comp, true), key);
    
    sm_put(sm, key, complexToLiteral(comp, false));
}

Complex* getComplex(int k, int v) {
    char str_k[100];
    sprintf(str_k, "%d", k);
    char str_V[100];
    sprintf(str_V, "%d", v);
    
    char *key = "_";
    key = concat(str_k, key);
    key = concat(key, str_V);
    
    char buf[4096];
    sm_get(sm, key, buf, sizeof(buf));
    
    //    printf("\n----- Getting Complex %s for %s -----\n", buf, key);
    
    return literalToComplex(buf);
}

bool checkSimplexSubSimplex(Simplex* simplex, Simplex* subSimplex) {
    bool result = true;
    for (int i = 0; i < subSimplex->elementCount; ++i) {
        SimplexElem subSimElem = getElementAt(subSimplex, i);
        
        bool elemMatch = false;
        for (int j = 0; j < simplex->elementCount; ++j) {
            SimplexElem simElem = getElementAt(simplex, j);
            
            if (subSimElem == simElem) {
                elemMatch = true;
                break;
            }
        }
        if (!elemMatch) {
            result = false;
            break;
        }
    }
    
    return result;
}

Complex* FSI(Complex* A, Complex* B, int K, int V) {
    Complex* complex = Init_Complex();
    if (K == 1) {
        int prevSubsCount = 0;
        
        for (int i = 0; i < B->simplexCount; i++) {
            Simplex* sim = getSimpexAt(B, i);
            int expV = prevSubsCount + (1 << sim->elementCount);
            if (V > prevSubsCount && expV > V) {
                V = V - prevSubsCount;
                if (complex->simplexCount == 0) {
                    complex = Init_Complex();
                }
                addSimplex(complex, simplexByExp(sim, V));
                break;
            }
            prevSubsCount = expV - 1;
        }
    } else {
        complex = getComplex(K, V);
    }
    
    return complex;
}

Complex* upperSimplexContainingDot(Complex* comp, Simplex* searchSimp) {
    Complex* neibr   = Init_Complex();
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp = comp->simplexes[i];
        
        if (checkSimplexSubSimplex(simp, searchSimp)) {
            addSimplex(neibr, simp);
        }
    }
    
    return neibr;
}

Complex* mergeComplexes(Complex* a, Complex* b, bool basic) {
    if (a->simplexCount == 0) {
        return b;
    }
    Complex* merged        = Init_Complex();
    for (int i = 0; i < a->simplexCount; ++i) {
        addSimplex(merged, getSimpexAt(a, i));
    }
    
    for (int i = 0; i < b->simplexCount; ++i) {
        bool unique = true;
        for (int  j = 0; j < merged->simplexCount; ++j) {
            if (strcmp(simplexToLiteral(merged->simplexes[j]), simplexToLiteral(b->simplexes[i])) == 0) {
                unique = false;
            }
        }
        
        if (basic || unique) {
            addSimplex(merged, getSimpexAt(b, i));
        }
    }
    
    return merged;
}

Simplex* buildIntersectedSimplex(Complex* comp) {
    Simplex* intersectedSimplex      = Init_Simplex();
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp = getSimpexAt(comp, i);
        for (int j = 0; j < simp->elementCount; ++j) {
            SimplexElem simpElem = getElementAt(simp, j);
            
            for (int l = 0; l < comp->simplexCount; ++l) {
                if (l == i) {
                    continue;
                }
                Simplex* simp2 = comp->simplexes[l];
                for (int m = 0; m < simp2->elementCount; ++m) {
                    SimplexElem simpElem2 = simp2->elements[m];
                    if (simpElem == simpElem2) {
                        bool unique = true;
                        for (int checkIndex = 0; checkIndex < intersectedSimplex->elementCount; ++checkIndex) {
                            SimplexElem checkElem = getElementAt(intersectedSimplex, checkIndex);
                            if (checkElem == simpElem) {
                                unique = false;
                            }
                        }
                        
                        if (unique) {
                            addElement(intersectedSimplex, simpElem);
                        }
                    }
                }
            }
            
        }
    }
    
    return intersectedSimplex;
}

Complex* unionIntersection(Complex** posibilityList, int posibilityListLength) {
    int*     walkIndexes = malloc(posibilityListLength * sizeof(int));
    memset(walkIndexes, 0, posibilityListLength * sizeof(int));
    
    //    printf("\n-- generation start -- \n");
    
    Complex* unionIntersection      = Init_Complex();
    
    bool cont = false;
    do {
        Complex* comp  = Init_Complex();
        for (int i = 0; i < posibilityListLength; ++i) {
            addSimplex(comp, getSimpexAt(posibilityList[i], walkIndexes[i]));
        }
        
        
        cont = false;
        for (int i = 0; i < posibilityListLength; ++i) {
            if (walkIndexes[i] < posibilityList[i]->simplexIndex) {
                cont = true;
            }
        }
        
        for (int i = posibilityListLength - 1; i >= 0; --i) {
            if (walkIndexes[i] + 1 < posibilityList[i]->simplexCount) {
                walkIndexes[i]++;
                break;
            }
        }
        
        Simplex* intersectedSimplex = buildIntersectedSimplex(comp);
        if (intersectedSimplex->elementIndex > -1) {
            addSimplex(unionIntersection, intersectedSimplex);
        }
    } while (cont);
    
    return unionIntersection;
}

Complex* intersectionUnionUpper(Complex* B, Complex** posibilityList, int posibilityListLength) {
    return NULL;
}

int CalculatePoints(Complex* comp) {
    SimplexElem elem = -1;
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp = getSimpexAt(comp, i);
        for (int j = 0; j < simp->elementCount; ++j) {
            SimplexElem elemMax = getElementAt(simp, j);
            if (elemMax > elem) {
                elem = elemMax;
            }
        }
    }
    return elem;
    
    int points = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* sim = comp->simplexes[i];
        points += sim->elementCount;
    }
    Simplex* tempSim = Init_Simplex();
    points = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* sim = comp->simplexes[i];
        if (sim == NULL) {
            continue;
        }
        for (int j = 0; j < sim->elementCount; ++j) {
            SimplexElem elem1 = sim->elements[j];
            
            int insertIndex = 0;
            bool checkUnique = true;
            for (int k = 0; k < tempSim->elementCount; ++k) {
                SimplexElem elem2 = tempSim->elements[k];
                if (elem2 == 0) {
                    insertIndex = k;
                    break;
                } else if(elem2 == elem1) {
                    checkUnique = false;
                    break;
                }
            }
            if (checkUnique) {
                tempSim->elements[insertIndex] = elem1;
                points++;
            }
        }
    }
    
    //    printf("\n%s\n", simplexToLiteral(tempSim));
    
    return points;
}


int Hom_Match(Complex* A, Complex* B, Complex* P, int k, int V) {
    Simplex* temp   = Init_Simplex();
    addElement(temp, k);
    
    Complex* ANeibr = upperSimplexContainingDot(A, temp);
    
    Complex** posibilityList       = malloc(P->simplexCount * sizeof(Complex));
    int       posibilityListLength = 0;
    
    
    Complex* BNeibr = Init_Complex();
    
    for (int i = 0; i < ANeibr->simplexCount; ++i) {
        Simplex* aNeibrSim  = getSimpexAt(ANeibr, i);
        Complex* BNeibrTemp = Init_Complex();
        
        for (int j = 0; j < aNeibrSim->elementCount; ++j) {
            SimplexElem elem = getElementAt(aNeibrSim, j);
            int    elemIndex = elem - 1;
            
            if (elem != (SimplexElem) k && elemIndex < P->simplexCount) {
                Simplex* tempSim = getSimpexAt(P, elemIndex);
                Complex* comp    = upperSimplexContainingDot(B, tempSim);
                BNeibrTemp       = mergeComplexes(BNeibrTemp, comp, false);
                
                posibilityList[posibilityListLength]    = BNeibrTemp;
                posibilityListLength++;
            }
        }
    }
    
    if (posibilityListLength > 1) {
        if (method1) {
            BNeibr = unionIntersection(posibilityList, posibilityListLength);
        } else if(method2) {
            BNeibr = intersectionUnionUpper(B, posibilityList, posibilityListLength);
        } else if(methodComp) {
            Complex* BNeibr1 = unionIntersection(posibilityList, posibilityListLength);
            Complex* BNeibr2 = intersectionUnionUpper(B, posibilityList, posibilityListLength);
            printf("\n %s \n %s \n", complexToLiteral(BNeibr1, true), complexToLiteral(BNeibr2, true));
            if (strcasecmp(complexToLiteral(BNeibr1, false), complexToLiteral(BNeibr2, false)) != 0) {
                printf("vai");
            } else {
                printf("\nYEA\n");
            }
        }
    } else if (posibilityListLength > 0){
        BNeibr = posibilityList[0];
    }
    
    for (int j = 0; j < BNeibr->simplexCount; ++j) {
        Simplex* simp = getSimpexAt(BNeibr, j);
        
        for (int l = 0; l < simp->elementCount; ++l) {
            Complex* temp      = Init_Complex();
            
            addSimplex(temp, Init_Simplex());
            
            addElement(getSimpexAt(temp, 0), getElementAt(simp, l));
            
            saveComplex(mergeComplexes(P, temp, true), k, V);
            //            Dest_Complex(temp);
            
            
            V++;
        }
        
        
        
        Complex* temp = Init_Complex();
        addSimplex(temp, simp);
        saveComplex(mergeComplexes(P, temp, true), k, V);
        //        Dest_Complex(temp);
        V++;
        
    }
    return V;
}

void Calculate_Hom(Complex* A, Complex* B) {
    int points = CalculatePoints(A);
    
    sm = sm_new(points);
    int lastV = 0;
    for (int k = 2; k <= points; ++k) {
        int V1 = 1, V = 1;
        Complex* P = Init_Complex();
        do {
            P = FSI(A, B, k - 1, V1);
            if (P != NULL && P->simplexCount > 0) {
                //                printf("\nP -> %s,K = %d, V1 = %d\n", complexToLiteral(P, true), k, V1);
                V = Hom_Match(A, B, P, k, V);
                V1++;
            } else {
                saveComplex(P, k, V);
            }
            lastV = V1;
        } while (P != NULL && P->simplexCount > 0);
        printf("");
    }
    
    printf("\n\n Generation Result File \n\n");
    fflush(stdout);
    
    LD_File* file = Init_file_util_ext("./hom_result", "txt", false);
    
    int bPoints = CalculatePoints(B);
    int V1 = 1, count = 0;
    Complex* P = NULL;
    Complex* posetPrep = Init_Complex();
    do {
        P = FSI(A, B, points, V1);
        if (P != NULL && P->simplexCount > 0) {
            Simplex* tmp = Init_Simplex();
            count++;
            for (int i = 0; i < P->simplexCount; ++i) {
                Simplex* simp = getSimpexAt(P, i);
                for (int j = 0; j < simp->elementCount; ++j) {
                    addElement(tmp, ( (i * bPoints) + getElementAt(simp, j)) );
                }
            }
            if (!containsSimplex(posetPrep, tmp)) {
                addSimplex(posetPrep, tmp);
            }
            V1++;
        }
    } while (P != NULL && P->simplexCount > 0);
    
    char* complexLiteral = complexToLiteral(posetPrep, true);
    wrtieLine(file, "RequirePackage(\"homology\");", false);
    wrtieLine(file, "SimplicialHomology(OrderComplex(OrderRelationToPoset(", true);
    wrtieLine(file, complexLiteral, true);
    wrtieLine(file, ",IsSubset)));", false);
    
    char actualPath[1024];
    realpath(file->path, actualPath);
    
    printf("\nResult is in:\n %s\n", actualPath);
    
    if(false) {
        for (int k = 2; k <= points; ++k) {
            int V1 = 1;
            Complex* P = NULL;
            do {
                P = FSI(A, B, k, V1);
                if (P != NULL && P->simplexCount > 0) {
                    printf("\nFSI[%d] = %s\n", V1, complexToLiteral(P, true));
                    if (k == points) {
                        char line[1024];
                        sprintf(line, "\nFSI[%d] = %s", V1, complexToLiteral(P, true));
                        wrtieLine(file, line, false);
                    }
                    V1++;
                }
            } while (P != NULL && P->simplexCount > 0);
        }
    }
    
    Destroy_file(file);
}




