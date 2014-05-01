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


Simplex* Init_Simplex(int vertices, bool fill)
{
    if (vertices > ((sizeof(SimplexElem) * 8) - 2)) {
        printf("\nSimplexElem StackOverflow\n");
        exit(0);
    }
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


Complex* Init_Complex(int simplexCount)
{
    Complex* complex       = (Complex*) malloc(sizeof(Complex));
    complex->simplexCount  = simplexCount;
    complex->simplexes     = malloc(simplexCount * sizeof(Simplex));
    return complex;
}

void Dest_Complex(Complex* complex)
{
    for (int i = 0; i < complex->simplexCount; ++i) {
        Dest_Simplex(complex->simplexes[i]);
    }
    free(complex->simplexes);
    free(complex);
}

char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void saveComplex(Complex* comp, int k, int v) {
    char str_k[10];
    sprintf(str_k, "%d", k);
    char str_V[10];
    sprintf(str_V, "%d", v);
    
    char *key = "_";
    key = concat(str_k, key);
    key = concat(key, str_V);
    
    if (comp->simplexCount < k) {
        printf("");
    }
    
//    printf("\n----- Saving Complex %s for %s -----\n", complexToLiteral(comp, true), key);
    
    sm_put(sm, key, complexToLiteral(comp, false));
}

Complex* getComplex(int k, int v) {
    char str_k[10];
    sprintf(str_k, "%d", k);
    char str_V[10];
    sprintf(str_V, "%d", v);
    
    char *key = "_";
    key = concat(str_k, key);
    key = concat(key, str_V);
    
    char buf[1024];
    sm_get(sm, key, buf, sizeof(buf));
    
//    printf("\n----- Getting Complex %s for %s -----\n", buf, key);
    
    return literalToComplex(buf);
}

//Simplex* dublicateSimplex(Simplex* simp) {
//    Simplex* temp = Init_Simplex(simp->verticesCount, false);
//    
//    for (int tempi = 0; tempi < simp->verticesCount; ++tempi) {
//        temp->simplexRel[tempi] = simp->simplexRel[tempi];
//    }
//    
//    return temp;
//}

bool checkSimplexSubSimplex(Simplex* simplex, Simplex* subSimplex) {
    bool result = true;
    for (int i = 0; i < subSimplex->verticesCount; ++i) {
        SimplexElem subSimElem = subSimplex->simplexRel[i];
        
        bool elemMatch = false;
        for (int j = 0; j < simplex->verticesCount; ++j) {
            SimplexElem simElem = simplex->simplexRel[j];
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
    Complex* complex = Init_Complex(0);
    if (K == 1) {
        int prevSubsCount = 0;
        
        for (int i = 0; i < B->simplexCount; i++) {
            Simplex* sim = B->simplexes[i];
            int expV = prevSubsCount + (1 << sim->verticesCount);
            if (V > prevSubsCount && expV > V) {
                V = V - prevSubsCount;
                if (complex->simplexCount == 0) {
                    complex = Init_Complex(K);
                }
                complex->simplexes[0] = simplexByExp(sim, V);
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
    Complex* neibr   = Init_Complex(comp->simplexCount);
    int simplexCount = 0;
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp = comp->simplexes[i];
        
        if (checkSimplexSubSimplex(simp, searchSimp)) {
            if(simplexCount > neibr->simplexCount) {
                neibr->simplexCount  <<= 1;
                realloc(neibr->simplexes, neibr->simplexCount * sizeof(Simplex));
            }
            
            neibr->simplexes[simplexCount] = simp;
            simplexCount++;
        }
    }
    
    neibr->simplexCount = simplexCount;
    realloc(neibr->simplexes, neibr->simplexCount * sizeof(Simplex));
    
    return neibr;
}

Complex* mergeComplexes(Complex* a, Complex* b, bool basic) {
    if (a->simplexCount == 0) {
        return b;
    }
    Complex* merged        = Init_Complex(a->simplexCount + b->simplexCount);
    for (int i = 0; i < a->simplexCount; ++i) {
        merged->simplexes[i] = a->simplexes[i];
    }
    int mergedIndex = a->simplexCount;
    for (int i = 0; i < b->simplexCount; ++i) {
        bool unique = true;
        for (int  j = 0; j < mergedIndex; ++j) {
            if (strcmp(simplexToLiteral(merged->simplexes[j]), simplexToLiteral(b->simplexes[i])) == 0) {
                unique = false;
            }
        }
        if (basic || unique) {
            merged->simplexes[mergedIndex] = b->simplexes[i];
            mergedIndex++;
        }
    }
    
    merged->simplexCount = mergedIndex;
    realloc(merged->simplexes, merged->simplexCount * sizeof(Simplex));

    return merged;
}

Simplex* buildIntersectedSimplex(Complex* comp) {
    Simplex* intersectedSimplex      = Init_Simplex(1, false);
    int      intersectedSimplexIndex = 0;
    
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* simp = comp->simplexes[i];
        for (int j = 0; j < simp->verticesCount; ++j) {
            SimplexElem simpElem = simp->simplexRel[j];
            
            for (int l = 0; l < comp->simplexCount; ++l) {
                if (l == i) {
                    continue;
                }
                Simplex* simp2 = comp->simplexes[l];
                for (int m = 0; m < simp2->verticesCount; ++m) {
                    SimplexElem simpElem2 = simp2->simplexRel[m];
                    if (simpElem == simpElem2) {
                        if (intersectedSimplexIndex >= intersectedSimplex->verticesCount) {
                            intersectedSimplex->verticesCount                   = intersectedSimplexIndex << 1;
                            realloc(intersectedSimplex->simplexRel, intersectedSimplex->verticesCount);
                        }
                        
                        bool unique = true;
                        for (int checkIndex = 0; checkIndex < intersectedSimplexIndex; ++checkIndex) {
                            SimplexElem checkElem = intersectedSimplex->simplexRel[checkIndex];
                            if (checkElem == simpElem) {
                                unique = false;
                            }
                        }
                        
                        if (unique) {
                            intersectedSimplex->simplexRel[intersectedSimplexIndex] = simpElem;
                            intersectedSimplexIndex++;
                        }
                    }
                }
            }
            
        }
    }
    
    intersectedSimplex->verticesCount = intersectedSimplexIndex;
    realloc(intersectedSimplex->simplexRel, intersectedSimplexIndex);
    
    return intersectedSimplex;
}

void printIntArray(int* arr, int arrLength) {
    printf("[");
    for (int i = 0; i < arrLength; ++i) {
        printf("%d", arr[i]);
        if (i != arrLength -1) {
            printf(", ");
        }
    }
    printf("]\n");
}

Complex* unionIntersection(Complex** posibilityList, int posibilityListLength) {
    int*     walkIndexes = malloc(posibilityListLength * sizeof(int));
    memset(walkIndexes, 0, posibilityListLength * sizeof(int));
    
//    printf("\n-- generation start -- \n");
    
    Complex* unionIntersection      = Init_Complex(1);
    int      unionIntersectionIndex = 0;
    bool cont = false;
    do {
        Complex* comp  = Init_Complex(posibilityListLength);
        for (int i = 0; i < posibilityListLength; ++i) {
            comp->simplexes[i] = posibilityList[i]->simplexes[walkIndexes[i]];
        }
        
        
        cont = false;
        for (int i = 0; i < posibilityListLength; ++i) {
            if (walkIndexes[i] < posibilityList[i]->simplexCount - 1) {
                cont = true;
            }
        }
        
//        printf("\nwalkIndexes");
//        printIntArray(walkIndexes, posibilityListLength);
        
        for (int i = posibilityListLength - 1; i >= 0; --i) {
            if (walkIndexes[i] + 1 < posibilityList[i]->simplexCount) {
                walkIndexes[i]++;
                break;
            }
        }
        
//        printf("\nUnionComplex%s\n", complexToLiteral(comp, true));
//        printf("\nIntersectedSimplex%s\n", simplexToLiteral(buildIntersectedSimplex(comp)));
//        printf("");
        
        if (unionIntersectionIndex >= unionIntersection->simplexCount) {
            unionIntersection->simplexCount = unionIntersectionIndex << 1;
            realloc(unionIntersection->simplexes, unionIntersection->simplexCount);
        }
        unionIntersection->simplexes[unionIntersectionIndex]  = buildIntersectedSimplex(comp);
        unionIntersectionIndex++;
    } while (cont);

    unionIntersection->simplexCount = unionIntersectionIndex;
    realloc(unionIntersection->simplexes, unionIntersection->simplexCount);
    

    
//    printf("\nunionIntersection%s\n", complexToLiteral(unionIntersection, true));
    
    return unionIntersection;
}

Complex* intersectionUnionUpper(Complex* B, Complex** posibilityList, int posibilityListLength) {
    Complex* unionIntersection      = Init_Complex(posibilityListLength);
    int      unionIntersectionIndex = 0;
    
    for (int i = 0; i < posibilityListLength; ++i) {
        Complex* comp = posibilityList[i];
        Simplex* simp = Init_Simplex(1, false);
        int  simIndex = 0;
        for (int j = 0; j < comp->simplexCount; ++j) {
            Simplex* tempSim = comp->simplexes[j];
            
            for (int l = 0; l < tempSim->verticesCount; ++l) {
                SimplexElem elem = tempSim->simplexRel[l];
                
                bool unique = true;
                for (int a = 0; a < simIndex; ++a) {
                    SimplexElem checkElem = simp->simplexRel[a];
                    if (elem == checkElem) {
                        unique = false;
                        break;
                    }
                }
                
                if (unique) {
                    if (simIndex + 1 > simp->verticesCount) {
                        simp->verticesCount = simp->verticesCount << 1;
                        realloc(simp->simplexRel, simp->verticesCount * sizeof(SimplexElem));
                        
                    }
                    simp->simplexRel[simIndex]  = elem;
                    simIndex++;
                }
            }
            
        }
        
        simp->verticesCount = simIndex;
        realloc(simp->simplexRel, simp->verticesCount * sizeof(SimplexElem));
        unionIntersection->simplexes[unionIntersectionIndex] = simp;
        unionIntersectionIndex++;
    }
    
    return upperSimplexContainingDot(B, buildIntersectedSimplex(unionIntersection));
}

int CalculatePoints(Complex* comp) {
    int points = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* sim = comp->simplexes[i];
        points += sim->verticesCount;
    }
    Simplex* tempSim = Init_Simplex(points, 0);
    points = 0;
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex* sim = comp->simplexes[i];
        if (sim == NULL) {
            continue;
        }
        for (int j = 0; j < sim->verticesCount; ++j) {
            SimplexElem elem1 = sim->simplexRel[j];
            
            int insertIndex = 0;
            bool checkUnique = true;
            for (int k = 0; k < tempSim->verticesCount; ++k) {
                SimplexElem elem2 = tempSim->simplexRel[k];
                if (elem2 == 0) {
                    insertIndex = k;
                    break;
                } else if(elem2 == elem1) {
                    checkUnique = false;
                    break;
                }
            }
            if (checkUnique) {
                tempSim->simplexRel[insertIndex] = elem1;
                points++;
            }
        }
    }
    
    //    printf("\n%s\n", simplexToLiteral(tempSim));
    
    return points;
}


int Hom_Match(Complex* A, Complex* B, Complex* P, int k, int V) {
    Simplex* temp   = Init_Simplex(1, false);
    temp->simplexRel[0] = (SimplexElem) k;
    

    Complex* ANeibr = upperSimplexContainingDot(A, temp);
    
    Complex** posibilityList       = malloc(P->simplexCount * sizeof(Complex));
    int       posibilityListLength = 0;
    
    Complex* BNeibr = Init_Complex(0);
    
    for (int i = 0; i < ANeibr->simplexCount; ++i) {
        Simplex* aNeibrSim  = ANeibr->simplexes[i];
        Complex* BNeibrTemp = Init_Complex(0);
        
        for (int j = 0; j < aNeibrSim->verticesCount; ++j) {
            SimplexElem elem = aNeibrSim->simplexRel[j];
            int    elemIndex = elem - 1;
            if (elem != (SimplexElem) k && elemIndex < P->simplexCount) {
                Simplex* tempSim = P->simplexes[elemIndex];
                Complex* comp    = upperSimplexContainingDot(B, tempSim);
                BNeibrTemp       = mergeComplexes(BNeibrTemp, comp, false);
                
                posibilityList[posibilityListLength]    = BNeibrTemp;
                posibilityListLength++;
            }
        }
        
        
    }
    
//    for (int j = 0; j < ANeibrList; ++j) {
//        Simplex* aNeibrSim   = ANeibr->simplexes[j]
//        Simplex* prevSimplex = P->simplexes[j];
//        Complex* comp        = upperSimplexContainingDot(B, prevSimplex);
//        BNeibr               = mergeComplexes(BNeibr, comp);
//        
//        posibilityList[j]    = BNeibr;
//        posibilityListLength++;
//    }
    
//    for (int i = 0; i < posibilityListLength; ++i) {
//      printf("posibilityList[%d] = %s\n", i, complexToLiteral(posibilityList[i], true));
//    }
    
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
    
//    printf("P: %s BNeib:%s K:%d\n", complexToLiteral(P, true), complexToLiteral(BNeibr, true), k);
    
    for (int j = 0; j < BNeibr->simplexCount; ++j) {
        Simplex* simp = BNeibr->simplexes[j];
        
        if (V == 23) {
            printf("");
        }
    
        
        for (int l = 0; l < simp->verticesCount; ++l) {
            Complex* temp      = Init_Complex(1);
            Simplex* temp_Simp = Init_Simplex(1, false);
            
            temp_Simp->simplexRel[0] = simp->simplexRel[l];
            temp->simplexes[0]       = temp_Simp;
            saveComplex(mergeComplexes(P, temp, true), k, V);
//            Dest_Complex(temp);
            
            V++;
        }
        
        
        
        Complex* temp = Init_Complex(1);
        temp->simplexes[0] = simp;
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
        Complex* P = Init_Complex(0);
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
    }
    
    printf("\n\n Result \n\n");
    
    for (int k = 2; k <= points; ++k) {
        int V1 = 1;
        Complex* P = NULL;
        do {
            P = FSI(A, B, k, V1);
            if (P != NULL && P->simplexCount > 0) {
                printf("\nFSI(A, B, %d, %d) = %s\n", k, V1, complexToLiteral(P, true));
                V1++;
            }
        } while (P != NULL && P->simplexCount > 0);
    }
}


Simplex* simplexByExp(Simplex* simplex, int exp) {
    int tmpA = exp, numberOfElemes = 0, j = 0;
    Simplex* simpl = Init_Simplex(simplex->verticesCount, false);
    for (j = 0; j < (8 * sizeof(int)); ++j) {
        if (tmpA & 1 && j < simplex->verticesCount) {
            simpl->simplexRel[numberOfElemes] = simplex->simplexRel[j];
            numberOfElemes++;
        }
        tmpA >>= 1;
    }
    realloc(simpl->simplexRel, numberOfElemes * sizeof(SimplexElem));
    simpl->verticesCount  = numberOfElemes;
    return simpl;
}

Complex* AllSubSimplexses(Simplex* simplex)
{
    int i, a = 1 << simplex->verticesCount;
    Complex*  complex     = Init_Complex(a);
    for (i = 0; i < a; ++i) {
        Simplex* simpl = simplexByExp(simplex, i);
        complex->simplexes[i] = simpl;
    }

    return complex;
}

Complex* literalToComplex(char* complexLiteral)
{
    Complex*  complex;
    
    int magicNumberSimplex    = 4;
    int magicNumberComplex    = 4;
    int magicNumberInput      = 4;
    int bracketsCount         = 0;
    int simplexi              = 0;
    int simplexElemi          = 0;
    
    char* posibleSimplexElem  = (char*) malloc(magicNumberInput * sizeof(char));
    int   posibleSimplexElemi = 0;
    
    for (int i = 0; i < strlen(complexLiteral); ++i) {
        char  aChar = complexLiteral[i];
        if (aChar == '[') {
            if (bracketsCount == 0) {
                complex  = Init_Complex(magicNumberComplex);
            } else if (bracketsCount == 1) {
                complex->simplexes[simplexi] = Init_Simplex(magicNumberSimplex, false);
                if (simplexi > magicNumberComplex) {
                    magicNumberComplex  <<= 1;
                    complex->simplexCount = magicNumberComplex;
                    realloc(complex->simplexes, complex->simplexCount * sizeof(complex));
                }
            }
            bracketsCount++;
        } else if (bracketsCount == 2 && aChar != ',' && aChar != ']' && aChar != '\0' && aChar != ' ') {
            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;
            if (posibleSimplexElemi > magicNumberInput) {
                magicNumberInput   <<= 1;
                realloc(posibleSimplexElem, magicNumberInput * sizeof(char));
            }
        } else if (bracketsCount == 2 && aChar == ',') {
            Simplex* simplex                  = complex->simplexes[simplexi];
            realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char));
            simplex->simplexRel[simplexElemi] = atoi(posibleSimplexElem);
            posibleSimplexElemi               = 0;
            simplexElemi++;
            if (simplexElemi > magicNumberSimplex) {
                magicNumberSimplex   <<= 1;
                simplex->verticesCount = magicNumberSimplex;
                realloc(simplex->simplexRel, simplex->verticesCount * sizeof(SimplexElem));
            }
        } else if (aChar == ']') {
            if (bracketsCount == 2) {
                Simplex* simplex       = complex->simplexes[simplexi];
                
                realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char)); // grabing last element
                simplex->simplexRel[simplexElemi] = atoi(posibleSimplexElem);
                simplexElemi++;
                
                simplex->verticesCount = simplexElemi;
                realloc(simplex->simplexRel, simplex->verticesCount * sizeof(SimplexElem));
                simplexi++;
                simplexElemi        = 0;
                posibleSimplexElemi = 0;
            } else if (bracketsCount == 1) {
                complex->simplexCount = simplexi;
                realloc(complex->simplexes, complex->simplexCount * sizeof(Simplex));
                break;
            }
            bracketsCount--;
        }
        
    }
    
    return complex;
}

