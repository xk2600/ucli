/*
 * view.h
 */
 /**
\ingroup ucli
\defgroup ucli_view view
@{

\brief This class is a container of commands.

A particular CLI session may contain a number of different views. Each 
view may contain its own specific commands as well as those available at
a global scope.

*/
#ifndef _ucli_view_h
#define _ucli_view_h

typedef struct ucli_view_s ucli_view_t;

#include "libucli/command.h"

/*=====================================
 * VIEW INTERFACE
 *===================================== */
/*-----------------
 * meta functions
 *----------------- */
ucli_view_t *
		ucli_view_new(const char *name,
             	  	       const char *prompt);
int
		ucli_view_bt_compare(const void *clientnode,
                	              const void *clientkey);
void
		ucli_view_bt_getkey(const void        *clientnode,
                	             lub_bintree_key_t *key);
size_t
		ucli_view_bt_offset(void);
/*-----------------
 * methods
 *----------------- */
void
		ucli_view_delete(ucli_view_t *instance);
ucli_command_t *
		ucli_view_new_command(ucli_view_t *instance,
                		       const char   *name,
                     		       const char   *text);
ucli_command_t *
		ucli_view_find_command(ucli_view_t *instance,
                		        const char   *name);
const ucli_command_t *
		ucli_view_find_next_completion(ucli_view_t          *instance,
                		                const ucli_command_t *cmd,
                		                const char            *line);
ucli_command_t *
		ucli_view_resolve_command(ucli_view_t *instance,
                		           const char   *line);                    
ucli_command_t *
		ucli_view_resolve_prefix(ucli_view_t *instance,
				          const char   *line);
void
		ucli_view_dump(ucli_view_t *instance);
/*-----------------
 * attributes 
 *----------------- */
const char *
		ucli_view__get_name(const ucli_view_t *instance);
void
		ucli_view__set_prompt(ucli_view_t *instance,
                		       const char   *prompt);
char *
		ucli_view__get_prompt(const ucli_view_t  *instance,
                               const char          *viewid);

#endif /* _ucli_view_h */
/** @} ucli_view */
