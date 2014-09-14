//
//  polytop_struct.c
//  HomCalculation
//
//  Created by Luka Dodelia on 6/15/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "polytop_struct.h"

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


PolCoordinates *Init_PolCoordinates() {
    PolCoordinates *polCoords = (PolCoordinates *) malloc(sizeof(PolCoordinates));
    polCoords->polcooordinateIndex = -1;
    polCoords->polcooordinateCount = 0;
    polCoords->polcooordinateCapacity = 1;
    polCoords->polcoordinates = malloc(polCoords->polcooordinateCapacity * sizeof(Coordinates));

    return polCoords;
}

void Dest_PolCoordinates(PolCoordinates *polCoords) {
    for (int i = 0; i < polCoords->polcooordinateCount; ++i) {
        Dest_Coordinates(polCoords->polcoordinates[i]);
    }
    free(polCoords->polcoordinates);
    free(polCoords);
}

Polytop *Init_Polytop() {
    Polytop *polytop = (Polytop *) malloc(sizeof(Polytop));
    polytop->polsimplexIndex = -1;
    polytop->polsimplexCount = 0;
    polytop->polsimplexCapacity = 1;
    polytop->polsimplexes = malloc(polytop->polsimplexCapacity * sizeof(Simplex));
    polytop->polcoordinates = Init_PolCoordinates();

    return polytop;
}

void Dest_Polytop(Polytop *polytop) {
    for (int i = 0; i < polytop->polsimplexCount; ++i) {
        Dest_Simplex(polytop->polsimplexes[i]);
    }

    Dest_PolCoordinates(polytop->polcoordinates);
    free(polytop->polsimplexes);
    free(polytop);
}


PolytopComplex *Init_PolytopComplex() {
    PolytopComplex *polytopComplex = (PolytopComplex *) malloc(sizeof(PolytopComplex));
    polytopComplex->polytopIndex = -1;
    polytopComplex->polytopCount = 0;
    polytopComplex->polytopCapacity = 1;
    polytopComplex->polytopes = malloc(polytopComplex->polytopCapacity * sizeof(Polytop));

    return polytopComplex;
}

void Dest_PolytopCoplex(PolytopComplex *polyComp) {
    for (int i = 0; i < polyComp->polytopCount; ++i) {
        Dest_Polytop(polyComp->polytopes[i]);
    }

    free(polyComp->polytopes);
    free(polyComp);
}


void addPolSimplex(Polytop *pol, Simplex *simp) {
    if (pol->polsimplexCapacity <= pol->polsimplexIndex + 1) {
        pol->polsimplexCapacity <<= 1;
        pol->polsimplexes = realloc(pol->polsimplexes, pol->polsimplexCapacity * sizeof(Simplex));
    }

    pol->polsimplexCount++;
    pol->polsimplexes[pol->polsimplexIndex + 1] = simp;
    pol->polsimplexIndex = pol->polsimplexCount - 1;
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

void addPolCoodinate(PolCoordinates *polCoord, Coordinates *coord) {
    if (polCoord->polcooordinateCapacity <= polCoord->polcooordinateIndex + 1) {
        polCoord->polcooordinateCapacity <<= 1;
        polCoord->polcoordinates = realloc(polCoord->polcoordinates, polCoord->polcooordinateCapacity * sizeof(Coord));
    }

    polCoord->polcooordinateCount++;
    polCoord->polcoordinates[polCoord->polcooordinateIndex + 1] = coord;
    polCoord->polcooordinateIndex = polCoord->polcooordinateCount - 1;
}

void addPolytop(PolytopComplex *polComp, Polytop *pol) {
    if (polComp->polytopCapacity <= polComp->polytopIndex + 1) {
        polComp->polytopCapacity <<= 1;
        polComp->polytopes = realloc(polComp->polytopes, polComp->polytopCapacity * sizeof(Polytop));
    }

    polComp->polytopCount++;
    polComp->polytopes[polComp->polytopIndex + 1] = pol;
    polComp->polytopIndex = polComp->polytopCount - 1;
}

Polytop *getPolytopAt(PolytopComplex *polComp, int index) {
    if (polComp->polytopIndex < index) {
        return NULL;
    } else {
        return polComp->polytopes[index];
    }
}

Simplex *getPolSimpexAt(Polytop *pol, int index) {
    if (pol->polsimplexIndex < index) {
        return NULL;
    } else {
        return pol->polsimplexes[index];
    }
}

Coord getCoordtAt(Coordinates *coords, int index) {
    if (coords->cooordinateIndex < index) {
        return -1;
    } else {
        return coords->coordinates[index];
    }
}

Coordinates *getPolCoordtAt(PolCoordinates *polCoords, int index) {
    if (polCoords->polcooordinateIndex < index) {
        return NULL;
    } else {
        return polCoords->polcoordinates[index];
    }
}

bool containsCoord(Coordinates *coords, Coord x) {
    for (int i = 0; i < coords->cooordinateCount; ++i) {
        Coord x2 = getCoordtAt(coords, i);
        if (x == x2) {
            return true;
        }
    }

    return false;
}


bool containsPolSimplex(Polytop *pol, Simplex *simp) {
    for (int i = 0; i < pol->polsimplexCount; ++i) {
        Simplex *simp2 = getPolSimpexAt(pol, i);
        char *literal1 = simplexToLiteral(simp);
        char *literal2 = simplexToLiteral(simp2);

        if (strcmp(literal1, literal2) == 0) {
            //            free(literal1);
            //            free(literal2);
            return true;
        }
        //        free(literal1);
        //        free(literal2);
    }

    return false;
}


bool containsPolytop(PolytopComplex *polComp, Polytop *pol) {
    return false;
}


Polytop *literalToPolytop(char *literal) {
    Polytop *polytop = NULL;

    int magicNumberInput = 4;
    int sbracketsCount = 0;
    int bracketCount = 0;

    char *posibleSimplexElem = (char *) malloc(magicNumberInput * sizeof(char));
    int posibleSimplexElemi = 0;

    for (int i = 0; i < strlen(literal); ++i) {
        char aChar = literal[i];

        if (aChar == '[') {
            aChar = startingChar;
        } else if (aChar == ']') {
            aChar = endingChar;
        }

        if (aChar == startingChar) {
            if (sbracketsCount == 0) {
                polytop = Init_Polytop();
            } else if (sbracketsCount == 1 && literal[i + 1] != '(' && literal[i + 2] != '(') {
                addPolSimplex(polytop, Init_Simplex());
            }
            sbracketsCount++;
        } else if (aChar == '(') {
            addPolCoodinate(polytop->polcoordinates, Init_Coordinates());
            bracketCount++;
        } else if (sbracketsCount == 2 && aChar != ',' && aChar != endingChar && aChar != '\0' && aChar != ' ' && aChar != ')') {
            posibleSimplexElem[posibleSimplexElemi] = aChar;
            posibleSimplexElemi++;
            if (posibleSimplexElemi > magicNumberInput) {
                magicNumberInput <<= 1;
                posibleSimplexElem = realloc(posibleSimplexElem, magicNumberInput * sizeof(char));
            }
        } else if (sbracketsCount == 2 && aChar == ',') {
            posibleSimplexElem = realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char));
            if (bracketCount == 1) {
                addCoordinate(getPolCoordtAt(polytop->polcoordinates, polytop->polcoordinates->polcooordinateIndex), atof(posibleSimplexElem));
            } else if (posibleSimplexElemi > 0) {
                addElement(getPolSimpexAt(polytop, polytop->polsimplexIndex), atoi(posibleSimplexElem));
            }
            posibleSimplexElemi = 0;
        } else if (aChar == ')') {
            posibleSimplexElem = realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char));
            addCoordinate(getPolCoordtAt(polytop->polcoordinates, polytop->polcoordinates->polcooordinateIndex), atof(posibleSimplexElem));
            posibleSimplexElemi = 0;
            bracketCount--;
        } else if (aChar == endingChar) {
            if (sbracketsCount == 2 && posibleSimplexElemi > 0) {
                posibleSimplexElem = realloc(posibleSimplexElem, posibleSimplexElemi * sizeof(char)); // grabing last element

                addElement(getPolSimpexAt(polytop, polytop->polsimplexIndex), atoi(posibleSimplexElem));

                posibleSimplexElemi = 0;
            } else if (sbracketsCount == 1) {
                break;
            }
            sbracketsCount--;
        }

    }


    return polytop;
}

