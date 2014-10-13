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

#define usefilesystem false

#define HOMFVECTORSIZE 40

Complex_Storage *storage0;
Complex_Storage *storage1;

int maxK;
long long homFVector[HOMFVECTORSIZE]; // assuming that maximum dimmension would be 20
long long fVectorDim(Complex *comp);

void saveComplex(Complex *comp) {
    bool save = true;
    char *literal = complexToLiteral(comp, true);
    for (long long i = 0; i < storage1->lietralCount; ++i) {
        char *lit = getLiteralAt(storage1, i);
        if (lit != NULL && strcmp(literal, lit) == 0) {
            save = false;
            break;
        }
    }

    if (save) {
#pragma omp critical
        {
            if (comp->simplexCount == maxK) {
                long long dim = fVectorDim(comp);
                homFVector[dim]++;
            }
            addLiteral(storage1, literal);
        }
    }
}

Complex *getComplex(long long v) {
    char *literal = getLiteralAt(storage0, v);
    Complex *comp = literalToComplex(literal);
    return comp;
}

long long fVectorDim(Complex *comp) {
    long long dim = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);
        dim += (simp->elementCount - 1);
    }
    return dim;
}

bool checkSimplexSubSimplex(Simplex *simplex, Simplex *subSimplex) {
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

Complex *FSI(Complex *A, Complex *B, int K, long long V) {
    Complex *complex = NULL;
    if (K == 1) {
        int prevSubsCount = 0;

        for (int i = 0; i < B->simplexCount; i++) {
            Simplex *sim = getSimpexAt(B, i);
            int expV = prevSubsCount + (1 << sim->elementCount);
            if (V > prevSubsCount && expV > V) {
                V = V - prevSubsCount;
                if (complex == NULL || complex->simplexCount == 0) {
                    complex = Init_Complex();
                }
                addSimplex(complex, simplexByExp(sim, V));
                break;
            }
            prevSubsCount = expV - 1;
        }
    }
    else {
        complex = getComplex(V);
    }

    return complex;
}

Complex *upperSimplexContainingDot(Complex *comp, Simplex *searchSimp) {
    Complex *neibr = Init_Complex();
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);

        if (checkSimplexSubSimplex(simp, searchSimp)) {
            addSimplex(neibr, simp);
        }
    }

    return neibr;
}

Complex *mergeComplexes(Complex *a, Complex *b, bool basic) {
    if (a == NULL || a->simplexCount == 0) {
        return b;
    }
    Complex *merged = Init_Complex();
    if (a != NULL) {
        for (int i = 0; i < a->simplexCount; ++i) {
            addSimplex(merged, getSimpexAt(a, i));
        }
    }

    for (int i = 0; i < b->simplexCount; ++i) {
        bool unique = true;
        for (int j = 0; j < merged->simplexCount; ++j) {
            char *aLit = simplexToLiteral(merged->simplexes[j]);
            char *bLit = simplexToLiteral(b->simplexes[i]);
            if (strcmp(aLit, bLit) == 0) {
                unique = false;
            }
            free(aLit);
            free(bLit);
        }

        if (basic || unique) {
            addSimplex(merged, getSimpexAt(b, i));
        }
    }

    return merged;
}

