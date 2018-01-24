

#include <stdio.h>
#include <string.h>

#include "mymkdir.h"

void 
test_mkdir()
{
    int ret;
    ret = mymkdir("./dir_test_mkdir");

    printf("%s() return %d\n", __FUNCTION__, ret);
    fflush(stdout);
}

void
test_mkdir_race()
{
    int ret;
    int oldret=0;
    for (;;)
    {
        ret = mymkdir("./dir_test_mkdir");
        if (ret != oldret) {
            printf("%s() return %d\n",__FUNCTION__,ret);
            fflush(stdout);
            oldret = ret;
        }
        // seconds
        sleep(0);
    }
}

int main()
{
    test_mkdir_race();
    return 0;
}
