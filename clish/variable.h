/* FILE variable.h
 *
 * PACAKGE clish
 * MODULE variable
 *
 * BRIEF This utility is used to expand variables within a string.
 *
 * DETAIL
 * 
 */
#ifndef _clish_variable_h
#define _clish_variable_h

#include "clish/shell.h"
#include "clish/command.h"
#include "clish/pargv.h"

char *                      clish_variable_expand(
                              const char              *string,
                              const char              *viewid,
                              const clish_command_t   *cmd,
                              clish_pargv_t           *pargv);

#endif /* _clish_variable_h */
/** @} clish_variable */
