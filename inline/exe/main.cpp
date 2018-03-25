

#include <stdio.h>

#include "../lib/header.h"



#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define WINAPI __stdcall

#define RTLD_LAZY 1
#define RTLD_NOW 2
#define RTLD_GLOBAL 4
#define RTLD_LOCAL 8


// only support ansi char, not utf-8
static void *dlopen(const char *name, int mode)
{
    if (name)
    {
        return (void *)LoadLibraryA(name);
    }
    return NULL;
}

static int dlclose(void *handle)
{
    return FreeLibrary((HMODULE)handle) ? 0 : -1;
}

static FARPROC dlsym(void *handle, const char *symbol)
{
    return GetProcAddress((HMODULE)handle, symbol);
}

#else
#include <dlfcn.h>
#define WINAPI
#endif // WIN32



int main()
{
    const char * lib_name;

    void * lib_handle = 0;

#ifdef WIN32
    lib_name = "./build_lib/Debug/inline_test.dll";
#else
    lib_name = "./build_lib/libinline_test.so";
#endif

    lib_handle = dlopen(lib_name, RTLD_NOW);

    if (!lib_handle)
    {
        fprintf(stderr, "fail dlopen %s\n", lib_name);
        return -1;
    }

    typedef const char * (*pfn_return_from_lib)();
    pfn_return_from_lib fn = (pfn_return_from_lib)dlsym(lib_handle, "return_from_lib");
    if (!fn)
    {
        fprintf(stderr, "fail dlsym\n");
    }
    if (fn)
    {
        const char * v = fn();
        fprintf(stdout, "-> printf from main exe %s\n", v);
    }
    
    dlclose(lib_handle); lib_handle = 0;


    return 0;
}
