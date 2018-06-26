

#include <map>
#include <stdio.h>

static int enable_memory_leak_detect()
{
    int flag_bk;
    int flag;

    flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    flag_bk = flag;
    flag |= _CRTDBG_LEAK_CHECK_DF;
    flag |= _CRTDBG_ALLOC_MEM_DF;
    _CrtSetDbgFlag(flag);

    //_CrtSetBreakAlloc(162);
    // called or not called all the same
    //_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    // 当从 vs 调试运行时，信息还是输出在 vs 里，其他则输出到 dbgview

    return flag_bk;
}

static void detect_memory_leak_at_end()
{
    int leak = _CrtDumpMemoryLeaks();
    if (leak != 0)
    {
#ifdef _ATL
        MessageBox(NULL, TEXT("Memory Leak"), TEXT("MemCheck"), 0);
#else 
        const char * pv = "\n\nMemory Leak\n";
        fprintf(stderr, pv);
#endif
    }
}

struct leak
{
    leak()
    {
        enable_memory_leak_detect();
    }
    ~leak()
    {
        detect_memory_leak_at_end();
    }
}g_leak;


// 在 insert 之前一定要 find
// 因为插入已经存在的值会导致插入失败 如果不处理这个返回值就会内存泄漏的可能（当值是new\malloc 出的）
void test_get_memory_leak()
{
    typedef std::map<int, int *> mytype_t;
    mytype_t c;

    std::pair<mytype_t::const_iterator,bool> rc;

    rc = c.insert(std::make_pair(0, (int *)malloc(4)));
    fprintf(stdout, "first insert return %d\n", rc.second);
    rc = c.insert(std::make_pair(0, (int *)malloc(4)));
    fprintf(stdout, "second insert return %d\n", rc.second);


    mytype_t::const_iterator it;
    for (it = c.begin(); it != c.end(); it++)
    {
        free(it->second);
    }
    c.clear();

}


void test()
{
    typedef std::map<int, int *> mytype_t;
    mytype_t c;
    mytype_t::const_iterator it;

    c.insert(std::make_pair(0, (int *)malloc(4)));
    it = c.find(0);
    if (it != c.end())
    {
        free(it->second);
        c.erase(it);
    }
    c.insert(std::make_pair(0, (int *)malloc(4)));
    
    for (it=c.begin(); it!=c.end(); it ++ )
    {
        free(it->second);
    }
    c.clear();

}


void test_detect_leak()
{
    enable_memory_leak_detect();
    detect_memory_leak_at_end();
}


int main()
{
    //test_detect_leak();
    //test();
    test_get_memory_leak();

    return 0;
}
