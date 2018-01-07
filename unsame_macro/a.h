
#ifndef A_H_
#define A_H_

#include <stdint.h>


struct a_struct_t
{
    uint32_t data1;
    
#ifdef _USE_DATA2
    uint32_t data2;
#endif
};

typedef struct a_struct_t a_struct_t;


void print_a_struct(const a_struct_t *);

#endif //A_H_
