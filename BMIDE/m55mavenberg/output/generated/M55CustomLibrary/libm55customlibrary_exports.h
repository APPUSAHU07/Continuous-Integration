//@<COPYRIGHT>@
//==================================================
//Copyright $2026.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/** 
    @file 

    This file contains the declaration for the Dispatch Library  M55CustomLibrary

*/

#include <common/library_indicators.h>

#ifdef EXPORTLIBRARY
#define EXPORTLIBRARY something else
#error ExportLibrary was already defined
#endif

#define EXPORTLIBRARY            libM55CustomLibrary

#if !defined(LIBM55CUSTOMLIBRARY) && !defined(IPLIB)
#   error IPLIB or LIBM55CUSTOMLIBRARY is not defined
#endif

/* Handwritten code should use M55CUSTOMLIBRARY_API, not M55CUSTOMLIBRARYEXPORT */

#define M55CUSTOMLIBRARY_API M55CUSTOMLIBRARYEXPORT

#if IPLIB==libM55CustomLibrary || defined(DEFINE_LIBM55CUSTOMLIBRARY_EXPORTS)
#   if defined(__lint)
#       define M55CUSTOMLIBRARYEXPORT       __export(M55CustomLibrary)
#       define M55CUSTOMLIBRARYGLOBAL       extern __global(M55CustomLibrary)
#       define M55CUSTOMLIBRARYPRIVATE      extern __private(M55CustomLibrary)
#   elif defined(_WIN32)
#       define M55CUSTOMLIBRARYEXPORT       __declspec(dllexport)
#       define M55CUSTOMLIBRARYGLOBAL       extern __declspec(dllexport)
#       define M55CUSTOMLIBRARYPRIVATE      extern
#   else
#       define M55CUSTOMLIBRARYEXPORT
#       define M55CUSTOMLIBRARYGLOBAL       extern
#       define M55CUSTOMLIBRARYPRIVATE      extern
#   endif
#else
#   if defined(__lint)
#       define M55CUSTOMLIBRARYEXPORT       __export(M55CustomLibrary)
#       define M55CUSTOMLIBRARYGLOBAL       extern __global(M55CustomLibrary)
#   elif defined(_WIN32) && !defined(WNT_STATIC_LINK)
#       define M55CUSTOMLIBRARYEXPORT      __declspec(dllimport)
#       define M55CUSTOMLIBRARYGLOBAL       extern __declspec(dllimport)
#   else
#       define M55CUSTOMLIBRARYEXPORT
#       define M55CUSTOMLIBRARYGLOBAL       extern
#   endif
#endif
