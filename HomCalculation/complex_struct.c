//  complex_struct.c
//  HomCalculation
//
//  Created by Luka Dodelia on 5/2/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "complex_struct.h"

Coordinates *Init_Coordinates() {
    Coordinates *coords = (Coordinates *) malloc(sizeof(Coordinates));
    coords->cooordinateIndex = -1;
    coords->cooordinateCount = 0;
    coords->cooordinateCapacity = 1;
    coords->coordinates = malloc(coords->cooordinateCapacity * sizeof(double));
    return coords;
}

void Dest_Coordinates(Coordinates *coords) {
    free(coords->coordinates);
    free(coords);
}

void addCoordinate(Coordinates *coord, Coord x) {
    if (coord->cooordinateCapacity <= coord->cooordinateIndex + 1) {
        coord->cooordinateCapacity <<= 1;
        coord->coordinates = realloc(coord->coordinates, coord->cooordinateCapacity * sizeof(Coord));
    }

    coord->cooordinateCount++;
    coord->coordinates[coord->cooordinateIndex + 1] = x;
    coord->cooordinateIndex = coord->cooordinateCount - 1;
}

Coord getCoordtAt(Coordinates *coords, int index) {
    if (coords->cooordinateIndex < index) {
        return -1;
    } else {
        return coords->coordinates[index];
    }
}

Simplex *Init_Simplex() {
    Simplex *simplex = (Simplex *) malloc(sizeof(Simplex));
    simplex->elementIndex = -1;
    simplex->elementCount = 0;
    simplex->elementCapacity = 1;
    simplex->elements = malloc(simplex->elementCapacity * sizeof(SimplexElem));

    return simplex;
}

void Dest_Simplex(Simplex *simplex) {
    free(simplex->elements);
    free(simplex);
}


Complex *Init_Complex() {
    Complex *complex = (Complex *) malloc(sizeof(Complex));
    complex->simplexIndex = -1;
    complex->simplexCount = 0;
    complex->simplexCapacity = 1;
    complex->simplexes = malloc(complex->simplexCapacity * sizeof(Simplex *));

    return complex;
}

void Light_Dest_Complex(Complex *complex) {
    free(complex->simplexes);
    free(complex);
}

void Dest_Complex(Complex *complex) {
    for (int i = 0; i < complex->simplexCount; ++i) {
        Dest_Simplex(complex->simplexes[i]);
    }
    free(complex->simplexes);
    free(complex);
}

void addSimplex(Complex *comp, Simplex *simp) {
    if (comp->simplexCapacity <= comp->simplexIndex + 1) {
        comp->simplexCapacity <<= 1;
        comp->simplexes = realloc(comp->simplexes, comp->simplexCapacity * sizeof(Simplex *));
    }

    comp->simplexCount++;
    comp->simplexes[comp->simplexIndex + 1] = simp;
    comp->simplexIndex = comp->simplexCount - 1;
}

Simplex* compySimplex(Simplex* a) {
    Simplex* b = Init_Simplex();
    for (int i = 0; i < a->elementCount; ++i) {
        addElement(b, getElementAt(a, i));
    }
    
    return b;
}

void addElement(Simplex *simp, SimplexElem elem) {
    if (simp->elementCapacity <= simp->elementIndex + 1) {
        simp->elementCapacity <<= 1;
        simp->elements = realloc(simp->elements, simp->elementCapacity * sizeof(SimplexElem));
    }

    simp->elementCount++;
    simp->elements[simp->elementIndex + 1] = elem;
    simp->elementIndex = simp->elementCount - 1;
}

Simplex *getSimpexAt(Complex *comp, int index) {
    if (comp->simplexIndex < index) {
        return NULL;
    } else {
        return comp->simplexes[index];
    }
}

SimplexElem getElementAt(Simplex *simp, int index) {
    if (simp->elementIndex < index) {
        return -1;
    } else {
        return simp->elements[index];
    }
}


bool containsElement(Simplex *simp, SimplexElem elem) {
    for (int i = 0; i < simp->elementCount; ++i) {
        SimplexElem elem2 = getElementAt(simp, i);
        if (elem == elem2) {
            return true;
        }
    }

    return false;
}

bool containsSubSimplex(Simplex *simp, Simplex *sub) {
    for (int i = 0; i < sub->elementCount; ++i) {
        SimplexElem elem = getElementAt(sub, i);
        if (!containsElement(simp, elem)) {
            return false;
        }
    }

    return true;
}

bool containsSimplex(Complex *comp, Simplex *simp) {
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp2 = getSimpexAt(comp, i);
        bool containsSimplex = true;

        for (int s1 = 0; s1 < simp->elementCount; ++s1) {
            SimplexElem elem1 = getElementAt(simp, s1);
            bool containsElem = containsElement(simp2, elem1);

            if (containsElem == false) {
                containsSimplex = false;
                break;
            }
        }

        if (containsSimplex == true) {
            return true;
        }
    }

    return false;
}

Simplex *getFacet(Complex *comp, Simplex *simp) {
    for (int i = 0; i < comp->simplexCount; ++i) {
        Simplex *simp2 = getSimpexAt(comp, i);
        bool containsSimplex = true;

        for (int s1 = 0; s1 < simp->elementCount; ++s1) {
            SimplexElem elem1 = getElementAt(simp, s1);
            bool containsElem = containsElement(simp2, elem1);

            if (containsElem == false) {
                containsSimplex = false;
                break;
            }
        }

        if (containsSimplex == true) {
            return simp2;
        }
    }

    return NULL;
}

