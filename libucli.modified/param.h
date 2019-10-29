/* FILE param.h
 *
 * PACKAGE ucli
 * MODULE param
 * 
 * BRIEF This class represents an instance of a parameter type.
 *
 * DETAIL Parameter instances are assocated with a command line and used to validate the
 * the arguments which a user is inputing for a command.
 *
 */
#ifndef _ucli_param_h
#define _ucli_param_h

typedef struct ucli_param_s ucli_param_t;

#include "libucli/ptype.h"



/******************************************************************************/
/* PARAM PUBLIC INTERFACE -- (implemented in param.c)
 *
 * REFERENCE
 *
 *       ## PARAM REPRESENTATION ########
 *
 *       struct ucli_param_s {
 *           char          *name;
 *           char          *text;
 *           ucli_ptype_t *ptype;
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
 *       ucli_param_new -           creates a param
 *       ucli_param_delete -        deletes a param
 *       ucli_param_help -          gets help text
 *       ucli_param_validate -      validates syntax
 *       ucli_param_dump -          dumps parameters
 *
 * ATTRIBUTES
 *
 *       ucli_param__get_name -     gets the name of the parameter
 *       ucli_param__get_text -     gets the help text for the parameter
 *       ucli_param__get_range -    gets the range for an (unsigned)integer parameter
 *       ucli_param__get_prefix -   gets the token which must prefix this value
 *       ucli_param__get_default -  gets the default value
 *       ucli_param__get_ptype -    gets the paramater type
 *       ucli_param__set_prefix -   sets the token which must prefix this value
 *       ucli_param__set_default -  sets the default value
 * 
 * ARGUMENTS
 *
 *       name           name of the parameter
 *       text           syntax help provided at '?'
 *       ptype          parameter type (ucli_ptype_method_e)
 *       instance       instance of a parameter type
 *       offset         n
 *       name           n
 *       name           n
 */


    /** METHODS ***************************************************************/
    ucli_param_t *         ucli_param_new(
                              const char          *name,
                              const char          *text,
                              ucli_ptype_t       *ptype);

    void                    ucli_param_delete(
                              ucli_param_t       *instance);

    void                    ucli_param_help(
                              const ucli_param_t *instance,
                              size_t               offset);

    char *                  ucli_param_validate(
                              const ucli_param_t *instance,
                              const char          *text);

    void                    ucli_param_dump(
                              const ucli_param_t *instance);
    
    /** ATTRIBUTES ************************************************************/
    const char *            ucli_param__get_name(
                              const ucli_param_t *instance);

    const char *            ucli_param__get_text(
                              const ucli_param_t *instance);

    const char *            ucli_param__get_range(
                              const ucli_param_t *instance);

    const char *            ucli_param__get_prefix(
                              const ucli_param_t *instance);

    const char *            ucli_param__get_default(
                              const ucli_param_t *instance);

    ucli_ptype_t *         ucli_param__get_ptype(
                              const ucli_param_t *instance);

    void                    ucli_param__set_prefix(
                              ucli_param_t       *instance,
                              const char          *prefix);

    void                    ucli_param__set_default(
                              ucli_param_t       *instance,
                              const char          *defval);

#endif /* _ucli_param_h */
/** @} ucli_param */
