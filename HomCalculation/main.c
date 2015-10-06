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
void parse_input(char *filename);

int main(int argc, char *argv[]) {
    clock_t begin, end;
    double time_spent;

    begin = clock();


    parse_input("A.txt");
    parse_input("B.txt");

    end = clock();
    time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf("\n %f The End.. \n", time_spent);

    return 0;
}

char *file_to_string(char *filename) {
    FILE *f = fopen(filename, "rb");
    if (f == NULL) {
        perror(sprintf("Error while opening %s. \n", filename));
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

void parse_input(char *filename) {
    char *file_content = file_to_string(filename);
    size_t file_length = strlen(file_content);

    Complex *Comp = NULL;
    char Comp_Literal[1000];
    int Comp_Literal_i = 0;

    int Comp_Sim_i = 0;

    char Comp_Sub_Literal[1000];
    int Comp_Sub_Literal_i = 0;

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
            }



            if (Comp_Literal_i > 0) {
                Comp_Literal[Comp_Literal_i++] = aChar;
            } else if (Comp_Sub_Literal_i > 0) {
                Comp_Sub_Literal[Comp_Sub_Literal_i++] = aChar;
            } else if(isdigit(aChar)) {
                Simplex *simplex = getSimpexAt(Comp, Comp_Sim_i);
                // aChar is matrix value of simplex->allowedSubSimplexes->simplexIndex

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
                addSimplex(simplex->allowedSubSimplexes, literalToSimplex(Comp_Sub_Literal));
            }
        }
    }

    printf("%i\n", line_count);
}