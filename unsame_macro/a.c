
#include <stdio.h>


#include "a.h"


void 
print_a_struct(const a_struct_t * p)
{
    printf(" sizeof(a_struct_t) in a.c =%lu\n  a_struct_t->data1=%u\n  a_struct_t->data2=%u\n"
        , sizeof(a_struct_t)
        , p->data1
        , p->data2
    );

    fflush(stdout);
}
