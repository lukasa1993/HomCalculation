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

    //lis_initialize(&argc, &argv);

    begin = clock();

//    char* A = "[[1,2,6],[2,3,6],[3,4,6],[4,5,6],[1,5,6]]";
//    char* B = "[[1,2,6],[2,3,4],[2,4,6],[4,5,6]]";

    char *old_a = "[[1,2,7],[2,3,7],[3,4,7],[4,5,7],[5,6,7],[1,6,7]]";
    char *old_b = "[[1,2,9],[2,8,9],[2,8,10],[2,3,10],[3,5,10],[3,4,5],[5,6,10],[6,8,10],[6,7,8]]";
    char *old_c = "[[1,4,5],[1,2,4],[1,3,5],[2,4,6],[2,3,6],[3,5,6],[4,5,6]]";


    char *n1 = "[[1,2,3],[2,3,4],[3,4,5],[4,5,6],[1,5,6],[2,4,6],[1,2,6]]";
    char *n2 = "[[1,2,6],[2,3,6],[3,4,6],[4,5,6],[1,5,6]]";
    char *n3 = "[[1,2,6],[2,3,4],[4,5,6],[2,4,6]]";
    char *n4 = "[[1,2,6],[2,3,4],[4,5,6],[2,4,6]";

    Complex *A = literalToComplex(old_a);
    Complex *B = literalToComplex(old_a);

    for (int i = 0; i < A->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(A, i);
        simp->allowedSubSimplexes = AllSubSimplexses(simp);
    }

    for (int i = 0; i < B->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(B, i);
        simp->allowedSubSimplexes = AllSubSimplexses(simp);
    }
    Calculate_Hom(A, B);


//    Calculate_Hom(literalToComplex(n1), literalToComplex(n2));
//    Calculate_Hom(literalToComplex(n2), literalToComplex(n1));
//    Calculate_Hom(literalToComplex(n1), literalToComplex(n3));
//    Calculate_Hom(literalToComplex(n3), literalToComplex(n1));
//
//    char *l1 = "[[1,2],[2,3],[3,4],[1,4]]";
//    char *l2 = "[[1,2,3],[2,4,5]]";
//

//    Calculate_Hom(A, B);
//    return 0;

//    Complex *A = literalToComplex("[[1,2,3,4],[1,2,5,6],[5,6,7,8],[3,4,7,8],[2,3,5,8],[1,4,6,7],[2,7]]");
//    Complex *B = literalToComplex("[[1,2,3,4],[1,2,5,6],[5,6,7,8],[3,4,7,8],[2,3,5,8],[1,4,6,7],[2,7]}");
//
//    Simplex *simpA1 = getSimpexAt(A, 0);
//    simpA1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpB1 = getSimpexAt(B, 0);
//    simpB1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpA2 = getSimpexAt(A, 1);
//    simpA2->allowedSubSimplexes = literalToComplex("[[1],[2],[5],[6],[1,2],[2,5],[5,6],[1,6],[1,2,5,6]]");
//
//    Simplex *simpB2 = getSimpexAt(B, 1);
//    simpB2->allowedSubSimplexes = literalToComplex("[[1],[2],[5],[6],[1,2],[2,5],[5,6],[1,6],[1,2,5,6]]");
//
//    Simplex *simpA3 = getSimpexAt(A, 2);
//    simpA3->allowedSubSimplexes = literalToComplex("[[5],[6],[7],[8],[5,6],[5,7],[7,8],[6,8],[5,6,7,8]]");
//
//    Simplex *simpB3 = getSimpexAt(B, 2);
//    simpB3->allowedSubSimplexes = literalToComplex("[[5],[6],[7],[8],[5,6],[5,7],[7,8],[6,8],[5,6,7,8]]");
//
//    Simplex *simpA4 = getSimpexAt(A, 3);
//    simpA4->allowedSubSimplexes = literalToComplex("[[3],[4],[7],[8],[3,4],[3,7],[7,8],[4,8],[3,4,7,8]]");
//
//    Simplex *simpB4 = getSimpexAt(B, 3);
//    simpB4->allowedSubSimplexes = literalToComplex("[[3],[4],[7],[8],[3,4],[3,7],[7,8],[4,8],[3,4,7,8]]");
//
//    Simplex *simpA5 = getSimpexAt(A, 4);
//    simpA5->allowedSubSimplexes = literalToComplex("[[2],[4],[5],[8],[2,5],[5,7],[3,8],[2,3],[2,3,5,8]]");
//
//    Simplex *simpB5 = getSimpexAt(B, 4);
//    simpB5->allowedSubSimplexes = literalToComplex("[[2],[4],[5],[8],[2,5],[5,7],[3,8],[2,3],[2,3,5,8]]");
//
//    Simplex *simpA6 = getSimpexAt(A, 5);
//    simpA6->allowedSubSimplexes = literalToComplex("[[1],[4],[6],[7],[1,4],[1,6],[6,8],[4,8],[1,4,6,7]]");
//
//    Simplex *simpB6 = getSimpexAt(B, 5);
//    simpB6->allowedSubSimplexes = literalToComplex("[[1],[4],[6],[7],[1,4],[1,6],[6,8],[4,8],[1,4,6,7]]");
//
//    Simplex *simpA7 = getSimpexAt(A, 6);
//    simpA7->allowedSubSimplexes = literalToComplex("[[2],[7],[2,7]]");
//
//    Simplex *simpB7 = getSimpexAt(B, 6);
//    simpB7->allowedSubSimplexes = literalToComplex("[[2],[7],[2,7]]");