char* simplexToLiteral(Simplex* simplex) {
    if (simplex == NULL) {
        return "";
    }
    
    char* literal  = malloc(255 * sizeof(char));
    int   literali = 0;
    
    literal[literali] = '[';
    literali++;
    
    for (int j = 0; j < simplex->verticesCount; ++j) {
        SimplexElem elem = simplex->simplexRel[j];
        char str[10];
        sprintf(str, "%d", elem);
        
        for (int s = 0; s < strlen(str); ++s) {
            literal[literali] = str[s];
            literali++;
        }
        
        if (j != simplex->verticesCount - 1) {
            literal[literali] = ',';
            literali++;
            if (true) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }
    
    literal[literali] = ']';
    literali++;
    
    return literal;
}

char* complexToLiteral(Complex* complex, bool pretty)
{
    if (complex == NULL) {
        return "[[]]";
    }
    char* literal  = malloc(255 * sizeof(char));
    int   literali = 0;
    literal[literali] = '[';
    literali++;
    
    for (int i = 0; i < complex->simplexCount; ++i) {
        Simplex* simplex = complex->simplexes[i];
        literal[literali] = '[';
        literali++;
        
        for (int j = 0; j < simplex->verticesCount; ++j) {
            SimplexElem elem = simplex->simplexRel[j];
            char str[10];
            sprintf(str, "%d", elem);
            
            for (int s = 0; s < strlen(str); ++s) {
                literal[literali] = str[s];
                literali++;
            }
            
            if (j != simplex->verticesCount - 1) {
                literal[literali] = ',';
                literali++;
                if (pretty) {
                    literal[literali] = ' ';
                    literali++;
                }
            }
        }
        
        literal[literali] = ']';
        literali++;
        
        if (i != complex->simplexCount - 1) {
            literal[literali] = ',';
            literali++;
            if (pretty) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }
    
    literal[literali] = ']';
    literali++;
    return literal;
}

void swapElements(SimplexElem** find, SimplexElem** replace, int findReplaceLen, SimplexElem** array, int arrayLen)
{
    for (int i = 0; i < arrayLen; ++i) {
        SimplexElem* elem = array[i];
        for (int j = 0; j < findReplaceLen; ++j) {
            SimplexElem* elem2 = find[j];
            if (elem == elem2) {
                array[i] = replace[j];
            }
        }
    }
}














