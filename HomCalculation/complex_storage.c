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

    storage->literalIndex = -1;
    storage->lietralCount = 0;
    storage->literalCapacity = 1;
    storage->complexLiterals = malloc((size_t) (storage->literalCapacity * sizeof(char *)));

    TrieCreate(&storage->trie_root);
    return storage;
}

void Destory_Storage(Complex_Storage *storage) {
    TrieDestroy(storage->trie_root);
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

    char cpt[200];
    strcpy(cpt, literal);


    TrieAdd(&storage->trie_root, cpt, (int) storage->literalIndex);
}

bool containsLiteral(Complex_Storage *storage, char *literal) {
    trieNode_t *a = TrieSearch(storage->trie_root->children, literal);

    return a != NULL;
}

char *getLiteralAt(Complex_Storage *storage, long long index) {
    if (storage->literalIndex < index) {
        return NULL;
    } else {
        return storage->complexLiterals[index];
    }
}

