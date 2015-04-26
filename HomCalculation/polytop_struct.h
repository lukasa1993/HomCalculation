//
//  polytop_struct.h
//  HomCalculation
//
//  Created by Luka Dodelia on 6/15/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_polytop_struct_h
#define HomCalculation_polytop_struct_h

#include "complex_struct.h"

typedef double Coord;

typedef struct Coordinates {
    int cooordinateIndex;
    int cooordinateCount;
    int cooordinateCapacity;
    Coord *coordinates;

} Coordinates;

typedef struct PolCoordinates {
    int polcooordinateIndex;
    int polcooordinateCount;
    int polcooordinateCapacity;
    Coordinates **polcoordinates;

} PolCoordinates;

typedef struct Polytop {
    int polsimplexIndex;
    int polsimplexCount;
    int polsimplexCapacity;
    Simplex *polsimplexes;
    PolCoordinates *polcoordinates;

} Polytop;

typedef struct PolytopComplex {
    int polytopIndex;
    int polytopCount;
    int polytopCapacity;
    Polytop **polytopes;

} PolytopComplex;


Coordinates *Init_Coordinates();

void Dest_Coordinates(Coordinates *coords);

void addCoordinate(Coordinates *coord, Coord x);

bool containsCoord(Coordinates *coords, Coord x);

Coord getCoordtAt(Coordinates *coords, int index);

Polytop *Init_Polytop();

void Dest_Polytop(Polytop *polytop);

void addPolSimplex(Polytop *pol, Simplex *simp);

bool containsPolSimplex(Polytop *pol, Simplex *simp);

Simplex *getPolSimpexAt(Polytop *pol, int index);

PolytopComplex *Init_PolytopComplex();

void Dest_PolytopCoplex(PolytopComplex *polyComp);

void addPolytop(PolytopComplex *polComp, Polytop *pol);

bool containsPolytop(PolytopComplex *polComp, Polytop *pol);

Polytop *getPolytopAt(PolytopComplex *polComp, int index);


// -----

Polytop *literalToPolytop(char *literal);

PolytopComplex *litralToPolytopComplex(char *literal);

char *polytopCoplexToLiteral(PolytopComplex *polComp);

char *polytopToLiteral(Polytop *polytop);

#endif
