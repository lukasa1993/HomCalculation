//
//  main.c
//  HomCalculation
//
//  Created by Luka Dodelia on 12/4/13.
//  Copyright (c) 2013 Luka Dodelia. All rights reserved.
//

//#include "lis.h"
//#include "lis_bridge.h"
#include "simplex.h"
#include "generate_complex.h"

//#include "poly_birdge.h"

//#include "polytop_struct.h"

int main(int argc, char *argv[]) {
    clock_t begin, end;
    double time_spent;

    begin = clock();

    // a b c d e f g h
    // 1 2 3 4 5 6 7 8


    Complex *A = literalToComplex("[[1,2,3,4],[3,4,7,8],[5,6,7,8],[1,2,5,6],[2,3,6,7],[1,4,5,8],[1,7]]");

    Simplex *simpA1 = getSimpexAt(A, 0);
    simpA1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");

    Simplex *simpA2 = getSimpexAt(A, 1);
    simpA2->allowedSubSimplexes = literalToComplex("[[3],[4],[7],[8],[3,4],[3,7],[4,7],[7,8],[3,4,7,8]]");

    Simplex *simpA3 = getSimpexAt(A, 2);
    simpA3->allowedSubSimplexes = literalToComplex("[[5],[6],[7],[8],[5,6],[6,7],[7,8],[5,8],[5,6,7,8]]");

    Simplex *simpA4 = getSimpexAt(A, 3);
    simpA4->allowedSubSimplexes = literalToComplex("[[1],[2],[5],[6],[1,2],[1,5],[2,6],[5,6],[1,2,5,6]]");

    Simplex *simpA5 = getSimpexAt(A, 4);
    simpA5->allowedSubSimplexes = literalToComplex("[[2],[3],[6],[7],[2,3],[3,7],[6,7],[2,6],[2,3,6,7]]");

    Simplex *simpA6 = getSimpexAt(A, 5);
    simpA6->allowedSubSimplexes = literalToComplex("[[1],[4],[5],[8],[1,4],[1,5],[4,8],[5,8],[1,4,5,8]]");

    Simplex *simpA7 = getSimpexAt(A, 6);
    simpA7->allowedSubSimplexes = literalToComplex("[[1],[6],[1,7]");

    Complex *B = literalToComplex("[[1,2,3,4],[1,2,5,6],[3,4,5,6]]");

    Simplex *simpB1 = getSimpexAt(B, 0);
    simpB1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");

    Simplex *simpB2 = getSimpexAt(B, 1);
    simpB2->allowedSubSimplexes = literalToComplex("[[1],[2],[5],[6],[1,2],[1,5],[2,6],[5,6],[1,2,5,6]]");

    Simplex *simpB3 = getSimpexAt(B, 2);
    simpB3->allowedSubSimplexes = literalToComplex("[[3],[4],[5],[6],[3,4],[4,5],[5,6],[3,6],[3,4,5,6]]");


    Complex *C = literalToComplex("[[1,2,3,4],[1,2,5,6],[5,6,7,8],[3,4,5,8]]");

    Simplex *simpC1 = getSimpexAt(C, 0);
    simpC1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");

    Simplex *simpC2 = getSimpexAt(C, 1);
    simpC2->allowedSubSimplexes = literalToComplex("[[1],[2],[5],[6],[1,2],[2,5],[5,6],[1,6],[1,2,5,6]]");

    Simplex *simpC3 = getSimpexAt(C, 2);
    simpC3->allowedSubSimplexes = literalToComplex("[[5],[6],[7],[8],[5,6],[6,7],[7,8],[5,8],[5,6,7,8]]");

    Simplex *simpC4 = getSimpexAt(C, 3);
    simpC4->allowedSubSimplexes = literalToComplex("[[3],[4],[5],[8],[3,4],[4,8],[5,8],[3,5],[3,4,5,8]]");

//    Calculate_Hom(A, A);
//    Calculate_Hom(A, B);
//    Calculate_Hom(A, C);

//    Calculate_Hom(B, B);
//    Calculate_Hom(B, A);
//    Calculate_Hom(B, C);

    Calculate_Hom(C, C);
//    Calculate_Hom(C, A);
//    Calculate_Hom(C, B);

    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("\n %f The End.. \n", time_spent);

    return 0;
}