char *simplexToLiteral(Simplex *simplex) {
    char literal[simplex->elementCount * 5];
    int literali = 0;

    literal[literali] = startingChar;
    literali++;

    for (int j = 0; j < simplex->elementCount; ++j) {
        SimplexElem elem = simplex->elements[j];
        char str[100];
        sprintf(str, "%d", elem);

        for (int s = 0; s < strlen(str); ++s) {
            literal[literali] = str[s];
            literali++;
        }

        if (j != simplex->elementCount - 1) {
            literal[literali] = ',';
            literali++;
            if (true) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }

    literal[literali] = endingChar;
    literali++;

    char *final = malloc((literali + 1) * sizeof(char));
    memcpy(final, literal, (size_t) literali);
    final[literali] = 0;

    return final;
}

char *complexToLiteral(Complex *complex, bool pretty) {
    int charCount = 2;
    if (complex != NULL) {
        for (int i = 0; i < complex->simplexCount; ++i) {
            charCount += getSimpexAt(complex, i)->elementCount + 1;
        }
        charCount *= 5;
    }
    char literal[charCount];
    int literali = 0;

    literal[literali] = startingChar;
    literali++;

    if (complex != NULL) {
        for (int i = 0; i < complex->simplexCount; ++i) {
            Simplex *simplex = getSimpexAt(complex, i);
            literal[literali] = startingChar;
            literali++;

            for (int j = 0; j < simplex->elementCount; ++j) {
                SimplexElem elem = getElementAt(simplex, j);
                char str[100];
                sprintf(str, "%d", elem);

                for (int s = 0; s < strlen(str); ++s) {
                    literal[literali] = str[s];
                    literali++;
                }

                if (j != simplex->elementIndex) {
                    literal[literali] = ',';
                    literali++;
                    if (pretty) {
                        literal[literali] = ' ';
                        literali++;
                    }
                }
            }

            literal[literali] = endingChar;
            literali++;

            if (i != complex->simplexIndex) {
                literal[literali] = ',';
                literali++;
                if (pretty) {
                    literal[literali] = ' ';
                    literali++;
                }
            }
        }
    }
    literal[literali] = endingChar;
    literali++;

    char *final = malloc((literali + 1) * sizeof(char));
    memcpy(final, literal, literali);
    final[literali] = 0;

    return final;
}

Complex *literalToComplex(char *complexLiteral) {
    Complex *complex = NULL;

    char posibleSimplexElem[10];

    int posibleSimplexElemi = 0;
    int bracketsCount = 0;

    for (int i = 0; i < strlen(complexLiteral); ++i) {
        char aChar = complexLiteral[i];

        if (aChar == '[') {
            aChar = startingChar;
        } else if (aChar == ']') {
            aChar = endingChar;
        }

        if (aChar == startingChar) {
            if (bracketsCount == 0) {
                complex = Init_Complex();
            } else if (bracketsCount == 1) {
                addSimplex(complex, Init_Simplex());
            }
            bracketsCount++;
        } else if (bracketsCount == 2 && isdigit(aChar)) {

            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;

        } else if (bracketsCount == 2 && aChar == ',' && posibleSimplexElemi > 0) {

            posibleSimplexElemi = 0;
            addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
            memset(posibleSimplexElem, 0, 10);
        } else if (aChar == endingChar) {
            if (bracketsCount == 2 && posibleSimplexElemi > 0) {
                posibleSimplexElemi = 0;
                addElement(getSimpexAt(complex, complex->simplexIndex), atoi(posibleSimplexElem));
                memset(posibleSimplexElem, 0, 10);
            } else if (bracketsCount == 1) {
                break;
            }
            bracketsCount--;
        }

    }

    return complex;
}


Coordinates *lietralToCoordinates(char *coordinatesLiteral) {
    Coordinates *coordinates = NULL;

    char posibleSimplexElem[100];
    memset(posibleSimplexElem, 0, 100);

    int posibleSimplexElemi = 0;
    int bracketsCount = 0;

    for (int i = 0; i < strlen(coordinatesLiteral); ++i) {
        char aChar = coordinatesLiteral[i];

        if (aChar == startingChar) {
            if (bracketsCount == 0) {
                coordinates = Init_Coordinates();
            }
            bracketsCount++;
        } else if (bracketsCount == 1 && (aChar == '.' || isdigit(aChar))) {

            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;

        } else if (bracketsCount == 1 && aChar == ',' && posibleSimplexElemi > 0) {
            addCoordinate(coordinates, atof(posibleSimplexElem));

            posibleSimplexElemi = 0;
            memset(posibleSimplexElem, 0, 100);

        } else if (aChar == endingChar) {
            if (bracketsCount == 1 && posibleSimplexElemi > 0) {
                posibleSimplexElemi = 0;
                addCoordinate(coordinates, atof(posibleSimplexElem));
            }
            bracketsCount--;
        }
    }

    return coordinates;
}

char *coordinatesToLiteral(Coordinates *coords) {
    char literal[coords->cooordinateCount * 5];
    int literali = 0;

    literal[literali] = startingChar;
    literali++;

    for (int j = 0; j < coords->cooordinateCount; ++j) {
        Coord coord = coords->coordinates[j];
        char str[100];
        sprintf(str, "%f", coord);

        for (int s = 0; s < strlen(str); ++s) {
            literal[literali] = str[s];
            literali++;
        }

        if (j != coords->cooordinateCount - 1) {
            literal[literali] = ',';
            literali++;
            if (true) {
                literal[literali] = ' ';
                literali++;
            }
        }
    }

    literal[literali] = endingChar;
    literali++;

    char *final = malloc((literali + 1) * sizeof(char));
    memcpy(final, literal, literali);
    final[literali] = 0;

    return final;
}