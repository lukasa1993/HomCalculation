//
// Created by Luka Dodelia on 4/26/15.
//

#include "lis_bridge.h"

/*
 * coords must have base Coordinates anad + 1 which
 * we are calcualation alphas for
  * */

//Coordinates *basisAlphas(Coordinates **coords, int length) {
//    int i, j, n = coords[0]->cooordinateCount, xi, mi;
//
//    Coordinates *first  = coords[0];
//    Coordinates *needed = coords[length - 1];
//
//    for (i = 0; i < length; i++) {
//        if (n != coords[i]->cooordinateCount) {
//            printf("\n dimensions doesn't match \n");
//
//            return NULL;
//        }
//    }
//
//
//    LIS_MATRIX A;
//    LIS_VECTOR b, x;
//    LIS_SOLVER solver;
//
//    lis_vector_create(0, &b);
//    lis_vector_set_size(b, 0, n);
//
//    lis_vector_create(0, &x);
//    lis_vector_set_size(x, 0, n);
//
//    for (xi = 0; xi < n; ++xi) {
//        lis_vector_set_value(LIS_INS_VALUE, xi, getCoordtAt(needed, xi), b);
//    }
//
//    lis_matrix_create(0, &A);
//    lis_matrix_set_size(A, 0, n);
//
//    for (i = 0; i < n; ++i) {
//        double a1 = getCoordtAt(first, i);
//        for (mi = 1, j = 0; mi < length - 1; ++mi, ++j) {
//            double a = getCoordtAt(coords[mi], i);
//
//            lis_matrix_set_value(LIS_INS_VALUE, i, j, a - a1, A);
//        }
//    }
//    lis_matrix_set_type(A, LIS_MATRIX_CSR);
//    lis_matrix_assemble(A);
//
//
//    lis_solver_create(&solver);
//    lis_solver_set_option("-i bicg -p none", solver);
//    lis_solver_set_option("-tol 1.0e-12", solver);
//
//    lis_solve(A, b, x, solver);
//
//    Coordinates *alphas = Init_Coordinates();
//    for (i = 0; i < n; ++i) {
//        double alpha;
//        lis_vector_get_value(x, i, &alpha);
//        addCoordinate(alphas, alpha);
//    }
//    return alphas;
//}
