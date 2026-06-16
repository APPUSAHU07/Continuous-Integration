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
 *   This file contains the declaration for the Extension C9testregisterhandler
 *
 */
 
#ifndef C9TESTREGISTERHANDLER_HXX
#define C9TESTREGISTERHANDLER_HXX
#include <tccore/method.h>
#include <C9testlib/libc9testlib_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern C9TESTLIB_API int C9testregisterhandler(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <C9testlib/libc9testlib_undef.h>
                
#endif  // C9TESTREGISTERHANDLER_HXX
