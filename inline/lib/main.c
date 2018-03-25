
#include <stdio.h>

#include "header.h"


int main()
{
    const char * v;

    v = return_from_lib();

    printf("-> printf from main %s\n", v);

    return 0;
}
