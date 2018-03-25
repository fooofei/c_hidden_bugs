


#ifdef WIN32
#include <windows.h>
#define WINAPI __stdcall
#define ABI_CALL  __cdecl
#define API_EXPORT __declspec(dllexport)
#else
#define WINAPI
#define ABI_CALL   
#define API_EXPORT  __attribute__ ((visibility("default")))
#endif


/*
- `inline` 关键字使用一定要严格的规范，不能在 `.h` 函数声明时 `inline` ，函数实现放到 `.cpp` ，这样不行，在linux上编译后，无法``loadlibrary``


c 语言 Windows linux 都 pass

 cpp 也 pass

 $ gcc -v
 Using built-in specs.
 Target: x86_64-suse-linux
 Configured with: ../configure --prefix=/usr --infodir=/usr/share/info --mandir=/usr/share/man --libdir=/usr/lib64 --libexecdir=/usr/lib64 --enable-languages=c,c++,objc,fortran,obj-c++,java,ada --enable-checking=release --with-gxx-include-dir=/usr/include/c++/4.3 --enable-ssp --disable-libssp --with-bugurl=http://bugs.opensuse.org/ --with-pkgversion='SUSE Linux' --disable-libgcj --disable-libmudflap --with-slibdir=/lib64 --with-system-zlib --enable-__cxa_atexit --enable-libstdcxx-allocator=new --disable-libstdcxx-pch --enable-version-specific-runtime-libs --program-suffix=-4.3 --enable-linux-futex --without-system-libunwind --with-cpu=generic --build=x86_64-suse-linux
 Thread model: posix
 gcc version 4.3.4 [gcc-4_3-branch revision 152973] (SUSE Linux)


留待验证 ？ 是  CentOS 5 不行吗？

*/


// 在声明里加就行  实现没加也 ok
#ifdef __cplusplus
extern "C"
#endif
API_EXPORT const char * return_from_lib();

inline void error_inline();
