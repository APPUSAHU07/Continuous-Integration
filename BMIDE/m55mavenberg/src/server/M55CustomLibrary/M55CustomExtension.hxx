//@<COPYRIGHT>@
//==================================================
//Copyright $2026.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the declaration for the Extension M55CustomExtension
 *
 */
 
#ifndef M55CUSTOMEXTENSION_HXX
#define M55CUSTOMEXTENSION_HXX
#include <tccore/method.h>
#include <M55CustomLibrary/libm55customlibrary_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern M55CUSTOMLIBRARY_API int M55CustomExtension(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <M55CustomLibrary/libm55customlibrary_undef.h>
                
#endif  // M55CUSTOMEXTENSION_HXX
