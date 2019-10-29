/* FILE command.h
 *
 * PACKAGE ucli
 * MODULE param
 *
 * BRIEF
 *
 * DETAIL
 *
 */
#ifndef _ucli_command_h
#define _ucli_command_h

typedef struct ucli_command_s ucli_command_t;

#include "lub/bintree.h"
#include "libucli/pargv.h"
#include "libucli/view.h"
#include "libucli/param.h"



/******************************************************************************/
/* COMMAND PUBLIC INTERFACE -- (implemented in command.c)
 *
 * REFERENCE
 *
 *       ## COMMAND REPRESENTATION ########
 *
 *       struct ucli_command_s {
 *       };
 *
 * ELEMENTS
 *
 *       name           parameter name
 *       text           parameter inline help text
 *
 * METHODS
 * 
 *       ucli_command_new -           creates a command
 *       ucli_command_delete -        deletes a command
 *       ucli_command_help -          gets help text
 *       ucli_command_dump -          dumps parameters
 *
 * ATTRIBUTES
 *
 *       ucli_command__get_name -     gets the name of the command

 * 
 * ARGUMENTS
 *
 *       name           name of the command
 *       text           syntax help provided at '?'
 *       instance       instance of a command
 *       offset         n
 *       name           n
 *       name           n
 */


    /** METHODS ***************************************************************/
    ucli_command_t *       ucli_command_new(
      const char              *name,
      const char              *text);
 
    int                     ucli_command_bt_compare(
      const void              *clientnode,
      const void              *clientkey);

    void                    ucli_command_bt_getkey(
      const void              *clientnode,
      lub_bintree_key_t       *key);

    size_t                  ucli_command_bt_offset(void);

    ucli_command_t *       ucli_command_choose_longest(
      ucli_command_t         *cmd1,
      ucli_command_t         *cmd2);

    int                     ucli_command_diff(
      const ucli_command_t   *cmd1,
      const ucli_command_t   *cmd2);


    void                    ucli_command_delete(
      ucli_command_t         *instance);

    void                    ucli_command_insert_param(
      ucli_command_t         *instance,
      ucli_param_t           *param);

    void                    ucli_command_help(
      const ucli_command_t   *instance);

    const ucli_param_t *   ucli_command_find_option(
      const ucli_command_t   *instance,
      const char              *name);

    void                    ucli_command_dump(
      const ucli_command_t   *instance);

    const ucli_param_t *   ucli_command_next_non_option(
      const ucli_command_t   *cmd,
      unsigned int            *index);

    /** ATTRIBUTES ************************************************************/
    const char *            ucli_command__get_name(
      const ucli_command_t   *instance);

    const char *            ucli_command__get_suffix(
      const ucli_command_t   *instance);

    const char *            ucli_command__get_text(
      const ucli_command_t   *instance);

    const char *            ucli_command__get_detail(
      const ucli_command_t   *instance);

    const char *            ucli_command__get_builtin(
      const ucli_command_t   *instance);

    const char *            ucli_command__get_escape_chars(
      const ucli_command_t   *instance);

    const ucli_param_t *   ucli_command__get_args(
      const ucli_command_t   *instance);

    char *                  ucli_command__get_action(
      const ucli_command_t   *instance,
      const char              *viewid,
      ucli_pargv_t           *pargv);

    ucli_view_t *          ucli_command__get_view(
      const ucli_command_t   *instance);
   
    char *                  ucli_command__get_viewid(
      const ucli_command_t   *instance,
      const char              *viewid,
      ucli_pargv_t           *pargv);

    const unsigned          ucli_command__get_param_count(
      const ucli_command_t   *instance);

    const ucli_param_t *   ucli_command__get_param(
      const ucli_command_t   *instance,
      unsigned                 index);

    void                    ucli_command__set_action(
      ucli_command_t         *instance,
      const char              *action);

    void                    ucli_command__set_builtin(
      ucli_command_t         *instance,
      const char              *builtin);

    void                    ucli_command__set_escape_chars(
      ucli_command_t         *instance,
      const char              *escape_chars);

    void                    ucli_command__set_args(
      ucli_command_t         *instance,
      ucli_param_t           *args);

    void                    ucli_command__set_detail(
      ucli_command_t         *instance,
      const char              *detail);
 
    void                    ucli_command__set_view(
      ucli_command_t         *instance,
      ucli_view_t            *view);

    void                    ucli_command__set_viewid(
      ucli_command_t         *instance,
      const char              *viewid);

#endif /* _ucli_command_h */
