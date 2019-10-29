/* FILE variable.h
 *
 * PACAKGE ucli
 * MODULE variable
 *
 * BRIEF This utility is used to expand variables within a string.
 *
 * DETAIL
 * 
 */
#ifndef _ucli_variable_h
#define _ucli_variable_h

#include "libucli/shell.h"
#include "libucli/command.h"
#include "libucli/pargv.h"

char *                      ucli_variable_expand(
                              const char              *string,
                              const char              *viewid,
                              const ucli_command_t   *cmd,
                              ucli_pargv_t           *pargv);

#endif /* _ucli_variable_h */
/** @} ucli_variable */