//    Complex *A = literalToComplex("[[1,2,3,4],[1,4,5,6],[2,3,5,6]]");
//    Complex *B = literalToComplex("[[1,2,3,4],[1,4,5,6],[2,3,5,6]]");
//
//    Simplex *simpA1 = getSimpexAt(A, 0);
//    simpA1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpB1 = getSimpexAt(B, 0);
//    simpB1->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpA2 = getSimpexAt(A, 1);
//    simpA2->allowedSubSimplexes = literalToComplex("[[1],[4],[5],[6],[1,4],[1,5],[4,6],[5,6],[1,4,5,6]]");
//
//    Simplex *simpB2 = getSimpexAt(B, 1);
//    simpB2->allowedSubSimplexes = literalToComplex("[[1],[4],[5],[6],[1,4],[1,5],[4,6],[5,6],[1,4,5,6]]");
//
//    Simplex *simpA3 = getSimpexAt(A, 2);
//    simpA3->allowedSubSimplexes = literalToComplex("[[2],[3],[5],[6],[2,3],[2,6],[3,5],[5,6],[2,3,5,6]]");
//
//    Simplex *simpB3 = getSimpexAt(B, 2);
//    simpB3->allowedSubSimplexes = literalToComplex("[[2],[3],[5],[6],[2,3],[2,6],[3,5],[5,6],[2,3,5,6]]");


//    Calculate_Hom(A, B);

//    Complex *A1 = literalToComplex("[[1,2,3,4],[1,4,5,6],[2,3,5,6]]");
//    Complex *B1 = literalToComplex("[[1,2,3,4],[1,4,5,6],[2,3,5,6]]");
//
//    Simplex *simpA11 = getSimpexAt(A1, 0);
//    simpA11->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpB11 = getSimpexAt(B1, 0);
//    simpB11->allowedSubSimplexes = literalToComplex("[[1],[2],[3],[4],[1,2],[2,3],[1,4],[3,4],[1,2,3,4]]");
//
//    Simplex *simpA12 = getSimpexAt(A1, 1);
//    simpA12->allowedSubSimplexes = literalToComplex("[[1],[4],[5],[6],[1,4],[1,5],[4,6],[5,6],[1,4,5,6]]");
//
//    Simplex *simpB12 = getSimpexAt(B1, 1);
//    simpB12->allowedSubSimplexes = literalToComplex("[[1],[4],[5],[6],[1,4],[1,5],[4,6],[5,6],[1,4,5,6]]");
//
//    Simplex *simpA13 = getSimpexAt(A1, 2);
//    simpA13->allowedSubSimplexes = literalToComplex("[[2],[3],[5],[6],[2,3],[2,5],[3,6],[5,6],[2,3,5,6]]");
//
//    Simplex *simpB13 = getSimpexAt(B1, 2);
//    simpB13->allowedSubSimplexes = literalToComplex("[[2],[3],[5],[6],[2,3],[2,5],[3,6],[5,6],[2,3,5,6]]");
//
//
//
//    Calculate_Hom(A1, B1);


//    char* C = "[[1,2,4],[2,3,5],[3,1,6],[1,4,6],[4,2,5],[4,5,6],[3,6,5]]";

    // ------ can't calculate? ----------------

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

    //lis_finalize();
}
