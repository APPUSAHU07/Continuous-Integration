//@<COPYRIGHT>@
//==================================================
//Copyright $2026.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/** 
    @file 

    This file contains the declaration for the Dispatch Library  C9testlib

*/

#include <common/library_indicators.h>

#ifdef EXPORTLIBRARY
#define EXPORTLIBRARY something else
#error ExportLibrary was already defined
#endif

#define EXPORTLIBRARY            libC9testlib

#if !defined(LIBC9TESTLIB) && !defined(IPLIB)
#   error IPLIB or LIBC9TESTLIB is not defined
#endif

/* Handwritten code should use C9TESTLIB_API, not C9TESTLIBEXPORT */

#define C9TESTLIB_API C9TESTLIBEXPORT

#if IPLIB==libC9testlib || defined(DEFINE_LIBC9TESTLIB_EXPORTS)
#   if defined(__lint)
#       define C9TESTLIBEXPORT       __export(C9testlib)
#       define C9TESTLIBGLOBAL       extern __global(C9testlib)
#       define C9TESTLIBPRIVATE      extern __private(C9testlib)
#   elif defined(_WIN32)
#       define C9TESTLIBEXPORT       __declspec(dllexport)
#       define C9TESTLIBGLOBAL       extern __declspec(dllexport)
#       define C9TESTLIBPRIVATE      extern
#   else
#       define C9TESTLIBEXPORT
#       define C9TESTLIBGLOBAL       extern
#       define C9TESTLIBPRIVATE      extern
#   endif
#else
#   if defined(__lint)
#       define C9TESTLIBEXPORT       __export(C9testlib)
#       define C9TESTLIBGLOBAL       extern __global(C9testlib)
#   elif defined(_WIN32) && !defined(WNT_STATIC_LINK)
#       define C9TESTLIBEXPORT      __declspec(dllimport)
#       define C9TESTLIBGLOBAL       extern __declspec(dllimport)
#   else
#       define C9TESTLIBEXPORT
#       define C9TESTLIBGLOBAL       extern
#   endif
#endif
