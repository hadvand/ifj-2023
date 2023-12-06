/**
 * @file main.c
 * @author Nelia Mashlai (xmashl00)
 * @author Nikita Vetluzhskikh (xvetlu00)
 * @author Oleg Borsh (xborsh00)
 * @author Murad Mikogaziev (xmikog00)
 * @brief main brain
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int main()
{
    int ret_code;
    if ((ret_code = analyse())) {
        return ret_code;
    }
    else {
#ifdef PARS_DEBUG
        fprintf(stderr, "\nERROR code %d\n", ret_code);
#else
        fprintf(stderr, "%d\n", ret_code);
#endif
    }

    return ER_NONE;
}