Simplex *buildIntersectedSimplex(Complex *comp) {
    Simplex *intersectedSimplex = Init_Simplex();

    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);

        for (int j = 0; j < simp->elementCount; ++j) {
            SimplexElem simpElem = getElementAt(simp, j);

            for (int l = 0; l < comp->simplexCount; ++l) {
                if (l == i) {
                    continue;
                }
                Simplex *simp2 = getSimpexAt(comp, l);
                for (int m = 0; m < simp2->elementCount; ++m) {

                    SimplexElem simpElem2 = getElementAt(simp2, m);
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

Complex *unionIntersection(Complex **posibilityList, int posibilityListLength) {
    if (posibilityListLength == 1) {
        return posibilityList[0];
    }
    int *walkIndexes = calloc(posibilityListLength, sizeof(int));//malloc(posibilityListLength * sizeof(int));

    //    printf("\n-- generation start -- \n");

    Complex *unionIntersection = Init_Complex();

    bool cont = false;
    do {
        Complex *comp = Init_Complex();
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
        //        printf("\n%s\n", complexToLiteral(comp, true));

        Simplex *intersectedSimplex = buildIntersectedSimplex(comp);

        if (intersectedSimplex->elementIndex > -1) {
            addSimplex(unionIntersection, intersectedSimplex);
        } else {
            Dest_Simplex(intersectedSimplex);
        }

        Light_Dest_Complex(comp);
    } while (cont);

    free(walkIndexes);
    return unionIntersection;
}

int CalculatePoints(Complex *comp) {
    SimplexElem elem = -1;

    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);
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
        Simplex *sim = comp->simplexes[i];
        points += sim->elementCount;
    }
    Simplex *tempSim = Init_Simplex();
    points = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *sim = comp->simplexes[i];
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
                }
                else if (elem2 == elem1) {
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


void Hom_Match(Complex *A, Complex *B, Complex *P, int k) {
    char* charA = complexToLiteral(A, true);
    char* charB = complexToLiteral(B, true);
    char* charP = complexToLiteral(P, true);

    Simplex *temp = Init_Simplex();
    addElement(temp, k);

    // dots in aneibr must be less then k
    Complex *ANeibrTemp = upperSimplexContainingDot(A, temp);
    Dest_Simplex(temp);

    char* charANeibrTemp = complexToLiteral(ANeibrTemp, true);

    Complex *ANeibr = Init_Complex();
    for (int i = 0; i < ANeibrTemp->simplexCount; ++i) {
        Simplex *aNeibrSim = getSimpexAt(ANeibrTemp, i);

        Simplex *aNebrSimplex = Init_Simplex();

        for (int j = 0; j < aNeibrSim->elementCount; ++j) {
            SimplexElem elem = getElementAt(aNeibrSim, j);
            if (elem < k) {
                addElement(aNebrSimplex, elem);
            }
        }

        if (aNebrSimplex->elementCount > 0) {
            addSimplex(ANeibr, aNebrSimplex);
        } else {
            Dest_Simplex(aNebrSimplex);
        }
    }
    Light_Dest_Complex(ANeibrTemp);

    size_t posibleSize = (size_t) (A->simplexCount + B->simplexCount + P->simplexCount);
    Complex **posibilityList = calloc(posibleSize, sizeof(Complex *));
    int posibilityListLength = 0;

    for (int i = 0; i < ANeibr->simplexCount; ++i) {
        Simplex *aNeibrSim = getSimpexAt(ANeibr, i);
        Simplex *fsiAT = Init_Simplex();

        for (int j = 0; j < aNeibrSim->elementCount; ++j) {
            SimplexElem elem = getElementAt(aNeibrSim, j);

            Simplex *pSimp = getSimpexAt(P, elem - 1);
            for (int l = 0; l < pSimp->elementCount; ++l) {
                SimplexElem pElem = getElementAt(pSimp, l);
                if (!containsElement(fsiAT, pElem)) {
                    addElement(fsiAT, pElem);
                }
            }
        }

        if (fsiAT->elementCount > 0) {
            Complex *comp = upperSimplexContainingDot(B, fsiAT);

            posibilityList[posibilityListLength] = comp;
            posibilityListLength++;
        }
    }

    Complex *BNeibr = unionIntersection(posibilityList, posibilityListLength);

//    printf("\n%s <|> %s\n", complexToLiteral(P, true), complexToLiteral(BNeibr, true));

    for (int i = 0; i < BNeibr->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(BNeibr, i);
        Complex *simpSubs = AllSubSimplexses(simp);

        for (int j = 0; j < simpSubs->simplexCount; ++j) {
            Simplex *subSimp = getSimpexAt(simpSubs, j);
            if (subSimp->elementCount > 0) {
                Complex *temp1 = Init_Complex();

                addSimplex(temp1, subSimp);

                Complex *M1Complex = mergeComplexes(P, temp1, true);

                saveComplex(M1Complex);
                Light_Dest_Complex(M1Complex);
                Dest_Complex(temp1);
            }
        }
    }

    if (posibilityListLength > 1) {
        for (int i = 0; i < posibilityListLength; ++i) {
            Light_Dest_Complex(posibilityList[i]);
        }
    }

    if (posibilityListLength > 1) {
        Dest_Complex(BNeibr);
    } else {
        Light_Dest_Complex(BNeibr);
    }
    Light_Dest_Complex(ANeibr);


    free(posibilityList);
}

Simplex *fVectorFromComplex(Complex *comp) {
    Complex *fComplex = Init_Complex();
    Simplex *AfVector = Init_Simplex();

    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);
        Complex *simpSubs = AllSubSimplexses(simp);
        //        printf("\n%s\n", complexToLiteral(simpSubs, true));
        for (int j = 0; j < simpSubs->simplexCount; ++j) {
            Simplex *pSimp = getSimpexAt(simpSubs, j);
            //          x  printf("\n%s\n", simplexToLiteral(pSimp));
            if (!containsSimplex(fComplex, pSimp)) {
                addSimplex(fComplex, pSimp);
            }
            else {
                //                Dest_Simplex(pSimp);
            }
        }
    }

    for (int i = 0; i < fComplex->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(fComplex, i);
        if (simp->elementCount < 1) {
            continue;
        }
        while (AfVector->elementIndex < simp->elementCount - 1) {
            addElement(AfVector, 0);
        }
        AfVector->elements[simp->elementCount - 1]++;
    }


    //    printf("\n%s\n", complexToLiteral(fComplex, true));

    Dest_Complex(fComplex);

    return AfVector;
}

