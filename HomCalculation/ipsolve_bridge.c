//
// Created by lukasa on 12/4/15.
//

#include "ipsolve_bridge.h"

#include "lp_lib.h"

void solve_complex(Complex *A, Complex *B, Complex *fsi) {

    printf("\nsolve start \n");
    printf("\n %s \n", complexToLiteral(fsi, true));

    lprec *lp;
    int Ncol, *colno = NULL, ret = 0;
    REAL *row = NULL;

    Matrix *matrix = Init_Matrix();

    Matrix *inquelityMatrixes[fsi->simplexCount];
    int inquelityMatrixes_index = 0;

    int dimensionSum = 0;
    for (int j = 0; j < fsi->simplexCount; ++j) {
        Simplex *Ki = getSimpexAt(fsi, j);

        int dimCheck = -1;
        for (int i = 0; i < B->simplexCount; ++i) {
            Simplex *Bi = getSimpexAt(B, i);
            if (containsSubSimplex(Bi, Ki)) {
                for (int ij = 0; ij < Bi->allowedSubSimplexes->simplexCount; ++ij) {
                    if (containsSubSimplex(getSimpexAt(Bi->allowedSubSimplexes, ij), Ki)) {
                        dimCheck += Bi->coodinates->rows;
                        inquelityMatrixes[inquelityMatrixes_index++] = Bi->inequalityMatrix;
                    }
                }
                if (dimCheck == -1) {
                    printf("\nProblem: %s\n", simplexToLiteral(Bi));
                }
                break;
            }
        }

        if (dimCheck != -1) {
            dimensionSum += dimCheck;
        } else {
            printf("\nProblem: %s\n", complexToLiteral(fsi, true));
        }
    }
    matrix->columns = dimensionSum;

    for (int k = 0; k < fsi->simplexCount; ++k) {
        Matrix *BiInequelity = inquelityMatrixes[k];

        for (int i = 0; i < BiInequelity->columns; ++i) {
            int prevFill = 0;
            int dimensionFill = dimensionSum;

            for (int l = 0; l < prevFill; ++l) {
                addMElement(matrix, 0);
            }

            for (int j = 0; j < BiInequelity->rows; ++j) {
                addMElement(matrix, getMatrixElem(BiInequelity, i, j));
                dimensionFill--;
                prevFill++;
            }

            for (int r = 0; r < dimensionFill; ++r) {
                addMElement(matrix, 0);
            }
            matrix->rows++;
        }
    }

    printf("\n%s\n", matrixToLiteral(matrix));
    exit(-1);

    Complex *comp = literalToComplex("[[]]");

    Ncol = matrix->rows - 1;
    lp = make_lp(0, Ncol);
    if (lp == NULL)
        ret = 1; /* couldn't construct a new model... */

//    set_break_at_first(lp, TRUE);

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

        for (int i = 0; i < comp->simplexCount; ++i) {
            Simplex *simp = getSimpexAt(comp, i);

            for (int j = 0; j < simp->elementCount - 1; ++j) {
                SimplexElem elem = getElementAt(simp, j);

                colno[j] = j + 1;
                row[j] = elem;
            }

            /* add the row to lpsolve */
            if (!add_constraintex(lp, Ncol, row, colno, LE, getElementAt(simp, simp->elementIndex)))
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
        print_lp(lp);
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

    printf("\nRet: %d\n", ret);

    if (ret == 0) {
        /* a solution is calculated, now lets get some results */

        /* objective value */
        printf("\nObjective value: %f\n", get_objective(lp));

        /* variable values */
        get_variables(lp, row);
        for (int j = 0; j < Ncol; j++)
            printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);

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

    exit(-1);
}


int demo() {
    lprec *lp;
    int Ncol, *colno = NULL, j, ret = 0;
    REAL *row = NULL;

    /* We will build the model row by row
       So we start with creating a model with 0 rows and 2 columns */
    Ncol = 2; /* there are two variables in the model */
    lp = make_lp(0, Ncol);
    if (lp == NULL)
        ret = 1; /* couldn't construct a new model... */

    if (ret == 0) {
        /* let us name our variables. Not required, but can be useful for debugging */
        set_col_name(lp, 1, "x");
        set_col_name(lp, 2, "y");

        /* create space large enough for one row */
        colno = (int *) malloc(Ncol * sizeof(*colno));
        row = (REAL *) malloc(Ncol * sizeof(*row));
        if ((colno == NULL) || (row == NULL))
            ret = 2;
    }

    if (ret == 0) {
        set_add_rowmode(lp, TRUE);  /* makes building the model faster if it is done rows by row */

        /* construct first row (120 x + 210 y <= 15000) */
        j = 0;

        colno[j] = 1; /* first column */
        row[j++] = 120;

        colno[j] = 2; /* second column */
        row[j++] = 210;

        /* add the row to lpsolve */
        if (!add_constraintex(lp, j, row, colno, LE, 15000))
            ret = 3;
    }

    if (ret == 0) {
        /* construct second row (110 x + 30 y <= 4000) */
        j = 0;

        colno[j] = 1; /* first column */
        row[j++] = 110;

        colno[j] = 2; /* second column */
        row[j++] = 30;

        /* add the row to lpsolve */
        if (!add_constraintex(lp, j, row, colno, LE, 4000))
            ret = 3;
    }

    if (ret == 0) {
        /* construct third row (x + y <= 75) */
        j = 0;

        colno[j] = 1; /* first column */
        row[j++] = 1;

        colno[j] = 2; /* second column */
        row[j++] = 1;

        /* add the row to lpsolve */
        if (!add_constraintex(lp, j, row, colno, LE, 75))
            ret = 3;
    }

    if (ret == 0) {
        set_add_rowmode(lp, FALSE); /* rowmode should be turned off again when done building the model */

        /* set the objective function (143 x + 60 y) */
        j = 0;

        colno[j] = 1; /* first column */
        row[j++] = 143;

        colno[j] = 2; /* second column */
        row[j++] = 60;

        /* set the objective in lpsolve */
        if (!set_obj_fnex(lp, j, row, colno))
            ret = 4;
    }

    if (ret == 0) {
        /* set the object direction to maximize */
        set_maxim(lp);

        /* just out of curioucity, now show the model in lp format on screen */
        /* this only works if this is a console application. If not, use write_lp and a filename */
        write_LP(lp, stdout);
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

    if (ret == 0) {
        /* a solution is calculated, now lets get some results */

        /* objective value */
        printf("Objective value: %f\n", get_objective(lp));

        /* variable values */
        get_variables(lp, row);
        for (j = 0; j < Ncol; j++)
            printf("%s: %f\n", get_col_name(lp, j + 1), row[j]);

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

    return (ret);
}
