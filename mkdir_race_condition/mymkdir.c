
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>

#include "mymkdir.h"

// open 2 process to mkdir,
// use 1 process to rmdir,
// the mkdir will occur a race condition

int mymkdir(const char * path)
{
    int ret;

    ret = access(path, F_OK);
    if (ret ==0)
    {
        return 0;
    }
 
    // DEFFILEMODE: Equivalent of 0666 = rw-rw-rw-
    // use this mode created directory, this user cannot use `cd`,
    //  the mode is drw-r--r-- 

    // ACCESSPERMS: Equivalent of 0777 = rwxrwxrwx 
    ret = mkdir(path, ACCESSPERMS);
    if (ret == -1 && errno == EEXIST) 
    {
        struct stat st;
        if (0 == stat(path, &st) && S_ISDIR(st.st_mode)) {
            ret = 0;
        }
    }

    return ret;
}

// https://wiki.sei.cmu.edu/confluence/display/c/FIO45-C.+Avoid+TOCTOU+race+conditions+while+accessing+files
// http://seclab.cs.ucdavis.edu/projects/vulnerabilities/doves/2.html
// https://rocfang.gitbooks.io/dev-notes/content/oexcl_de_zuo_yong.html
// https://stackoverflow.com/questions/24712383/how-to-avoid-race-condition-when-checking-if-file-exists-and-then-creating-it
// https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
// https://stackoverflow.com/questions/37931151/race-condition-with-stat-and-mkdir-in-sequence
// https://stackoverflow.com/questions/6314042/stat-vs-mkdir-with-eexist?noredirect=1&lq=1
// 
int mymkdir2(const char * path)
{
    int fd;

    // create file
    fd = open(path, O_CREAT | O_EXCL | O_RDWR);
    if (fd<0)
    {
        // If file/directory exists, return -1
        if (errno == EEXIST) {
            return 0;
        }
        return -1;
    }
    else
    {
        close(fd);
        return 0;
    }
}
