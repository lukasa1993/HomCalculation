//
// Created by Luka Dodelia on 4/26/15.
//

#include "lis_bridge.h"

void basisAlphas(Coordinates **coords, int length, int dim) {
    int i, j, n = dim, dim_i, xi;

    for (dim_i = dim + 1; dim_i < length; ++dim_i) {
        Coordinates *coord = coords[dim_i];

        LIS_MATRIX A;
        LIS_VECTOR b, x;
        LIS_SOLVER solver;

        lis_vector_create(0, &b);
        lis_vector_set_size(b, 0, coord->cooordinateCount);

        lis_vector_create(0, &x);
        lis_vector_set_size(x, 0, length);

        for (xi = 0; xi < coord->cooordinateCount; ++xi) {
            lis_vector_set_value(LIS_INS_VALUE, xi, getCoordtAt(coord, xi), b);
        }
        lis_vector_print(b);

        lis_matrix_create(0, &A);
        lis_matrix_set_size(A, 0, n);

        Coordinates *first = coords[0];

        for (i = 1; i < n; ++i) {
            Coordinates *current = coords[i];
            for (j = 0; j < n; ++j) {
                double a  = getCoordtAt(current, j);
                double a1 = getCoordtAt(first, j);
                lis_matrix_set_value(LIS_INS_VALUE, i, j, a - a1, A);
            }
        }
        lis_matrix_set_type(A, LIS_MATRIX_CSR);
        lis_matrix_assemble(A);


        lis_solver_create(&solver);
        lis_solver_set_option("-i bicg -p none", solver);
        lis_solver_set_option("-tol 1.0e-12", solver);

        lis_solve(A, b, x, solver);
        lis_vector_print(b);
    }

}