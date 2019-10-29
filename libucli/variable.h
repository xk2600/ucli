/*
 * variable.h
 */
 /**
\ingroup ucli
\defgroup ucli_variable variable
@{

\brief This utility is used to expand variables within a string.

*/
#ifndef _ucli_variable_h
#define _ucli_variable_h

#include "shell.h"
#include "command.h"
#include "pargv.h"

/*=====================================
 * VARIABLE INTERFACE
 *===================================== */
/*-----------------
 * meta functions
 *----------------- */
char *
		ucli_variable_expand(const char            *string,
               	              const char            *viewid,
               	              const ucli_command_t *cmd,
                   		      ucli_pargv_t         *pargv);
/*-----------------
 * methods
 *----------------- */

/*-----------------
 * attributes
 *----------------- */

#endif /* _ucli_variable_h */
/** @} ucli_variable */
