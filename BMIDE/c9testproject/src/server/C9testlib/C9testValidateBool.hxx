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
 *   This file contains the declaration for the Extension C9testValidateBool
 *
 */
 
#ifndef C9TESTVALIDATEBOOL_HXX
#define C9TESTVALIDATEBOOL_HXX
#include <tccore/method.h>
#include <C9testlib/libc9testlib_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern C9TESTLIB_API int C9testValidateBool(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <C9testlib/libc9testlib_undef.h>
                
#endif  // C9TESTVALIDATEBOOL_HXX
