//
//  complex_storage.h
//  HomCalculation
//
//  Created by Luka Dodelia on 6/28/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#ifndef HomCalculation_complex_storage_h
#define HomCalculation_complex_storage_h

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct Complex_Storage {
    long long literalIndex;
    long long lietralCount;
    long long literalCapacity;
    char **complexLiterals;
} Complex_Storage;

Complex_Storage *Init_Storage();

void Destory_Storage(Complex_Storage *storage);

void addLiteral(Complex_Storage *storage, char *literal);

char *getLiteralAt(Complex_Storage *storage, long long index);

#endif
