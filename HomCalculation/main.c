//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

#include <limits.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <C:\Program Files (x86)\Visual Leak Detector\include\vld.h>
#endif

#include "simplex.h"
#include "generate_complex.h"

//#include "polytop_struct.h"

#define MAX_STRING_LEN 1024

Complex** complexes;
int complexesCount;

void input_mannual()
{
    printf("\nFirst Complex:");
    char firstComplexLitteral[8192];
    scanf("%s", firstComplexLitteral);
    
    printf("\nSecond Complex:");
    char secondComplexLitteral[8192];
    scanf("%s", secondComplexLitteral);
    
    complexesCount = 2;
    complexes      = malloc(complexesCount * sizeof(Complex*));
    complexes[0]   = literalToComplex(firstComplexLitteral);
    complexes[1]   = literalToComplex(secondComplexLitteral);
}

void input_random()
{
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
    
    Complex* firstComplex;
    Complex* secondComplex;
    
    if (strcmp(sameLimit, "yes") == 0 || strcmp(sameLimit, "y") == 0 || strcmp(sameLimit, "1") == 0) {
        firstComplex  = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        secondComplex = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
    } else {
        int maxFacetCount1 = 0, maxDimenssion1 = 0, pointsCount1 = 0;
        
        printf("\nSecond Max Facet Count:");
        scanf("%d", &maxFacetCount1);
        
        printf("\nnSecond Max Dimmension:");
        scanf("%d", &maxDimenssion1);
        
        printf("\nnSecond Point Count:");
        scanf("%d", &pointsCount1);
        
        firstComplex  = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        secondComplex = generateComplex(maxFacetCount1, maxDimenssion1, pointsCount1);
    }
    
    printf("\nComplexes:\n%s\n%s\n", complexToLiteral(firstComplex, true), complexToLiteral(secondComplex, true));
    
    complexesCount = 2;
    complexes      = malloc(complexesCount * sizeof(Complex*));
    complexes[0]   = firstComplex;
    complexes[1]   = secondComplex;
}

void input_file_random()
{
    LD_File* file = Init_file_util_ext("./hom_data", "comp", true);
    
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
    complexes      = malloc(complexesCount * sizeof(Complex*));


    
    char str[15];
    sprintf(str, "%d", complexCount);
    wrtieLine(file, str, false);
    for (int i = 0; i < complexCount; ++i) {
        Complex* complex     = generateComplex(maxFacetCount, maxDimenssion, pointsCount);
        char* complexLiteral = complexToLiteral(complex, true);
        complexes[i]         = complex;

        wrtieLine(file, complexLiteral, false);
        printf("%s\n", complexLiteral);
        fflush(stdout);
    }
    
    printf("\nFile Path:\n%s", file->path);
    fflush(stdout);
}

void input_file()
{
    printf("\nFiles Path:");
    char path[1024];
    scanf("%s", path);
    
    LD_File* file = Init_file_util(path, false);
    char* file_content = readFile(file);
    
    int complexCount = 0, offset = 0;
    sscanf(file_content, "%d", &complexCount);
    
    char str[15];
    sprintf(str, "%d", complexCount);
    offset = (int) strlen(str);
    
    complexesCount = complexCount;
    complexes      = malloc(complexesCount * sizeof(Complex*));
    
    for (int i = 0; i < complexCount; ++i) {
        char* literalComplex = malloc(2048);
        sscanf(&file_content[offset], "%s", literalComplex);
        offset += strlen(literalComplex);
        
        while (file_content[offset] != '\n') {
            offset++;
        }
        
        if (strlen(literalComplex) > 4) {
            Complex* complex = literalToComplex(literalComplex);
            complexes[i]     = complex;
            printf("\n%s\n", complexToLiteral(complex, true));
        }
        free(literalComplex);
    }
}

int main(int argc, const char * argv[])
{
//    Polytop* pol = literalToPolytop("[[1,2], [2,3], [ (6.421, 4.213), (1.32131, 3.421312), (2.3412, 2.32112)]]");
//    
//    printf("\n%s\n", polytopToLiteral(pol));
//    return 0;
   
	Calculate_Hom(literalToComplex("[[1,2,3], [4,5,3]]"), literalToComplex("[[1,2,3], [4,5,3]]"));

//    Calculate_Hom(literalToComplex("[[1,2,7],[2,3,7],[3,4,7],[4,5,7],[5,6,7],[6,1,7]]"), literalToComplex("[[1,2,7],[2,3,7],[3,4,7],[4,5,7],[5,6,7],[6,1,7]]"));
    
    exit(0);
    printf("Input Type(Types: file, file-random-generated, random-generated, mannual):");
    char buffer[255];
    scanf("%s", buffer);
    
    if (strcmp(buffer, "file") == 0 || strcmp(buffer, "1") == 0) {
        input_file();
    } else if (strcmp(buffer, "file-random-generated") == 0  || strcmp(buffer, "2") == 0) {
        input_file_random();
    } else if (strcmp(buffer, "random-generated") == 0  || strcmp(buffer, "3") == 0) {
        input_random();
    } else {
        input_mannual();
    }
    
    for (int i = 0; i < complexesCount; ++i) {
        Complex* comp1 = complexes[i];
        for (int j = 0; j < complexesCount; ++j) {
            if (j != i) {
                Complex* comp2 = complexes[j];
                Calculate_Hom(comp1, comp2);
            }
        }
        break;
    }
    
    printf("\nThe End!\n");
    return 0;
}
