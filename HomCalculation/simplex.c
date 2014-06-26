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

StrMap *sm;

void saveComplex(Complex* comp, int k, int v) {
	char str_k[100];
	sprintf(str_k, "%d", k);
	char str_V[100];
	sprintf(str_V, "%d", v);
    
	char* key0 = "_";
	char* key1 = concat(str_k, key0);
	char* key = concat(key1, str_V);
    
	free(key1);
    
	//printf("\n----- Saving Complex %s for %s -----\n", complexToLiteral(comp, true), key);
    
	char* tmp = complexToLiteral(comp, false);
    
	if (usefilesystem) {
		char* path = concat("./", key);
		char* pathf = concat(path, ".txt");
		LD_File* file = Init_file_util(pathf, true);
		wrtieLine(file, tmp, false);
		free(path);
		free(pathf);
	} else {
		sm_put(sm, key, tmp);
	}
    
	free(tmp);
	free(key);
}

Complex* getComplex(int k, int v) {
	char str_k[100];
	sprintf(str_k, "%d", k);
	char str_V[100];
	sprintf(str_V, "%d", v);
    
	char* key0 = "_";
	char* key1 = concat(str_k, key0);
	char* key = concat(key1, str_V);
    
	free(key1);
    
	if (usefilesystem) {
		char* path = concat("./", key);
		char* pathf = concat(path, ".txt");
		LD_File* file = Init_file_util(pathf, false);
		char* buf = readFile(file);
		Complex* comp = literalToComplex(buf);
		free(path);
		free(pathf);
		free(buf);
        
		return comp;
	} else {
        
		char buf[4096];
		sm_get(sm, key, buf, sizeof(buf));
		//    printf("\n----- Getting Complex %s for %s -----\n", buf, key);
		free(key);
		return literalToComplex(buf);
	}
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
		complex = getComplex(K, V);
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
			char* aLit = simplexToLiteral(merged->simplexes[j]);
			char* bLit = simplexToLiteral(b->simplexes[i]);
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
	int*     walkIndexes = malloc(posibilityListLength * sizeof(int));
	memset(walkIndexes, 0, posibilityListLength * sizeof(int));
    
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


void Hom_Match(Complex* A, Complex* B, Complex* P, int k, int *V) {
	Simplex* temp = Init_Simplex();
	addElement(temp, k);
    
	Complex* ANeibr = upperSimplexContainingDot(A, temp);
	Dest_Simplex(temp);
    
	Complex** posibilityList = malloc(P->simplexCount * sizeof(Complex*));
	int       posibilityListLength = 0;
    
    
	Complex* BNeibr = NULL;
    
	for (int i = 0; i < ANeibr->simplexCount; ++i) {
		Simplex* aNeibrSim = getSimpexAt(ANeibr, i);
		Complex* BNeibrTemp = NULL;
        
		for (int j = 0; j < aNeibrSim->elementCount; ++j) {
			SimplexElem elem = getElementAt(aNeibrSim, j);
			int    elemIndex = elem - 1;
            
			if (elem != (SimplexElem)k && elemIndex < P->simplexCount) {
				Simplex* tempSim = getSimpexAt(P, elemIndex);
				Complex* comp = upperSimplexContainingDot(B, tempSim);
				BNeibrTemp = mergeComplexes(BNeibrTemp, comp, false);
                
				posibilityList[posibilityListLength] = BNeibrTemp;
				posibilityListLength++;
                
				//free(comp);
			}
		}
	}
    
	if (posibilityListLength > 1) {
		if (method1) {
			BNeibr = unionIntersection(posibilityList, posibilityListLength);
		}
		else if (method2) {
			BNeibr = intersectionUnionUpper(B, posibilityList, posibilityListLength);
		}
		else if (methodComp) {
			Complex* BNeibr1 = unionIntersection(posibilityList, posibilityListLength);
			Complex* BNeibr2 = intersectionUnionUpper(B, posibilityList, posibilityListLength);
			printf("\n %s \n %s \n", complexToLiteral(BNeibr1, true), complexToLiteral(BNeibr2, true));
			char* bn1Lit = complexToLiteral(BNeibr1, false);
			char* bn2Lit = complexToLiteral(BNeibr2, false);
			if (strcasecmp(bn1Lit, bn2Lit) != 0) {
				printf("vai");
			}
			else {
				printf("\nYEA\n");
			}
            
			//            free(bn1Lit);
			//            free(bn2Lit);
			//            free(BNeibr1);
			//            free(BNeibr2);
		}
	}
	else if (posibilityListLength > 0){
		BNeibr = posibilityList[0];
	}
    
	if (BNeibr != NULL) {
		for (int j = 0; j < BNeibr->simplexCount; ++j) {
			Simplex* simp = getSimpexAt(BNeibr, j);
            
			for (int l = 0; l < simp->elementCount; ++l) {
				Complex* temp1 = Init_Complex();
                
				addSimplex(temp1, Init_Simplex());
                
				addElement(getSimpexAt(temp1, 0), getElementAt(simp, l));
                
				Complex* M1Complex = mergeComplexes(P, temp1, true);
				saveComplex(M1Complex, k, *V);
				(*V)++;
				Light_Dest_Complex(M1Complex);
				Dest_Complex(temp1);
                
			}
            
            
            
			Complex* temp2 = Init_Complex();
			addSimplex(temp2, simp);
			Complex* MComplex = mergeComplexes(P, temp2, true);
			saveComplex(MComplex, k, *V);
			(*V)++;
			Light_Dest_Complex(MComplex);
			Light_Dest_Complex(temp2);
            
            
		}
		if (posibilityListLength > 1) {
			for (int i = 0; i < posibilityListLength; ++i) {
				Light_Dest_Complex(posibilityList[i]);
			}
		}
        
		Light_Dest_Complex(ANeibr);
		Light_Dest_Complex(BNeibr);
		free(posibilityList);
	}
    
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
	int points = CalculatePoints(A);
    
	Simplex* fVA = fVectorFromComplex(A);
	Simplex* fVB = fVectorFromComplex(B);
    
	char* fVALit = simplexToLiteral(fVA);
	char* fVBLit = simplexToLiteral(fVB);
    
	printf("\nAF: %s\n", fVALit);
	printf("\nBF: %s\n", fVBLit);
    
	int k1 = 0;
	for (int i = 0; i < A->simplexCount; ++i) {
		Simplex* simp = getSimpexAt(A, i);
		Complex* simpSubs = AllSubSimplexses(simp);
		k1 += simpSubs->simplexCount;
		Dest_Complex(simpSubs);
	}
    
	sm = sm_new(points);
	int V1 = 1, last_V1 = k1, V = 1;
	for (int k = 2; k <= points; ++k) {
		for (V1 = 1; V1 <= last_V1; ++V1) {
			//printf("\ntn:%d\n", omp_get_num_threads());
			Complex* P = FSI(A, B, k - 1, V1);
			if (P != NULL && P->simplexCount > 0) {
				//                printf("\nP -> %s,K = %d, V1 = %d\n", complexToLiteral(P, true), k, V1);
                
                Hom_Match(A, B, P, k, &V);
                
				Dest_Complex(P);
			}
			else {
                saveComplex(P, k, V);
			}
		}
        
		last_V1 = V;
		printf("\n%d -> %d\n", k, V1 - 1);
	}
    
	printf("\n\n Doing Safe House \n\n");
	fflush(stdout);
    
	V1 = 0;
	LD_File* file0 = Init_file_util_ext("./hom_safe_house", "txt", false);
	Complex* P = NULL;
	do {
		P = FSI(A, B, points, V1);
		if (P != NULL && P->simplexCount > 0) {
			wrtieLine(file0, complexToLiteral(P, true), false);
			V1++;
		}
	} while (P != NULL && P->simplexCount > 0);
    
	printf("\n\n Generation Result File \n\n");
	fflush(stdout);
    
	LD_File* file = Init_file_util_ext("./hom_result", "txt", false);
    
	int bPoints = CalculatePoints(B);
	V1 = 1;
	int count = 0;
	P = NULL;
	Complex* posetPrep = Init_Complex();
	Simplex* fVector = Init_Simplex();
	for (int i = 0; i < points * 4; ++i) {
		addElement(fVector, 0);
	}
	do {
		P = FSI(A, B, points, V1);
		if (P != NULL && P->simplexCount > 0) {
			Simplex* tmp = Init_Simplex();
			count++;
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
				char* maxSim = "";
				for (int i = 0; i < P->simplexCount; ++i) {
					Simplex* simp = getSimpexAt(P, i);
					if (maxDim < simp->elementCount) {
						maxDim = simp->elementCount;
						maxDimCount = 1;
						maxSim = simplexToLiteral(simp);
					}
					char* simpLit = simplexToLiteral(simp);
					if (maxDim == simp->elementCount && strcmp(maxSim, simpLit) != 0) {
						maxDimCount++;
					}
					free(simpLit);
				}
				//                addElement(fVector, maxDimCount);
				fVector->elements[maxDim - 1] += maxDimCount;
			}
			V1++;
		}
	} while (P != NULL && P->simplexCount > 0);
    
	printf("\nFVector: %s\n", simplexToLiteral(fVector));
    
	char* complexLiteral = complexToLiteral(posetPrep, true);
	wrtieLine(file, "RequirePackage(\"homology\");", false);
	wrtieLine(file, "SimplicialHomology(OrderComplex(OrderRelationToPoset(", true);
	wrtieLine(file, complexLiteral, true);
	wrtieLine(file, ",IsSubset)));", false);
    
	printf("\nResult is in:\n %s\n", file->path);
    
	if (false) {
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




