/* FILE command.h
 *
 * PACKAGE clish
 * MODULE param
 *
 * BRIEF
 *
 * DETAIL
 *
 */
#ifndef _clish_command_h
#define _clish_command_h

typedef struct clish_command_s clish_command_t;

#include "lub/bintree.h"
#include "lib/pargv.h"
#include "lib/view.h"
#include "lib/param.h"



/******************************************************************************/
/* COMMAND PUBLIC INTERFACE -- (implemented in command.c)
 *
 * REFERENCE
 *
 *       ## COMMAND REPRESENTATION ########
 *
 *       struct clish_command_s {
 *       };
 *
 * ELEMENTS
 *
 *       name           parameter name
 *       text           parameter inline help text
 *
 * METHODS
 * 
 *       clish_command_new -           creates a command
 *       clish_command_delete -        deletes a command
 *       clish_command_help -          gets help text
 *       clish_command_dump -          dumps parameters
 *
 * ATTRIBUTES
 *
 *       clish_command__get_name -     gets the name of the command

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
    clish_command_t *       clish_command_new(
      const char              *name,
      const char              *text);
 
    int                     clish_command_bt_compare(
      const void              *clientnode,
      const void              *clientkey);

    void                    clish_command_bt_getkey(
      const void              *clientnode,
      lub_bintree_key_t       *key);

    size_t                  clish_command_bt_offset(void);

    clish_command_t *       clish_command_choose_longest(
      clish_command_t         *cmd1,
      clish_command_t         *cmd2);

    int                     clish_command_diff(
      const clish_command_t   *cmd1,
      const clish_command_t   *cmd2);


    void                    clish_command_delete(
      clish_command_t         *instance);

    void                    clish_command_insert_param(
      clish_command_t         *instance,
      clish_param_t           *param);

    void                    clish_command_help(
      const clish_command_t   *instance);

    const clish_param_t *   clish_command_find_option(
      const clish_command_t   *instance,
      const char              *name);

    void                    clish_command_dump(
      const clish_command_t   *instance);

    const clish_param_t *   clish_command_next_non_option(
      const clish_command_t   *cmd,
      unsigned int            *index);

    /** ATTRIBUTES ************************************************************/
    const char *            clish_command__get_name(
      const clish_command_t   *instance);

    const char *            clish_command__get_suffix(
      const clish_command_t   *instance);

    const char *            clish_command__get_text(
      const clish_command_t   *instance);

    const char *            clish_command__get_detail(
      const clish_command_t   *instance);

    const char *            clish_command__get_builtin(
      const clish_command_t   *instance);

    const char *            clish_command__get_escape_chars(
      const clish_command_t   *instance);

    const clish_param_t *   clish_command__get_args(
      const clish_command_t   *instance);

    char *                  clish_command__get_action(
      const clish_command_t   *instance,
      const char              *viewid,
      clish_pargv_t           *pargv);

    clish_view_t *          clish_command__get_view(
      const clish_command_t   *instance);
   
    char *                  clish_command__get_viewid(
      const clish_command_t   *instance,
      const char              *viewid,
      clish_pargv_t           *pargv);

    const unsigned          clish_command__get_param_count(
      const clish_command_t   *instance);

    const clish_param_t *   clish_command__get_param(
      const clish_command_t   *instance,
      unsigned                 index);

    void                    clish_command__set_action(
      clish_command_t         *instance,
      const char              *action);

    void                    clish_command__set_builtin(
      clish_command_t         *instance,
      const char              *builtin);

    void                    clish_command__set_escape_chars(
      clish_command_t         *instance,
      const char              *escape_chars);

    void                    clish_command__set_args(
      clish_command_t         *instance,
      clish_param_t           *args);

    void                    clish_command__set_detail(
      clish_command_t         *instance,
      const char              *detail);
 
    void                    clish_command__set_view(
      clish_command_t         *instance,
      clish_view_t            *view);

    void                    clish_command__set_viewid(
      clish_command_t         *instance,
      const char              *viewid);

#endif /* _clish_command_h */
