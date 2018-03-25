
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "header.h"

#ifdef __cplusplus
extern "C"
#endif
const char * return_from_lib()
{
    fprintf(stdout, "-> printf from %s()\n", __FUNCTION__);
    fflush(stdout);
    error_inline();
    return __FUNCTION__;
}


inline void error_inline()
{
    fprintf(stdout, "-> fprintf from %s()\n", __FUNCTION__);
    fflush(stdout);
}