void Calculate_Hom(Complex *A, Complex *B) {
    for (int i = 0; i < HOMFVECTORSIZE; ++i) {
        homFVector[i] = 0;
    }

    storage0 = Init_Storage();
    storage1 = Init_Storage();

    int points = CalculatePoints(A);

    maxK = points;

    Simplex *fVA = fVectorFromComplex(A);
    Simplex *fVB = fVectorFromComplex(B);

    char *fVALit = simplexToLiteral(fVA);
    char *fVBLit = simplexToLiteral(fVB);

    printf("\nAF: %s\n", fVALit);
    printf("\nBF: %s\n", fVBLit);

    long long k1 = 0;
    for (int i = 0; i < A->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(A, i);
        Complex *simpSubs = AllSubSimplexses(simp);
        k1 += simpSubs->simplexCount;

        Dest_Complex(simpSubs);
    }

    storage0->lietralCount = k1;
    for (int k = 2; k <= points; ++k) {
#pragma omp parallel for shared(A, B, storage0, storage1, k)
        for (long long V1 = 0; V1 < storage0->lietralCount; ++V1) {
            Complex *P = NULL;
#pragma omp critical
            {
                P = FSI(A, B, k - 1, V1);
            }
            if (P != NULL && P->simplexCount > 0) {

                Hom_Match(A, B, P, k);

                Dest_Complex(P);
            }

        }

#pragma omp barrier

        printf("\n%d => %lld\n", k, storage1->lietralCount);
        fflush(stdout);

        if (k == 2) {
            storage0->lietralCount = 0;
        }
        Destory_Storage(storage0);
        storage0 = storage1;
        storage1 = Init_Storage();
    }
    Destory_Storage(storage1);

    printf("\n\n F-Vector: [");
    for (int i = 0; i < HOMFVECTORSIZE; ++i) {
        if (homFVector[i] == 0) continue;
        printf("%lld", homFVector[i]);
        if (i + 1 != HOMFVECTORSIZE && homFVector[i + 1] != 0) {
            printf(", ");
        }
    }
    printf("]\n\n");

    // hdd-return

    printf("\n\n Safe House \n\n");
    fflush(stdout);

    char *a = malloc(1024);
    sprintf(a, "./hom_safe( %s - %s) ", complexToLiteral(A, true), complexToLiteral(B, true));

    LD_File *file1 = Init_file_util_ext(a, "txt", false);
    for (long long V1 = 0; V1 < storage0->lietralCount; ++V1) {
        wrtieLine(file1, getLiteralAt(storage0, V1), false);
    }
    return; // temporarry
    printf("\n\n Generation Result File \n\n");
    fflush(stdout);

    LD_File *file = Init_file_util_ext("./hom_result", "txt", false);


    int bPoints = CalculatePoints(B);
    Complex *posetPrep = Init_Complex();

    for (long long V1 = 0; V1 < storage0->lietralCount; ++V1) {
        Complex *P = FSI(A, B, points, V1);
        if (P != NULL && P->simplexCount > 0) {
            Simplex *tmp = Init_Simplex();

            for (int i = 0; i < P->simplexCount; ++i) {
                Simplex *simp = getSimpexAt(P, i);
                for (int j = 0; j < simp->elementCount; ++j) {
                    addElement(tmp, ((i * bPoints) + getElementAt(simp, j)));
                }
            }

            if (!containsSimplex(posetPrep, tmp)) {
                addSimplex(posetPrep, tmp);
                int maxDim = 0;
                int maxDimCount = 0;
                char *maxSim;
                for (int i = 0; i < P->simplexCount; ++i) {
                    Simplex *simp = getSimpexAt(P, i);
                    if (maxDim < simp->elementCount) {
                        maxDim = simp->elementCount;
                        maxDimCount = 1;
                        maxSim = simplexToLiteral(simp);
                    } else {
                        maxSim = malloc(sizeof(char));
                    }
                    char *simpLit = simplexToLiteral(simp);
                    if (maxDim == simp->elementCount && maxSim != NULL && strcmp(maxSim, simpLit) != 0) {
                        maxDimCount++;
                    }

                    free(maxSim);
                    free(simpLit);
                }

            } else {
                Dest_Simplex(tmp);
            }

            Dest_Complex(P);
        }
    }

    char *complexLiteral = complexToLiteral(posetPrep, true);
    wrtieLine(file, "RequirePackage(\"homology\");", false);
    wrtieLine(file, "SimplicialHomology(OrderComplex(OrderRelationToPoset(", true);
    wrtieLine(file, complexLiteral, true);
    wrtieLine(file, ",IsSubset)));", false);

    printf("\nResult is in:\n %s\n", file->path);

    Destroy_file(file);
    Destory_Storage(storage0);
    Dest_Complex(posetPrep);
    Dest_Simplex(fVA);
    Dest_Simplex(fVB);
    free(fVALit);
    free(fVBLit);
}




