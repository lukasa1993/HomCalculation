//
// Created by lukasa on 12/4/15.
//

#include "ipsolve_bridge.h"

#include "lp_lib.h"

bool solve_complex(Complex *A, Complex *B, Complex *fsi) {

    SimplexElem pointNumber = fsi->simplexCount;
    Simplex *Ai = NULL;

    for (int i = 0; i < A->simplexCount; ++i) {
        Simplex *simp = getSimpexAt(A, i);
        if (containsElement(simp, pointNumber) && (simp->dimension + 1) < pointNumber) {
            Ai = simp;
            break;
        }
    }

    if (Ai == NULL) {
        return true;
    }

//    printf("\nsolve start \n");
//    printf("\n %s \n", complexToLiteral(fsi, true));

    lprec *lp;
    int Ncol, *colno = NULL, ret = 0;
    REAL *row = NULL;

    Matrix *matrix = Init_Matrix();

    Complex *Binquelity = Init_Complex();

    int dimensionSum = 0;
    for (int j = 0; j < fsi->simplexCount; ++j) {
        Simplex *Ki = getSimpexAt(fsi, j);

        int dimCheck = -1;
        for (int i = 0; i < B->simplexCount; ++i) {
            Simplex *Bi = getSimpexAt(B, i);
            for (int ij = 0; ij < Bi->allowedSubSimplexes->simplexCount; ++ij) {
                Simplex *Bij = getSimpexAt(Bi->allowedSubSimplexes, ij);
                if (strcmp(simplexToLiteral(Bij), simplexToLiteral(Ki)) == 0) {
                    dimCheck += Bi->coodinates->columns;
                    addSimplex(Binquelity, Bij);
                    break;
                }

            }
            if (dimCheck == -1) {
                printf("\nProblem: %s\n", simplexToLiteral(Bi));
            } else {
                break;
            }
        }

        if (dimCheck != -1) {
            dimensionSum += dimCheck;
        } else {
            printf("\nProblem: %s\n", complexToLiteral(fsi, true));
        }
    }
    dimensionSum += 1;
    matrix->columns = dimensionSum;
    matrix->rows = 0;

    int lastRowCount = 0;
    int lastColumnCount = 0;
    int padding = 0;
    for (int k = 0; k < fsi->simplexCount; ++k) {
        Simplex *Bij = getSimpexAt(Binquelity, k);
        Matrix *BiInequelity = Bij->inequalityMatrix;

        lastRowCount = 0;
        for (int i = 0; i < BiInequelity->rows; ++i) {
            int prevFill = padding;
            int dimensionFill = dimensionSum - padding;
            for (int l = 0; l < prevFill; ++l) {
                addMElement(matrix, 0);
            }

            dimensionFill--;
            for (int j = 0; j < BiInequelity->columns - 1; ++j) {
                addMElement(matrix, getMatrixElem(BiInequelity, i, j));
                dimensionFill--;
                prevFill++;
            }
            lastColumnCount = BiInequelity->columns - 1;
            for (int r = 0; r < dimensionFill; ++r) {
                addMElement(matrix, 0);
            }
            addMElement(matrix, getMatrixElem(BiInequelity, i, BiInequelity->columns - 1));
            matrix->rows++;
            lastRowCount++;
        }
        padding += BiInequelity->columns - 1;
    }

    SimplexElem skip_rows = (Ai->dimension + 1) + ((pointNumber - 1) - (Ai->dimension + 1)) * Ai->dimension;


    for (int m = matrix->rows - lastRowCount; m < matrix->rows; ++m) {

        int ci = skip_rows;
        int mjStart = 0;

        for (int h = matrix->columns - 1 - lastColumnCount; h < matrix->columns - 1; ++h) {
            double mElem = getMatrixElem(matrix, m, h);

            int mj = mjStart;
            for (int cj = 0; cj < Ai->coodinates->columns; ++cj) {

                double cElem = getMatrixElem(Ai->coodinates, ci, cj);

                setMatrixElem(matrix, m, mj, cElem * mElem);

                mj += lastColumnCount;
            }
            setMatrixElem(matrix, m, h, 0);
            mjStart++;
            ci++;
        }
    }

//    printf("\n%d\n", lastRowCount);
//    printf("\n%s\n", matrixToLiteral(Ai->coodinates));

//    if(strcmp(complexToLiteral(fsi, false), "[[1],[2],[3],[1]]") == 0) {
//        printf("\n%d\n", dimensionSum);
//        printf("\n%s\n", complexToLiteral(fsi, true));
//        printf("\n%s\n", matrixToLiteral(matrix));
//    }

//
//    exit(-1);

    Ncol = matrix->columns - 1;
    lp = make_lp(0, Ncol);
    if (lp == NULL)
        ret = 1; /* couldn't construct a new model... */

    set_break_at_first(lp, TRUE);

    if (ret == 0) {
        /* let us name our variables. Not required, but can be useful for debugging */
        for (int i = 1; i <= Ncol; ++i) {
            char xi[100];
            sprintf(xi, "x%d", i);

            set_col_name(lp, i, xi);
        }

        /* create space large enough for one row */
        colno = (int *) malloc(Ncol * sizeof(*colno));
        row = (REAL *) malloc(Ncol * sizeof(*row));
        if ((colno == NULL) || (row == NULL))
            ret = 2;
    }

    if (ret == 0) {
        set_add_rowmode(lp, TRUE);  /* makes building the model faster if it is done rows by row */

        // LD FOR STARTS HERE


        for (int i = 0; i < matrix->rows; ++i) {
            for (int j = 0; j < matrix->columns - 1; ++j) {
                colno[j] = j + 1;
                row[j] = getMatrixElem(matrix, i, j);
            }

            /* add the row to lpsolve */
            if (!add_constraintex(lp, Ncol, row, colno, LE, getMatrixElem(matrix, i, matrix->columns - 1)))
                ret = 3;

            if (ret != 0) {
                break;
            }
        }

        // LD FOR ENDS HERE
    }

    if (ret == 0) {
        set_add_rowmode(lp, FALSE); /* rowmode should be turned off again when done building the model */

        for (int i = 1; i <= Ncol; ++i) {
            colno[i - 1] = i;
            row[i - 1] = 1;
        }

        /* set the objective in lpsolve */
        if (!set_obj_fnex(lp, Ncol, row, colno))
            ret = 4;
    }

    if (ret == 0) {
        /* set the object direction to maximize */
        set_maxim(lp);

        /* just out of curiosity, now show the model in lp format on screen */
        /* this only works if this is a console application. If not, use write_lp and a filename */
//        write_LP(lp, stdout);
//        if(strcmp(complexToLiteral(fsi, false), "[[1],[2],[3],[4]]") == 0) {
//            print_lp(lp);
//        }
        /* write_lp(lp, "model.lp"); */

        /* I only want to see important messages on screen while solving */
        set_verbose(lp, IMPORTANT);

        /* Now let lpsolve calculate a solution */
        ret = solve(lp);
        if (ret == OPTIMAL)
            ret = 0;
        else
            ret = 5;
    }

//    if(strcmp(complexToLiteral(fsi, false), "[[1],[2],[3],[4]]") == 0) {
//        printf("\nRet: %d\n", ret);
//    }
    bool retBool = false;
    if (ret == 0) {
        /* a solution is calculated, now lets get some results */
        retBool = true;
        /* objective value */
//        printf("\nObjective value: %f\n", get_objective(lp));

        /* variable values */
//        get_variables(lp, row);
//        for (int j = 0; j < Ncol; j++)
//            printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);

        /* we are done now */
    }

    /* free allocated memory */
    if (row != NULL)
        free(row);
    if (colno != NULL)
        free(colno);

    if (lp != NULL) {
        /* clean up such that all used memory by lpsolve is freed */
        delete_lp(lp);
    }

//    exit(-1);
    return retBool;
}