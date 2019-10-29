/* FILE pargv.h
 * 
 * PACKAGE ucli
 * MODULE pargv
 *
 * BRIEF This represents a vector of command line arguments.
 *
 */

#ifndef _ucli_pargv_h
#define _ucli_pargv_h

typedef struct ucli_pargv_s ucli_pargv_t;
typedef struct ucli_parg_s  ucli_parg_t;

#include "libucli/ptype.h"
#include "libucli/command.h"



/* LINE EVALUATION RESULT */
typedef enum {
    ucli_LINE_OK, 
    ucli_BAD_CMD, 
    ucli_BAD_PARAM,
    ucli_BAD_HISTORY
} ucli_pargv_status_t;



/******************************************************************************/
/* PARGV PUBLIC INTERFACE -- (implemented in pargv.c)
 *
 * REFERENCE
 *
 *       ## ARGUMENT REPRESENTATION ########
 *
 *       struct ucli_parg_s { 
 *           const ucli_param_t *param;
 *           char                *value;
 *       };
 *
 *       ## ARRAY OF ARGUMENTS AND COUNT ###
 *
 *       struct ucli_pargv_s {
 *           unsigned     pargc;
 *           ucli_parg_t pargs[1];
 *       };
 *
 *
 * ELEMENTS
 *
 *       param          parameter attributes (name and type)
 *       value          parameter binary value
 *
 *       pargc          count of arguments
 *       pargs          array of arguments*
 *
 * METHODS
 * 
 *       ucli_pargv_new -       create an argument array instance
 *       ucli_pargv_delete -    delete an argument array instance
 *       ucli_pargv_find_arg -  find an argument in an argument array instance
 *       ucli_pargv_dump -      print argument array to stdout
 *       ucli_parg_dump -       print argument to stdout
 *
 * ATTRIBUTES
 *
 *       ucli_parg__get_name -  get the parameter argument name for an argument instance
 *       ucli_parg__get_value - get the value for a paramter argument instance
 *       ucli_parg__get_ptype - get the parameter type for an argument instance
 *
 * ARGUMENTS
 *
 *       cmd -          command this parameter is associated with
 *       line -         ???????
 *       offset -       ???????
 *       status -       ???????
 *       instance -     instance of either argument array (pargv) or  *                               
 *
 */

    /** METHODS ***************************************************************/
    ucli_pargv_t *         ucli_pargv_new(
                              const ucli_command_t *cmd,
                              const char            *line,
                              size_t                 offset,
                              ucli_pargv_status_t  *status);

    void                    ucli_pargv_delete(
                              ucli_pargv_t         *instance);

    const ucli_parg_t *    ucli_pargv_find_arg(
                              ucli_pargv_t         *instance, 
                              const char            *name);

    void                    ucli_pargv_dump(
                              const ucli_pargv_t   *instance);

    void                    ucli_parg_dump(
                              const ucli_parg_t    *instance);
    
    /** ATTRIBUTES ************************************************************/
    const char *            ucli_parg__get_name(
                              const ucli_parg_t    *instance);

    const char *            ucli_parg__get_value(
                              const ucli_parg_t    *instance);

    const ucli_ptype_t *   ucli_parg__get_ptype(
                              const ucli_parg_t    *instance);

/** END PARG INTERFACE ********************************************************/







#endif /* _ucli_pargv_h */