PolytopComplex *litralToPolytopComplex(char *literal) {
    PolytopComplex *polComp = Init_PolytopComplex();

    return polComp;
}


char *coordinatesToLiteral(PolCoordinates *polCoords) {
    char *literal = malloc(2048 * sizeof(char));
    int literalIndex = 0;

    literal[literalIndex] = startingChar;
    literalIndex++;

    for (int i = 0; i < polCoords->polcooordinateCount; ++i) {
        Coordinates *coords = getPolCoordtAt(polCoords, i);
        literal[literalIndex] = '(';
        literalIndex++;
        for (int j = 0; j < coords->cooordinateCount; ++j) {
            Coord x = getCoordtAt(coords, j);
            char str[100];
            sprintf(str, "%f", x);

            for (int s = 0; s < strlen(str); ++s) {
                literal[literalIndex] = str[s];
                literalIndex++;
            }
            if (j != coords->cooordinateCount - 1) {
                literal[literalIndex] = ',';
                literalIndex++;
                literal[literalIndex] = ' ';
                literalIndex++;
            }

        }
        literal[literalIndex] = ')';
        literalIndex++;
        if (i != polCoords->polcooordinateCount - 1) {
            literal[literalIndex] = ',';
            literalIndex++;
            literal[literalIndex] = ' ';
            literalIndex++;
        }
    }

    literal[literalIndex] = endingChar;
    literalIndex++;

    return literal;
}

char *polytopToLiteral(Polytop *polytop) {
    char *literal = malloc(2048 * sizeof(char));
    int literalIndex = 0;
    literal[literalIndex] = startingChar;
    literalIndex++;

    for (int i = 0; i < polytop->polsimplexCount; ++i) {
        char *simpLiteral = simplexToLiteral(getPolSimpexAt(polytop, i));
        for (int j = 0; j < strlen(simpLiteral); ++j) {
            literal[literalIndex] = simpLiteral[j];
            literalIndex++;
        }
        literal[literalIndex] = ',';
        literalIndex++;

        literal[literalIndex] = ' ';
        literalIndex++;

    }

    char *coordinateLiteral = coordinatesToLiteral(polytop->polcoordinates);
    for (int i = 0; i < strlen(coordinateLiteral); ++i) {
        literal[literalIndex] = coordinateLiteral[i];
        literalIndex++;
    }

    literal[literalIndex] = endingChar;
    literalIndex++;

    return literal;
}

char *polytopCoplexToLiteral(PolytopComplex *polComp) {
    char *literal = malloc(1);

    return literal;
}







