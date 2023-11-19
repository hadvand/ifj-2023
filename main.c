#include "parser/parser.h"
#include "structures/error.h"
#include <stdio.h>

int main()
{
    int ret_code;
    if ((ret_code = analyse())) {
        return ret_code;
    }
    else {
        fprintf(stderr, "\nERROR code %d\n", ret_code);
    }

    return ER_NONE;
}
