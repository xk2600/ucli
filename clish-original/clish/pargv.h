/*
 * pargv.h
 */
 /**
\ingroup ucli
\defgroup ucli_pargv pargv
@{

\brief This class represents a vector of command line arguments.

*/

#ifndef _ucli_pargv_h
#define _ucli_pargv_h

typedef enum 
{
	ucli_LINE_OK, 
	ucli_BAD_CMD, 
	ucli_BAD_PARAM,
	ucli_BAD_HISTORY
} ucli_pargv_status_t;

typedef struct ucli_pargv_s ucli_pargv_t;
typedef struct ucli_parg_s  ucli_parg_t;

#include "libucli/ptype.h"
#include "libucli/command.h"

/*=====================================
 * PARGV INTERFACE
 *===================================== */
/*-----------------
 * meta functions
 *----------------- */
ucli_pargv_t *
		ucli_pargv_new(const ucli_command_t *cmd,
		                const char            *line,
		                size_t                 offset,
		                ucli_pargv_status_t  *status);
/*-----------------
 * methods
 *----------------- */
void
		ucli_pargv_delete(ucli_pargv_t *instance);
const ucli_parg_t *
		ucli_pargv_find_arg(ucli_pargv_t *instance,
 				     const char    *name);
void
		ucli_pargv_dump(const ucli_pargv_t *instance);
/*-----------------
 * attributes 
 *----------------- */

/*=====================================
 * PARG INTERFACE
 *===================================== */
/*-----------------
 * meta functions
 *----------------- */
/*-----------------
 * methods
 *----------------- */
void
		ucli_parg_dump(const ucli_parg_t *instance);
/*-----------------
 * attributes 
 *----------------- */
const char *
		ucli_parg__get_name(const ucli_parg_t *instance);
const char *
		ucli_parg__get_value(const ucli_parg_t *instance);
const ucli_ptype_t *
		ucli_parg__get_ptype(const ucli_parg_t *instance);


#endif /* _ucli_pargv_h */
/** @} ucli_pargv */
