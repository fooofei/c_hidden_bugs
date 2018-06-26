

#include <stdio.h>
#include <stdint.h>
#include <string.h> // memcmp

struct test
{
    uint32_t a;
    uint32_t b;
    uint16_t c;
    uint16_t d;
    uint8_t e;
};


int main()
{   
    printf("sizeof(struct test)=%zu \n", sizeof(struct test)); // 16
    // 证明这个结构体有 padding
    
    int i = 0;

    for (i = 0; i < 10; i += 1)
    {
        struct test a;  // 教训 一定记得在这里初始化
        struct test b;

        a.a = 1; a.b = 1; a.c = 1; a.d = 1; a.e = 1;
        b.a = 1; b.b = 1; b.c = 1; b.d = 1; b.e = 1;

        // 不一定相等，因为 e 字段后面的大小是填充的，未初始化的，是随机值
        printf("a==b? %s \n", (0 == memcmp(&a, &b, sizeof(a))) ? "true" : "false");
    }

    return 0;
}

/* msvc debug
sizeof(struct test)=16
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true
a==b? true

*/

/* msvc release 

sizeof(struct test)=16
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false
a==b? false

*/
