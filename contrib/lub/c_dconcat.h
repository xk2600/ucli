/* 
 * @file c_dconcat.h 
 * 
 * a set of macros to enable concatenation of arguments with a delimter.
 */

/**
 * \ingroup lub
 * \defgroup lub_c_dconcat Delimited Concatenation
 * @{
 * This set of macros enables more complex macro generation, by providing
 * preprocessing function-type macros for concatenation of arguments with
 * a delimter.
 *
 * \b Example:
 * 
 * \code
 * #include "lub/c_dconcat.h"
 *
 * #define CmdFactory(...)              \
 *    static int                        \
 *    _DCONCAT(_,TclCmd_,__VA_ARGS__)(  \
 *      ClientData    dummy,            \
 *      Tcl_Interp *  interp,           \
 *      int           objc,             \
 *      Tcl_Obj * const objv[] )        \
 *
 * CmdFactory(Ball,Bounce) {
 *    // do some work...
 * }
 *
 * \endcode
 *
 * returns the following snippet
 *
 * \code
 * #include "lub/c_dconcat.h"
 *
 * static int
 * TclCmd__Ball_Bounce(ClientData dummy, Tcl_interp * interp, int objc, Tcl_Obj * const objv[] ) {
 *    // do some work...
 * }
 */

/**
 * \def _GET_ARGC(...)
 * \return a count of the arguments supplied.
 */

/**
 * \def _GET_DCONCAT_FN(...)
 * \return specific \c _DCONCAT<#> function-type macro name.
 */

/**
 * \def _DCONCAT(DELIM, ...)
 * \return concatenated arguments with \c DELIM as the delimeter between them.
 */

#ifndef _lub_c_dconcat_h
#define _lub_c_dconcat_h

#  define _ARGC(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#  define _GET_DCONCAT_FN(...) _ARGC(__VA_ARGS__,_DCONCAT8, _DCONCAT7, _DCONCAT6, _DCONCAT5, _DCONCAT4, _DCONCAT3, _DCONCAT2)

#  define _DCONCAT2(D, a, b)                   a ## D ## b 
#  define _DCONCAT3(D, a, b, c)                a ## D ## b ## D ## c
#  define _DCONCAT4(D, a, b, c, d)             a ## D ## b ## D ## c ## D ## d 
#  define _DCONCAT5(D, a, b, c, d, e)          a ## D ## b ## D ## c ## D ## d ## D ## e 
#  define _DCONCAT6(D, a, b, c, d, e, f)       a ## D ## b ## D ## c ## D ## d ## D ## e ## D ## f
#  define _DCONCAT7(D, a, b, c, d, e, f, g)    a ## D ## b ## D ## c ## D ## d ## D ## e ## D ## f ## D ## g
#  define _DCONCAT8(D, a, b, c, d, e, f, g, h) a ## D ## b ## D ## c ## D ## d ## D ## e ## D ## f ## D ## g ## D ## h

#  define _DCONCAT(DELIM, ...) _GET_DCONCAT_FN(__VA_ARGS__)(DELIM,__VA_ARGS__)

#endif
