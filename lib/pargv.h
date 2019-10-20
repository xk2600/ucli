/* FILE pargv.h
 * 
 * PACKAGE clish
 * MODULE pargv
 *
 * BRIEF This represents a vector of command line arguments.
 *
 */

#ifndef _clish_pargv_h
#define _clish_pargv_h

typedef struct clish_pargv_s clish_pargv_t;
typedef struct clish_parg_s  clish_parg_t;

#include "lib/ptype.h"
#include "lib/command.h"



/* LINE EVALUATION RESULT */
typedef enum {
    clish_LINE_OK, 
    clish_BAD_CMD, 
    clish_BAD_PARAM,
    clish_BAD_HISTORY
} clish_pargv_status_t;



/******************************************************************************/
/* PARGV PUBLIC INTERFACE -- (implemented in pargv.c)
 *
 * REFERENCE
 *
 *       ## ARGUMENT REPRESENTATION ########
 *
 *       struct clish_parg_s { 
 *           const clish_param_t *param;
 *           char                *value;
 *       };
 *
 *       ## ARRAY OF ARGUMENTS AND COUNT ###
 *
 *       struct clish_pargv_s {
 *           unsigned     pargc;
 *           clish_parg_t pargs[1];
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
 *       clish_pargv_new -       create an argument array instance
 *       clish_pargv_delete -    delete an argument array instance
 *       clish_pargv_find_arg -  find an argument in an argument array instance
 *       clish_pargv_dump -      print argument array to stdout
 *       clish_parg_dump -       print argument to stdout
 *
 * ATTRIBUTES
 *
 *       clish_parg__get_name -  get the parameter argument name for an argument instance
 *       clish_parg__get_value - get the value for a paramter argument instance
 *       clish_parg__get_ptype - get the parameter type for an argument instance
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
    clish_pargv_t *         clish_pargv_new(
                              const clish_command_t *cmd,
                              const char            *line,
                              size_t                 offset,
                              clish_pargv_status_t  *status);

    void                    clish_pargv_delete(
                              clish_pargv_t         *instance);

    const clish_parg_t *    clish_pargv_find_arg(
                              clish_pargv_t         *instance, 
                              const char            *name);

    void                    clish_pargv_dump(
                              const clish_pargv_t   *instance);

    void                    clish_parg_dump(
                              const clish_parg_t    *instance);
    
    /** ATTRIBUTES ************************************************************/
    const char *            clish_parg__get_name(
                              const clish_parg_t    *instance);

    const char *            clish_parg__get_value(
                              const clish_parg_t    *instance);

    const clish_ptype_t *   clish_parg__get_ptype(
                              const clish_parg_t    *instance);

/** END PARG INTERFACE ********************************************************/







#endif /* _clish_pargv_h */
