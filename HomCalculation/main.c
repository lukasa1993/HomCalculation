//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include "simplex.h"
#include "generate_complex.h"

//#include "polytop_struct.h"

#define MAX_STRING_LEN 1024

Complex **complexes;
int complexesCount;

void input_mannual() {
    printf("\nFirst Complex:");
    char firstComplexLitteral[8192];
    scanf("%s", firstComplexLitteral);

    printf("\nSecond Complex:");
    char secondComplexLitteral[8192];
    scanf("%s", secondComplexLitteral);

    complexesCount = 2;
    complexes = malloc(complexesCount * sizeof(Complex *));
    complexes[0] = literalToComplex(firstComplexLitteral);
    complexes[1] = literalToComplex(secondComplexLitteral);
}

void input_random() {
    int maxFacetCount = 0, maxDimenssion = 0, pointsCount = 0;

    printf("\nMax Facet Count:");
    scanf("%d", &maxFacetCount);

    printf("\nMax Dimmension:");
    scanf("%d", &maxDimenssion);

    printf("\nPoint Count:");
    scanf("%d", &pointsCount);

    printf("\nSame Limit For Second?:");
    char sameLimit[128];
    scanf("%s", sameLimit);

    Complex *firstComplex;
    Complex *secondComplex;

    if (strcmp(sameLimit, "yes") == 0 || strcmp(sameLimit, "y") == 0 || strcmp(sameLimit, "1") == 0) {
        firstComplex = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        secondComplex = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
    } else {
        int maxFacetCount1 = 0, maxDimenssion1 = 0, pointsCount1 = 0;

        printf("\nSecond Max Facet Count:");
        scanf("%d", &maxFacetCount1);

        printf("\nnSecond Max Dimmension:");
        scanf("%d", &maxDimenssion1);

        printf("\nnSecond Point Count:");
        scanf("%d", &pointsCount1);

        firstComplex = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        secondComplex = generateComplex(maxFacetCount1, maxDimenssion1, pointsCount1);
    }

    printf("\nComplexes:\n%s\n%s\n", complexToLiteral(firstComplex, true), complexToLiteral(secondComplex, true));

    complexesCount = 2;
    complexes = malloc(complexesCount * sizeof(Complex *));
    complexes[0] = firstComplex;
    complexes[1] = secondComplex;
}

void input_file_random() {
    LD_File *file = Init_file_util_ext("./hom_data", "comp", true);

    printf("\nMaximum Complex Count:");
    int maxComplexCount = 0, maxFacetCount = 0, maxDimenssion = 0, pointsCount = 0;
    scanf("%d", &maxComplexCount);

    printf("\nMax Facet Count:");
    scanf("%d", &maxFacetCount);

    printf("\nMax Dimmension:");
    scanf("%d", &maxDimenssion);

    printf("\nPoint Count:");
    scanf("%d", &pointsCount);

    srand((int) time(NULL));
    int complexCount = random_in_range(1, maxComplexCount);
    printf("\nGenerating %d Complexes\n", complexCount);
    fflush(stdout);

    complexesCount = complexCount;
    complexes = malloc(complexesCount * sizeof(Complex *));


    char str[15];
    sprintf(str, "%d", complexCount);
    wrtieLine(file, str, false);
    for (int i = 0; i < complexCount; ++i) {
        Complex *complex = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        char *complexLiteral = complexToLiteral(complex, true);
        complexes[i] = complex;

        wrtieLine(file, complexLiteral, false);
        printf("%s\n", complexLiteral);
        fflush(stdout);
    }

    printf("\nFile Path:\n%s", file->path);
    fflush(stdout);
}

void input_file() {
    printf("\nFiles Path:");
    char path[1024];
    scanf("%s", path);

    LD_File *file = Init_file_util(path, false);
    char *file_content = readFile(file);

    int complexCount = 0, offset = 0;
    sscanf(file_content, "%d", &complexCount);

    char str[15];
    sprintf(str, "%d", complexCount);
    offset = (int) strlen(str);

    complexesCount = complexCount;
    complexes = malloc(complexesCount * sizeof(Complex *));

    for (int i = 0; i < complexCount; ++i) {
        char *literalComplex = malloc(2048);
        sscanf(&file_content[offset], "%s", literalComplex);
        offset += strlen(literalComplex);

        while (file_content[offset] != '\n') {
            offset++;
        }

        if (strlen(literalComplex) > 4) {
            Complex *complex = literalToComplex(literalComplex);
            complexes[i] = complex;
            printf("\n%s\n", complexToLiteral(complex, true));
        }
        free(literalComplex);
    }
}

int main(int argc, const char *argv[]) {
    clock_t begin, end;
    double time_spent;

    begin = clock();

//    char* A = "[[1,2,6],[2,3,6],[3,4,6],[4,5,6],[1,5,6]]";
//    char* B = "[[1,2,6],[2,3,4],[2,4,6],[4,5,6]]";


    char *n1 = "[[1,2,3],[2,3,4],[3,4,5],[4,5,6],[1,5,6],[2,4,6],[1,2,6]]";
    char *n2 = "[[1,2,6],[2,3,6],[3,4,6],[4,5,6],[1,5,6]]";
    char *n3 = "[[1,2,6],[2,3,4],[4,5,6],[2,4,6]]";

//    Calculate_Hom(literalToComplex(n1), literalToComplex(n2));
//    Calculate_Hom(literalToComplex(n2), literalToComplex(n1));
//    Calculate_Hom(literalToComplex(n1), literalToComplex(n3));
//    Calculate_Hom(literalToComplex(n3), literalToComplex(n1));

    char *l1 = "[[1,2],[2,3],[3,4],[1,4]]";
    char *l2 = "[[1,2,3],[2,4,5]]";

    Complex *A = literalToComplex(l1);
    Complex *B = literalToComplex(l2);

    for (int i = 0; i < A->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(A, i);
        simp->allowedSubSimplexes = complexToLiteral(AllSubSimplexses(simp), true);
    }


    Calculate_Hom(A, B);
//
//    char* C = "[[1,2,4],[2,3,5],[3,1,6],[1,4,6],[4,2,5],[4,5,6],[3,6,5]]";

    // ------ can't calcualate? --------------

//    Calculate_Hom(literalToComplex(A), literalToComplex(C));
//
//    Calculate_Hom(literalToComplex(B), literalToComplex(C));
//
//    Calculate_Hom(literalToComplex(C), literalToComplex(A));
//    Calculate_Hom(literalToComplex(C), literalToComplex(B));
//    Calculate_Hom(literalToComplex(C), literalToComplex(C));

    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("\n %f The End.. \n", time_spent);
}
