//
//  simplex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include "simplex.h"

#define HOMFVECTORSIZE 40

Complex_Storage *storage0;
Complex_Storage *storage1;

int       maxK;
long long homFVector[HOMFVECTORSIZE]; // assuming that maximum dimmension would be 20
long long fVectorDim(Complex *comp);

static inline void DoProgress(char label[], int step, int total);

void saveComplex(Complex *comp) {
    if (comp->simplexCount == maxK) {
        char      *literal = complexToLiteral(comp, true);
        long long dim      = fVectorDim(comp);

        homFVector[dim]++;
        addLiteral(storage1, literal);
    }
//    printf("\n%s\n", literal);
}

Complex *getComplex(long long v) {
    char    *literal = getLiteralAt(storage0, v);
    Complex *comp    = literalToComplex(literal);
    return comp;
}

long long fVectorDim(Complex *comp) {
    long long dim = 0;
    for (int  i   = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);
        dim += (simp->elementCount - 1);
    }
    return dim;
}

bool checkSimplexSubSimplex(Simplex *simplex, Simplex *subSimplex) {
    bool     result = true;
    for (int i      = 0; i < subSimplex->elementCount; ++i) {
        SimplexElem subSimElem = getElementAt(subSimplex, i);

        bool elemMatch = false;
        for (int    j          = 0; j < simplex->elementCount; ++j) {
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
    Complex *complex = Init_Complex();
    if (A->simplexCount > 0 && K == 1) {

        for (int i = 0; i < B->simplexCount; i++) {
            Simplex *sim  = getSimpexAt(B, i);
            Complex *subs = sim->allowedSubSimplexes;
            addSimplex(complex, getSimpexAt(subs, (int) V));
        }
    }
    else {
        Dest_Complex(complex);
        complex = getComplex(V);
    }

    return complex;
}

Complex *upperSimplexContainingDot(Complex *comp, Simplex *searchSimp) {
    Complex  *neibr = Init_Complex();
    for (int i      = 0; i < comp->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(comp, i);

        if (checkSimplexSubSimplex(simp, searchSimp)) {
            Simplex* tmpSimp = Init_Simplex();
            for (int j      = 0; j < simp->elementCount; ++j) {
                addElement(tmpSimp, getElementAt(simp, j));
            }
            addSimplex(neibr, tmpSimp);
        }
    }

    return neibr;
}

Complex *mergeComplexes(Complex *a, Complex *b, bool basic) {
    if (a == NULL || a->simplexCount == 0) {
        return b;
    }
    Complex  *merged = Init_Complex();
    for (int i       = 0; i < a->simplexCount; ++i) {
        addSimplex(merged, compySimplex(getSimpexAt(a, i)));
    }

    for (int i = 0; i < b->simplexCount; ++i) {
        bool     unique = true;
        for (int j      = 0; j < merged->simplexCount; ++j) {
            char *aLit = simplexToLiteral(merged->simplexes[j]);
            char *bLit = simplexToLiteral(b->simplexes[i]);
            if (strcmp(aLit, bLit) == 0) {
                unique = false;
            }
            free(aLit);
            free(bLit);
        }

        if (basic || unique) {
            addSimplex(merged, compySimplex(getSimpexAt(b, i)));
        }
    }

    return merged;
}

Simplex *buildIntersectedSimplex(Complex *comp) {
    Simplex *intersectedSimplex = Init_Simplex();

//    for (int i = 0; i < comp->simplexCount; ++i) {
    Simplex  *simp = getSimpexAt(comp, 0);
    for (int j     = 0; j < simp->elementCount; ++j) {
        SimplexElem simpElem = getElementAt(simp, j);
        bool isInAll = true;
        for (int    l        = 1; l < comp->simplexCount; ++l) {
            Simplex *simp2 = getSimpexAt(comp, l);

            if (!containsElement(simp2, simpElem)) {
                isInAll = false;
            }
        }

        if (isInAll) {
            addElement(intersectedSimplex, simpElem);
        }
    }

//    }

    return intersectedSimplex;
}

Complex *unionIntersection(Complex **posibilityList, int posibilityListLength) {
    if (posibilityListLength == 1) {
        return posibilityList[0];
    }
    int  *walkIndexes = calloc((size_t) posibilityListLength, sizeof(int));//malloc(posibilityListLength * sizeof(int));

    // printf("\n-- generation start -- \n");

    Complex *unionIntersection = Init_Complex();

    bool cont;
    do {
        Complex  *comp = Init_Complex();
        for (int i     = 0; i < posibilityListLength; ++i) {
            addSimplex(comp, compySimplex(getSimpexAt(posibilityList[i], walkIndexes[i])));
        }


        cont = false;
        for (int i = 0; i < posibilityListLength; ++i) {
            if (walkIndexes[i] < posibilityList[i]->simplexCount - 1) {
                cont = true;
            }
        }

        for (int i = posibilityListLength - 1; i >= 0; --i) {

            if (walkIndexes[i] + 1 < posibilityList[i]->simplexCount) {
                walkIndexes[i]++;
                break;
            }
            else if (i > 0) {
                walkIndexes[i] = 0;
            }
        }

        //  printf("\n%s\n", complexToLiteral(comp, true));

        Simplex *intersectedSimplex = buildIntersectedSimplex(comp);

        //  printf("\n%s\n", simplexToLiteral(intersectedSimplex));

        if (intersectedSimplex->elementIndex > -1) {
            addSimplex(unionIntersection, intersectedSimplex);
        }
        else {
            Dest_Simplex(intersectedSimplex);
        }

        Dest_Complex(comp);
    }
    while (cont);

    free(walkIndexes);
    return unionIntersection;
}

int CalculatePoints(Complex *comp) {
    SimplexElem elem = -1;

    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex  *simp = getSimpexAt(comp, i);
        for (int j     = 0; j < simp->elementCount; ++j) {
            SimplexElem elemMax = getElementAt(simp, j);
            if (elemMax > elem) {
                elem = elemMax;
            }
        }
    }
    return elem;
}


Complex_Storage *Hom_Match(Complex *A, Complex *B, Complex *P, int k) {
    Simplex *temp = Init_Simplex();
    addElement(temp, k);

    // dots in aneibr must be less then k
    Complex *ANeibrTemp = upperSimplexContainingDot(A, temp);
    Dest_Simplex(temp);

    Complex  *ANeibr = Init_Complex();
    for (int i       = 0; i < ANeibrTemp->simplexCount; ++i) {
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
        }
        else {
            Dest_Simplex(aNebrSimplex);
        }
    }
    Dest_Complex(ANeibrTemp);

    size_t  posibleSize          = (size_t) (A->simplexCount + B->simplexCount + P->simplexCount);
    Complex **posibilityList     = calloc(posibleSize, sizeof(Complex *));
    int     posibilityListLength = 0;

    for (int i = 0; i < ANeibr->simplexCount; ++i) {
        Simplex *aNeibrSim = getSimpexAt(ANeibr, i);
        Simplex *fsiAT     = Init_Simplex();

        for (int j = 0; j < aNeibrSim->elementCount; ++j) {
            SimplexElem elem = getElementAt(aNeibrSim, j);

            Simplex  *pSimp = getSimpexAt(P, elem - 1);
            for (int l      = 0; l < pSimp->elementCount; ++l) {
                SimplexElem pElem = getElementAt(pSimp, l);
                if (!containsElement(fsiAT, pElem)) {
                    addElement(fsiAT, pElem);
                }
            }
        }

        if (fsiAT->elementCount > 0) {
            Complex *comp = upperSimplexContainingDot(B, fsiAT);
            if (comp->simplexCount > 0) {
                Simplex *first = getSimpexAt(comp, 0);

                if (first->elementCount > 0) {
                    posibilityList[posibilityListLength] = comp;
                    posibilityListLength++;
                }
            }
        }
        Dest_Simplex(fsiAT);
    }

//    LD_File *second_log = Init_file_util_ext("./log2", "txt", false);
//    char *a = malloc(1024);
//
//    if (k == CalculatePoints(A) && a != NULL) {
//        sprintf(a, "\n-  FSI: %s\n", complexToLiteral(P, true));
//        wrtieLine(second_log, a, false);
//        for (int i = 0; i < posibilityListLength; ++i) {
//            sprintf(a, "\n-- BT%d %s \n", i, complexToLiteral(posibilityList[i], true));
//            wrtieLine(second_log, a, false);
//        }
//    }

    Complex_Storage *storage = Init_Storage();
    if (posibilityListLength == 0) {
        Dest_Complex(ANeibr);
        free(posibilityList);
        return storage;
    }

    Complex *BNeibr = unionIntersection(posibilityList, posibilityListLength);
//    if (k == CalculatePoints(A) && a != NULL) {
//        sprintf(a, "\n---  IBT: %s\n", complexToLiteral(BNeibr, true));
//        wrtieLine(second_log, a, false);
//    }
//
//    Destroy_file(second_log);
//    free(a);

    for (int i = 0; i < BNeibr->simplexCount; ++i) {
        Simplex *simp     = getSimpexAt(BNeibr, i);
        Simplex *bsimp    = getFacet(B, simp);
        Complex *simpSubs = bsimp->allowedSubSimplexes;


        for (int j = 0; j < simpSubs->simplexCount; ++j) {
            Simplex *subSimp = getSimpexAt(simpSubs, j);
            bool allowed     = containsSubSimplex(simp, subSimp);

            if (subSimp->elementCount > 0 && allowed) {
                Complex *temp1 = Init_Complex();

                addSimplex(temp1, compySimplex(subSimp));

                Complex *M1Complex = mergeComplexes(P, temp1, true);
                char    *literal   = complexToLiteral(M1Complex, true);

                if (!containsLiteral(storage, literal)) {
                    addLiteral(storage, literal);

                    saveComplex(M1Complex);
                } else {
                    free(literal);
                }

                Dest_Complex(M1Complex);
                Dest_Complex(temp1);
            }
        }
    }
//    Destory_Storage(storage);


    if (posibilityListLength > 1) {
        for (int i = 0; i < posibilityListLength; ++i) {
            Dest_Complex(posibilityList[i]);
        }
    }

    Dest_Complex(BNeibr);
    Dest_Complex(ANeibr);


    free(posibilityList);

    return storage;
}

