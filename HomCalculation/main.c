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
Complex *parse_input(char *filename);

int main(int argc, char *argv[]) {
    clock_t begin, end;
    double time_spent;

    begin = clock();


    Complex *A = parse_input("A.txt");
    Complex *B = parse_input("B.txt");


    Calculate_Hom(A, B);

    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("\n %f The End.. \n", time_spent);

    return 0;
}

char *file_to_string(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        char error[255];
        sprintf(error, "Error while opening %s. \n", filename);
        perror(error);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = (size_t) ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;

    return string;
}


Complex *parse_input(char *filename) {

    char *file_content = file_to_string(filename);
    size_t file_length = strlen(file_content);

    Complex *Comp = NULL;
    char Comp_Literal[1000];
    int Comp_Literal_i = 0;

    int Comp_Sim_i = 0;
    int Comp_Sub_Sim_i = 0;

    char Comp_Sub_Literal[1000];
    int Comp_Sub_Literal_i = 0;


    char Matrix_Literal[1000];
    int Matrix_Literal_i = 0;


    Matrix *matrix = NULL;
    int matrixCount = 0;

    char Coordinates_literal[1000];
    int Coordinates_literal_I = 0;

    memset(Matrix_Literal, 0, 1000);
    memset(Coordinates_literal, 0, 1000);
    memset(Comp_Sub_Literal, 0, 1000);
    memset(Comp_Literal, 0, 1000);

    Matrix *coordinates = NULL;

    int line_count = 0;
    bool line_skip = false;

    for (int i = 0; i < file_length; i++) {
        char aChar = file_content[i];
        if (aChar == '#') {
            line_skip = true;
        }

        if (!line_skip) {

            if (aChar == '[') {
                if (Comp == NULL) {
                    Comp_Literal[Comp_Literal_i++] = aChar;
                } else {
                    Comp_Sub_Literal[Comp_Sub_Literal_i++] = aChar;
                }
            } else if (Comp_Literal_i > 0) {
                Comp_Literal[Comp_Literal_i++] = aChar;
            } else if (Comp_Sub_Literal_i > 0) {
                Comp_Sub_Literal[Comp_Sub_Literal_i++] = aChar;
            } else if (isdigit(aChar) || aChar == '.' || aChar == '-') {
                Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);

                if (matrixCount > 0 && matrixCount == simplex->allowedSubSimplexes->simplexCount) {
                    Coordinates_literal[Coordinates_literal_I++] = aChar;
                } else {
                    Matrix_Literal[Matrix_Literal_i++] = aChar;
                }
            }

            if (aChar == ' ') {
                if (Matrix_Literal_i > 0) {
                    if (matrix == NULL) {
                        matrix = Init_Matrix();
                    }

                    if (matrix->columns == -1) {
                        matrix->columns = atoi(Matrix_Literal);

                        memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                        Matrix_Literal_i = 0;
                    } else if (matrix->rows == -1) {
                        matrix->rows = atoi(Matrix_Literal);

                        memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                        Matrix_Literal_i = 0;
                    } else {
                        addMElement(matrix, atof(Matrix_Literal));
                        memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                        Matrix_Literal_i = 0;
                    }
                }

                if (Coordinates_literal_I > 0) {
                    Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);
                    if (simplex->dimension) {
                        if (coordinates == NULL) {

                            coordinates = Init_Matrix();
                            coordinates->rows = simplex->dimension;
                            coordinates->columns = simplex->elementCount;
                        }

                        addMElement(coordinates, atof(Coordinates_literal));

                        memset(Coordinates_literal, 0, Coordinates_literal_I + 10);
                        Coordinates_literal_I = 0;
                    }
                }
            }
        }


        if (aChar == '\n') {
            line_count++;
            line_skip = false;

            if (Comp_Literal_i > 0 && Comp == NULL) {
                Comp = literalToComplex(Comp_Literal);
                Comp_Literal_i = 0;
            } else if (Comp_Sub_Literal_i > 0) {
                Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);

                if (simplex->allowedSubSimplexes == NULL) {
                    simplex->allowedSubSimplexes = Init_Complex();
                }
                Simplex *tmpSmp = literalToSimplex(Comp_Sub_Literal);
                addSimplex(simplex->allowedSubSimplexes, tmpSmp);
                memset(Comp_Sub_Literal, 0, Comp_Sub_Literal_i + 10);
                Comp_Sub_Literal_i = 0;
            } else if (Matrix_Literal_i > 0) {
                if (matrix->columns == -1) {
                    matrix->columns = atoi(Matrix_Literal);

                    memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                    Matrix_Literal_i = 0;
                } else if (matrix->rows == -1) {
                    matrix->rows = atoi(Matrix_Literal);

                    memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                    Matrix_Literal_i = 0;
                } else {
                    addMElement(matrix, atof(Matrix_Literal));
                    memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                    Matrix_Literal_i = 0;
                    if (matrix->rows * matrix->columns == matrix->mCount) {
                        matrixCount++;

                        Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);
                        Simplex *subSimp = getSimpexAt(simplex->allowedSubSimplexes, Comp_Sub_Sim_i);
                        subSimp->inequalityMatrix = matrix;

                        matrix = NULL;
                        memset(Matrix_Literal, 0, Matrix_Literal_i + 10);
                        Matrix_Literal_i = 0;
                        Comp_Sub_Sim_i++;
                    }
                }
            } else if (Coordinates_literal_I > 0) {
                Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);

                if (simplex->dimension == -1) {
                    simplex->dimension = atoi(Coordinates_literal);
                } else {
                    addMElement(coordinates, atof(Coordinates_literal));
                    if (coordinates->rows * coordinates->columns == coordinates->mCount) {
                        simplex->coodinates = coordinates;
                        coordinates = NULL;
                        Comp_Sim_i++;
                        matrixCount = 0;
                        Comp_Sub_Sim_i = 0;
                    }
                }

                memset(Coordinates_literal, 0, Coordinates_literal_I + 10);
                Coordinates_literal_I = 0;
            }
        }
    }


    // testing what we read

//    printf("%s\n", complexToLiteral(Comp, true));
//    for (int a1 = 0; a1 < Comp->simplexCount; a1++) {
//        Simplex *simp = getSimpexAt(Comp, a1);
//
//        for (int a2 = 0; a2 < simp->allowedSubSimplexes->simplexCount; a2++) {
//            Simplex *subSimp = getSimpexAt(simp->allowedSubSimplexes, a2);
//
//            printf("%s\n", simplexToLiteral(subSimp));
//            printf("%s\n", matrixToLiteral(subSimp->inequalityMatrix));
//        }
//        printf("%d\n", simp->dimension);
//        printf("%s\n", matrixToLiteral(simp->coodinates));
//
//        printf("\n ---- Sub Simp END ----\n");
//    }

    return Comp;
}


