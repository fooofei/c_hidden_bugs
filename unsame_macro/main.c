
/*
  this file shows an error use of macro define in header file

  macro _USE_DATA2 defined in mylib project which use sizeof(a_struct_t)=8,
  but _USE_DATA2 not use in main.c which use  sizeof(a_struct_t)=4.

  solutions:

  -> add macro _USE_DATA2 in main.c
  or -> add _USE_DATA2 in unsame_error project link flags (in makefile)

*/

#include <stdio.h>
#include <string.h>
#include "a.h"



int main()
{
    uint32_t t1 = 2; // or can access here
    a_struct_t a;
    uint32_t t2 = 3; 
    memset(&a, 0, sizeof(a));

    printf("sizeof(a) in main.c is %lu\n", sizeof(a));

    // compile error
    //printf("a.data2=%u\n", a.data2);

    /* data2 is an invalid data, and sometimes may cause crash. */
    print_a_struct(&a);

    return 0;
}
