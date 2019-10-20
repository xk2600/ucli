/* FILE param.h
 *
 * PACKAGE clish
 * MODULE param
 * 
 * BRIEF This class represents an instance of a parameter type.
 *
 * DETAIL Parameter instances are assocated with a command line and used to validate the
 * the arguments which a user is inputing for a command.
 *
 */
#ifndef _clish_param_h
#define _clish_param_h

typedef struct clish_param_s clish_param_t;

#include "clish/ptype.h"



/******************************************************************************/
/* PARAM PUBLIC INTERFACE -- (implemented in param.c)
 *
 * REFERENCE
 *
 *       ## PARAM REPRESENTATION ########
 *
 *       struct clish_param_s {
 *           char          *name;
 *           char          *text;
 *           clish_ptype_t *ptype;
 *           char          *prefix;
 *           char          *defval;
 *       };
 *
 * ELEMENTS
 *
 *       name           parameter name
 *       text           parameter inline help text
 *       ptype          type of this parameter
 *       prefix         if non NULL the name of the identifying switch parameter
 *       defval         default value to use fo this parameter
 *
 * METHODS
 * 
 *       clish_param_new -           creates a param
 *       clish_param_delete -        deletes a param
 *       clish_param_help -          gets help text
 *       clish_param_validate -      validates syntax
 *       clish_param_dump -          dumps parameters
 *
 * ATTRIBUTES
 *
 *       clish_param__get_name -     gets the name of the parameter
 *       clish_param__get_text -     gets the help text for the parameter
 *       clish_param__get_range -    gets the range for an (unsigned)integer parameter
 *       clish_param__get_prefix -   gets the token which must prefix this value
 *       clish_param__get_default -  gets the default value
 *       clish_param__get_ptype -    gets the paramater type
 *       clish_param__set_prefix -   sets the token which must prefix this value
 *       clish_param__set_default -  sets the default value
 * 
 * ARGUMENTS
 *
 *       name           name of the parameter
 *       text           syntax help provided at '?'
 *       ptype          parameter type (clish_ptype_method_e)
 *       instance       instance of a parameter type
 *       offset         n
 *       name           n
 *       name           n
 */


    /** METHODS ***************************************************************/
    clish_param_t *         clish_param_new(
                              const char          *name,
                              const char          *text,
                              clish_ptype_t       *ptype);

    void                    clish_param_delete(
                              clish_param_t       *instance);

    void                    clish_param_help(
                              const clish_param_t *instance,
                              size_t               offset);

    char *                  clish_param_validate(
                              const clish_param_t *instance,
                              const char          *text);

    void                    clish_param_dump(
                              const clish_param_t *instance);
    
    /** ATTRIBUTES ************************************************************/
    const char *            clish_param__get_name(
                              const clish_param_t *instance);

    const char *            clish_param__get_text(
                              const clish_param_t *instance);

    const char *            clish_param__get_range(
                              const clish_param_t *instance);

    const char *            clish_param__get_prefix(
                              const clish_param_t *instance);

    const char *            clish_param__get_default(
                              const clish_param_t *instance);

    clish_ptype_t *         clish_param__get_ptype(
                              const clish_param_t *instance);

    void                    clish_param__set_prefix(
                              clish_param_t       *instance,
                              const char          *prefix);

    void                    clish_param__set_default(
                              clish_param_t       *instance,
                              const char          *defval);

#endif /* _clish_param_h */
/** @} clish_param */
