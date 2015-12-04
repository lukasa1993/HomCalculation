//
// Created by lukasa on 12/4/15.
//

#include "ipsolve_bridge.h"

#include <stdio.h>
#include "lp_lib.h"

void solve_complex(Complex *A, Complex *B, Complex *fsi) {
    
    lprec *lp;
    int ret;
    /* Create a new LP model */
    lp = make_lp(0, 4);
    if (lp == NULL) {
        fprintf(stderr, "Unable to create new LP model\n");
    }
    set_verbose(lp, IMPORTANT);

    ret = solve(lp);

    printf("%d", ret);

    delete_lp(lp);
}