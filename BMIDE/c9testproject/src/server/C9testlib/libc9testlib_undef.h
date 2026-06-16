//@<COPYRIGHT>@
//==================================================
//Copyright $2026.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@


#include <common/library_indicators.h>

#if !defined(EXPORTLIBRARY)
#   error EXPORTLIBRARY is not defined
#endif

#undef EXPORTLIBRARY

#if !defined(LIBC9TESTLIB) && !defined(IPLIB)
#   error IPLIB or LIBC9TESTLIB is not defined
#endif

#undef C9TESTLIB_API
#undef C9TESTLIBEXPORT
#undef C9TESTLIBGLOBAL
#undef C9TESTLIBPRIVATE
