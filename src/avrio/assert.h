/* Copyright (c) 2005,2007 Joerg Wunsch
   All rights reserved.

   Portions of documentation Copyright (c) 1991, 1993
   The Regents of the University of California.

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  $Id: assert.h,v 1.3 2007/01/23 15:32:48 joerg_wunsch Exp $
*/

#include <stdlib.h>

/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup assert_module Diagnostics
    \code #include <assert.h> \endcode

    This header file defines a debugging aid.

    As there is no standard error output stream available for many
    applications using this library, the generation of a printable
    error message is not enabled by default.  These messages will
    only be generated if the application defines the macro

    \code __ASSERT_USE_STDERR \endcode

    before including the \c <assert.h> header file.  By default,
    only abort() will be called to halt the application.
 *  @{
*/

#undef assert

#if defined(__DOXYGEN__)
/**
 * \def assert
 * \param expression Expression to test for.
 *
 * The assert() macro tests the given expression and if it is false,
 * the calling process is terminated.  A diagnostic message is written
 * to stderr and the function abort() is called, effectively
 * terminating the program.
 *
 * If expression is true, the assert() macro does nothing.
 *
 * The assert() macro may be removed at compile time by defining
 * NASSERT as a macro (e.g., by using the compiler option -DNASSERT).
 */
#define assert(expression)

/**
 *   @}
 * @}
 */

#else /* !DOXYGEN */

#ifndef ASSERT_TYPE_EQUAL
/** Ensure two variables have the same type. */
#define ASSERT_TYPE_EQUAL(var1, var2)  \
  do { (void)(&(var1) == &(var2)); } while(0)
#endif

#ifndef ASSERT_TYPE_IS
/** Ensure variable is of specified type. */
#define ASSERT_TYPE_IS(var, type)  \
  do { (void)(&(var) == (type *)0); } while(0)
#endif

#if defined(NASSERT)
#define assert(e)	((void)0)
#else /* !NASSERT */
#if defined(__ASSERT_USE_STDERR)
#define assert(e)	((e) ? (void)0 : \
                         __assert(__func__, __LINE__, #e))
#else /* !__ASSERT_USE_STDERR */
#define assert(e)	((e) ? (void)0 : abort())
#endif /* __ASSERT_USE_STDERR */
#endif /* NASSERT */
#endif /* DOXYGEN */

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__DOXYGEN__)

extern void __assert (const char *__func, int __lineno, const char *__sexp);

#endif /* not __DOXYGEN__ */

#ifdef __cplusplus
}
#endif

/*@}*/
/* EOF */
