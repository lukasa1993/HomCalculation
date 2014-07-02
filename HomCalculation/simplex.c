//
//  simplex.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include "simplex.h"
#include "complex_storage.h"

#include <omp.h>

#define method1    true // unionIntersection
#define method2    false  // intersectionUnionUpper
#define methodComp false // both

#define usefilesystem false

Complex_Storage* storage0;
Complex_Storage* storage1;

void saveComplex(Complex* comp) {
	char* literal = complexToLiteral(comp, true);
    addLiteral(storage1, literal);
}

Complex* getComplex(long long v) {
    char* literal = getLiteralAt(storage0, v);
    Complex* comp = literalToComplex(literal);
    return comp;
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

Complex* FSI(Complex* A, Complex* B, int K, long long V) {
	Complex* complex = NULL;
	if (K == 1) {
		int prevSubsCount = 0;
        
		for (int i = 0; i < B->simplexCount; i++) {
			Simplex* sim = getSimpexAt(B, i);
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

Complex* upperSimplexContainingDot(Complex* comp, Simplex* searchSimp) {
	Complex* neibr = Init_Complex();
    
	for (int i = 0; i < comp->simplexCount; ++i) {
		Simplex* simp = comp->simplexes[i];
        
		if (checkSimplexSubSimplex(simp, searchSimp)) {
			addSimplex(neibr, simp);
		}
	}
    
	return neibr;
}

Complex* mergeComplexes(Complex* a, Complex* b, bool basic) {
	if (a == NULL || a->simplexCount == 0) {
		return b;
	}
	Complex* merged = Init_Complex();
	if (a != NULL) {
		for (int i = 0; i < a->simplexCount; ++i) {
			addSimplex(merged, getSimpexAt(a, i));
		}
	}
    
	for (int i = 0; i < b->simplexCount; ++i) {
		bool unique = true;
		for (int j = 0; j < merged->simplexCount; ++j) {
			sds aLit = simplexToLiteral(merged->simplexes[j]);
			sds bLit = simplexToLiteral(b->simplexes[i]);
			if (strcmp(aLit, bLit) == 0) {
				unique = false;
			}
			sdsfree(aLit);
            sdsfree(bLit);
		}
        
		if (basic || unique) {
			addSimplex(merged, getSimpexAt(b, i));
		}
	}
    
	return merged;
}

Simplex* buildIntersectedSimplex(Complex* comp) {
	Simplex* intersectedSimplex = Init_Simplex();
    
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
    if (posibilityListLength == 1) {
        return posibilityList[0];
    }
	int*     walkIndexes = calloc(posibilityListLength, sizeof(int));//malloc(posibilityListLength * sizeof(int));
    
	//    printf("\n-- generation start -- \n");
    
	Complex* unionIntersection = Init_Complex();
    
	bool cont = false;
	do {
		Complex* comp = Init_Complex();
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
        
		Simplex* intersectedSimplex = buildIntersectedSimplex(comp);
        
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


void Hom_Match(Complex* A, Complex* B, Complex* P, int k) {
	Simplex* temp = Init_Simplex();
	addElement(temp, k);
    
	Complex* ANeibr = upperSimplexContainingDot(A, temp);
	Dest_Simplex(temp);
    
	Complex** posibilityList       = calloc(A->simplexCount + B->simplexCount + P->simplexCount, sizeof(Complex*));
	int       posibilityListLength = 0;
    
	for (int i = 0; i < ANeibr->simplexCount; ++i) {
		Simplex* aNeibrSim   = getSimpexAt(ANeibr, i);
        
		for (int j = 0; j < aNeibrSim->elementCount; ++j) {
			SimplexElem elem = getElementAt(aNeibrSim, j);
			int    elemIndex = elem - 1;
            
			if (elem != (SimplexElem)k && elemIndex < P->simplexCount) {
				Simplex* tempSim = getSimpexAt(P, elemIndex);
				Complex* comp    = upperSimplexContainingDot(B, tempSim);
                
                if (posibilityListLength > 0) {
                    posibilityList[posibilityListLength] = mergeComplexes(posibilityList[posibilityListLength - 1], comp, false);
                    posibilityListLength++;
                    Light_Dest_Complex(comp);
                } else {
                    posibilityList[posibilityListLength] = comp;
                    posibilityListLength++;
                    
                }
			}
		}
	}
    
    Complex* BNeibr = unionIntersection(posibilityList, posibilityListLength);
    
    if (BNeibr == NULL) {
        printf("");
    }
    
	
    for (int j = 0; j < BNeibr->simplexCount; ++j) {
        Simplex* simp = getSimpexAt(BNeibr, j);
        
        for (int l = 0; l < simp->elementCount; ++l) {
            Complex* temp1 = Init_Complex();
            
            addSimplex(temp1, Init_Simplex());
            
            addElement(getSimpexAt(temp1, 0), getElementAt(simp, l));
            
            Complex* M1Complex = mergeComplexes(P, temp1, true);
#pragma omp critical
            {
                saveComplex(M1Complex);
            }
            Light_Dest_Complex(M1Complex);
            Dest_Complex(temp1);
            
        }
        
        
        
        Complex* temp2 = Init_Complex();
        addSimplex(temp2, simp);
        Complex* MComplex = mergeComplexes(P, temp2, true);
#pragma omp critical
        {
            saveComplex(MComplex);
        }
        Light_Dest_Complex(MComplex);
        Light_Dest_Complex(temp2);
        
        
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

Simplex* fVectorFromComplex(Complex* comp)
{
    
	Complex* fComplex = Init_Complex();
	Simplex* AfVector = Init_Simplex();
    
	for (int i = 0; i < comp->simplexCount; ++i) {
		Simplex* simp = getSimpexAt(comp, i);
		Complex* simpSubs = AllSubSimplexses(simp);
		//        printf("\n%s\n", complexToLiteral(simpSubs, true));
		for (int j = 0; j < simpSubs->simplexCount; ++j) {
			Simplex* pSimp = getSimpexAt(simpSubs, j);
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
		Simplex* simp = getSimpexAt(fComplex, i);
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

void Calculate_Hom(Complex* A, Complex* B) {
    storage0 = Init_Storage();
    storage1 = Init_Storage();
    
	int points = CalculatePoints(A);
    
	Simplex* fVA = fVectorFromComplex(A);
	Simplex* fVB = fVectorFromComplex(B);
    
	char* fVALit = simplexToLiteral(fVA);
	char* fVBLit = simplexToLiteral(fVB);
    
	printf("\nAF: %s\n", fVALit);
	printf("\nBF: %s\n", fVBLit);
    
	long long k1 = 0;
	for (int i = 0; i < A->simplexCount; ++i) {
		Simplex* simp     = getSimpexAt(A, i);
		Complex* simpSubs = AllSubSimplexses(simp);
		k1               += simpSubs->simplexCount;
        
		Dest_Complex(simpSubs);
	}
    
    storage0->lietralCount = k1;
	for (int k = 2; k <= points; ++k) {
#pragma omp parallel for shared(A, B, storage0, storage1, k)
		for (long long V1 = 1; V1 < storage0->lietralCount; ++V1) {
            Complex* P = NULL;
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
        
        printf("\n%d => %lld\n", k, storage0->lietralCount);
        fflush(stdout);
        
		if (k == 2) {
            storage0->lietralCount = 0;
        }
        Destory_Storage(storage0);
        storage0 = storage1;
        storage1 = Init_Storage();
    }
    Destory_Storage(storage1);
    
	printf("\n\n Generation Result File \n\n");
	fflush(stdout);
    
	LD_File* file = Init_file_util_ext("./hom_result", "txt", false);
    
    
	int bPoints        = CalculatePoints(B);
	Complex* posetPrep = Init_Complex();
	Simplex* fVector   = Init_Simplex();
    
	for (int i = 0; i < points * 4; ++i) {
		addElement(fVector, 0);
	}
    
	for (long long V1 = 1; V1 < storage0->lietralCount; ++V1) {
		Complex* P = FSI(A, B, points, V1);
		if (P != NULL && P->simplexCount > 0) {
			Simplex* tmp = Init_Simplex();
			
            for (int i = 0; i < P->simplexCount; ++i) {
				Simplex* simp = getSimpexAt(P, i);
				for (int j = 0; j < simp->elementCount; ++j) {
					addElement(tmp, ((i * bPoints) + getElementAt(simp, j)));
				}
			}
            
			if (!containsSimplex(posetPrep, tmp)) {
				addSimplex(posetPrep, tmp);
				int maxDim = 0;
				int maxDimCount = 0;
				sds maxSim;
				for (int i = 0; i < P->simplexCount; ++i) {
					Simplex* simp = getSimpexAt(P, i);
					if (maxDim < simp->elementCount) {
						maxDim = simp->elementCount;
						maxDimCount = 1;
						maxSim = simplexToLiteral(simp);
					} else {
                        maxSim = sdsempty();
                    }
					sds simpLit = simplexToLiteral(simp);
					if (maxDim == simp->elementCount && maxSim != NULL && strcmp(maxSim, simpLit) != 0) {
						maxDimCount++;
					}

                    sdsfree(maxSim);
					sdsfree(simpLit);
				}

				fVector->elements[maxDim - 1] += maxDimCount;
			} else {
                Dest_Simplex(tmp);
            }
            
            Dest_Complex(P);
		}
	}
	printf("\nFVector: %s\n", simplexToLiteral(fVector));
    
	sds complexLiteral = complexToLiteral(posetPrep, true);
	wrtieLine(file, "RequirePackage(\"homology\");", false);
	wrtieLine(file, "SimplicialHomology(OrderComplex(OrderRelationToPoset(", true);
	wrtieLine(file, complexLiteral, true);
	wrtieLine(file, ",IsSubset)));", false);
    
	printf("\nResult is in:\n %s\n", file->path);
    
    sdsfree(complexLiteral);
	Destroy_file(file);
    Destory_Storage(storage0);
    Dest_Complex(posetPrep);
    Dest_Simplex(fVector);
    Dest_Simplex(fVA);
    Dest_Simplex(fVB);
    sdsfree(fVALit);
    sdsfree(fVBLit);
}




