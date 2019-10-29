/*
 * param.h
 */
/**
\ingroup ucli
\defgroup ucli_param param
@{

\brief This class represents an instance of a parameter type.

Parameter instances are assocated with a command line and used to validate the
the arguments which a user is inputing for a command.

*/
#ifndef _ucli_param_h
#define _ucli_param_h

typedef struct ucli_param_s ucli_param_t;

#include "libucli/ptype.h"

/*=====================================
 * PARAM INTERFACE
 *===================================== */
/*-----------------
 * meta functions
 *----------------- */
ucli_param_t *
    ucli_param_new(const char    *name,
                    const char    *text,
                    ucli_ptype_t *ptype);
/*-----------------
 * methods
 *----------------- */
void
    ucli_param_delete(ucli_param_t *instance);
void
    ucli_param_help(const ucli_param_t *instance,
                     size_t               offset);
char * 
    ucli_param_validate(const ucli_param_t *instance,
                         const char          *text);
void
    ucli_param_dump(const ucli_param_t *instance);
/*-----------------
 * attributes
 *----------------- */
const char *
    ucli_param__get_name(const ucli_param_t *instance);
const char *
    ucli_param__get_text(const ucli_param_t *instance);
const char *
    ucli_param__get_range(const ucli_param_t *instance);
const char *
    ucli_param__get_prefix(const ucli_param_t *instance);
const char *
    ucli_param__get_default(const ucli_param_t *instance);
ucli_ptype_t *
    ucli_param__get_ptype(const ucli_param_t *instance);
void
    ucli_param__set_prefix(ucli_param_t *instance,
                            const char    *prefix);
void
    ucli_param__set_default(ucli_param_t *instance,
                             const char    *defval);

#endif /* _ucli_param_h */
/** @} ucli_param */
