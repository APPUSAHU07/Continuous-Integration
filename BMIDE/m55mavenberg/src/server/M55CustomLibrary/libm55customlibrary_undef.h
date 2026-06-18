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

#if !defined(LIBM55CUSTOMLIBRARY) && !defined(IPLIB)
#   error IPLIB or LIBM55CUSTOMLIBRARY is not defined
#endif

#undef M55CUSTOMLIBRARY_API
#undef M55CUSTOMLIBRARYEXPORT
#undef M55CUSTOMLIBRARYGLOBAL
#undef M55CUSTOMLIBRARYPRIVATE
