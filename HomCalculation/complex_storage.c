//
//  complex_storage.c
//  HomCalculation
//
//  Created by Luka Dodelia on 6/28/14.
//  Copyright (c) 2014 Luka Dodelia. All rights reserved.
//

#include "complex_storage.h"


Complex_Storage *Init_Storage() {
    Complex_Storage *storage = malloc(sizeof(Complex_Storage));

    storage->literalIndex    = -1;
    storage->lietralCount    = 0;
    storage->literalCapacity = 1;
    storage->complexLiterals = malloc((size_t) (storage->literalCapacity * sizeof(char *)));

    return storage;
}

void Destory_Storage(Complex_Storage *storage) {
    for (int i = 0; i < storage->lietralCount; ++i) {
        free(storage->complexLiterals[i]);
    }
    free(storage->complexLiterals);
    free(storage);

}

void addLiteral(Complex_Storage *storage, char *literal) {
    if (storage->literalCapacity <= storage->literalIndex + 1) {
        storage->literalCapacity <<= 1;
        storage->complexLiterals = realloc(storage->complexLiterals,
                                           (size_t) (storage->literalCapacity * sizeof(char *)));
    }

    storage->lietralCount++;
    storage->complexLiterals[storage->literalIndex + 1] = literal;
    storage->literalIndex = storage->lietralCount - 1;
}

bool containsLiteral(Complex_Storage *storage, char *literal) {
    for (long long i = 0; i < storage->lietralCount; ++i) {
        char *lit = getLiteralAt(storage, i);
        if (lit != NULL && strcmp(literal, lit) == 0) {
            return true;
        }
    }
    return false;
}

char *getLiteralAt(Complex_Storage *storage, long long index) {
    if (storage->literalIndex < index) {
        return NULL;
    } else {
        return storage->complexLiterals[index];
    }
}