Simplex *fVectorFromComplex(Complex *comp) {
    Complex *fComplex = Init_Complex();
    Simplex *AfVector = Init_Simplex();

    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex  *simp     = getSimpexAt(comp, i);
        Complex  *simpSubs = AllSubSimplexses(simp);
        //        printf("\n%s\n", complexToLiteral(simpSubs, true));
        for (int j         = 0; j < simpSubs->simplexCount; ++j) {
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

void df_hom_match(Complex *A, Complex *B, Complex_Storage *storage, int k) {
    for (int i = 0; i < storage->lietralCount; ++i) {
        char *PLiteral = getLiteralAt(storage, i);
        if (PLiteral == NULL) continue;
        Complex *P = literalToComplex(PLiteral);

        Complex_Storage *next_storage = Hom_Match(A, B, P, k);
        Dest_Complex(P);
        if (k <= maxK) {
            df_hom_match(A, B, next_storage, k + 1);
        }
        Destory_Storage(next_storage);

    }
}

void Calculate_Hom(Complex *A, Complex *B) {
    printf("\n------- Calculation ------- \n");
    printf("%s -> %s \n", complexToLiteral(A, true), complexToLiteral(B, true));


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

    LD_File *file1 = Init_file_util_ext("./hom_safe", "txt", false);
    wrtieLine(file1, complexToLiteral(A, true), true);
    wrtieLine(file1, " -> ", true);
    wrtieLine(file1, complexToLiteral(B, true), false);

    for (int i = 0; i < B->simplexCount; ++i) {
        Simplex  *simp = getSimpexAt(B, i);
        Complex  *subs = simp->allowedSubSimplexes;
        for (int j     = 0; j < subs->simplexCount; ++j) {
            Complex *comp = Init_Complex();
            addSimplex(comp, getSimpexAt(subs, j));
            if (comp->simplexCount > 0 && comp->simplexes[0]->elementCount > 0) {
                char *literal = complexToLiteral(comp, true);
                if (!containsLiteral(storage0, literal))
                    addLiteral(storage0, literal);
            }
            Light_Dest_Complex(comp);
        }
    }


    for (long long V1 = 0; V1 < storage0->lietralCount; ++V1) {
        Complex         *P            = getComplex(V1);
        Complex_Storage *startStorage = Init_Storage();
        addLiteral(startStorage, complexToLiteral(P, true));

        df_hom_match(A, B, startStorage, 2);

        Destory_Storage(startStorage);

        for (long long V11 = 0; V11 < storage1->lietralCount; ++V11) {
            wrtieLine(file1, getLiteralAt(storage1, V11), false);
        }

        Destory_Storage(storage1);
        storage1 = Init_Storage();
    }

    int      len = 0;
    for (int i   = HOMFVECTORSIZE - 1; i >= 0; --i) {
        if (homFVector[i] > 0) {
            len = i + 1;
            break;
        }
    }

    char *fvectorstr = "";
    fvectorstr = concat(fvectorstr, "\n\n F-Vector: [");
    printf("\n\n F-Vector: [");
    for (int i = 0; i < len; ++i) {
        char tmp[10];
        sprintf(tmp, "%lld", homFVector[i]);
        fvectorstr = concat(fvectorstr, tmp);
        printf("%lld", homFVector[i]);
        if (i != len - 1) {
            fvectorstr = concat(fvectorstr, ", ");
            printf(", ");
        }
    }
    printf("]\n\n");
    fvectorstr = concat(fvectorstr, "]\n\n");
    wrtieLine(file1, fvectorstr, false);

    printf("\n\n Safe House \n\n");
    fflush(stdout);

//    LD_File *file1 = Init_file_util_ext("./hom_safe", "txt", false);
//    wrtieLine(file1, complexToLiteral(A, true), true);
//    wrtieLine(file1, " -> ", true);
//    wrtieLine(file1, complexToLiteral(B, true), false);
//    wrtieLine(file1, fvectorstr, false);
//    for (long long V1 = 0; V1 < storage0->lietralCount; ++V1) {
//        wrtieLine(file1, getLiteralAt(storage0, V1), false);
//    }


    if (true)   // end fast
    {
        Destory_Storage(storage0);
        Dest_Simplex(fVA);
        Dest_Simplex(fVB);
        free(fVALit);
        free(fVBLit);

    }
    else {

        printf("\n\n Generation Result File \n\n");
        fflush(stdout);

        LD_File *file = Init_file_util_ext("./hom_result", "txt", false);


        int     bPoints    = CalculatePoints(B);
        Complex *posetPrep = Init_Complex();

        for (
                long long V1 = 0;
                V1 < storage0->
                        lietralCount;
                ++V1) {
            Complex *P = FSI(A, B, points, V1);
            if (P != NULL && P->simplexCount > 0) {
                Simplex *tmp = Init_Simplex();

                for (
                        int i = 0;
                        i < P->
                                simplexCount;
                        ++i) {
                    Simplex     *simp = getSimpexAt(P, i);
                    for (
                            int j     = 0;
                            j < simp->
                                    elementCount;
                            ++j) {
                        addElement(tmp, ((i
                                          * bPoints) +
                                         getElementAt(simp, j
                                         )));
                    }
                }

                if (!
                        containsSimplex(posetPrep, tmp
                        )) {
                    addSimplex(posetPrep, tmp
                    );
                    int         maxDim      = 0;
                    int         maxDimCount = 0;
                    char        *maxSim;
                    for (
                            int i           = 0;
                            i < P->
                                    simplexCount;
                            ++i) {
                        Simplex *simp = getSimpexAt(P, i);
                        if (maxDim < simp->elementCount) {
                            maxDim      = simp->elementCount;
                            maxDimCount = 1;
                            maxSim      = simplexToLiteral(simp);
                        }
                        else {
                            maxSim = malloc(sizeof(char));
                        }
                        char *simpLit = simplexToLiteral(simp);
                        if (maxDim == simp->elementCount && maxSim != NULL &&
                            strcmp(maxSim, simpLit
                            ) != 0) {
                            maxDimCount++;
                        }

                        free(maxSim);
                        free(simpLit);
                    }

                }
                else {
                    Dest_Simplex(tmp);
                }

                Dest_Complex(P);
            }
        }

        char *complexLiteral = complexToLiteral(posetPrep, true);
        wrtieLine(file,
                  "RequirePackage(\"homology\");", false);
        wrtieLine(file,
                  "SimplicialHomology(OrderComplex(OrderRelationToPoset(", true);
        wrtieLine(file, complexLiteral,
                  true);
        wrtieLine(file,
                  ",IsSubset)));", false);

        printf("\nResult is in:\n %s\n", file->path);

        Destroy_file(file);
        Destory_Storage(storage0);
        Dest_Complex(posetPrep);
        Dest_Simplex(fVA);
        Dest_Simplex(fVB);
        free(fVALit);
        free(fVBLit);

    }
}

static inline void DoProgress(char label[], int step, int total) {
    int percent = (step * 100) / total;

    printf("%s[", label);

    //fill progress bar with =
//    for (int i = 0; i < pos; i++) printf("%c", '=');

    //fill progress bar with spaces
    printf(" %d / %d %c", step, total, ']');
    printf(" %3d%%\r", percent);
    fflush(stdout);
}


