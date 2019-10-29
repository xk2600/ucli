/*
 * shell_find_create_view.c
 */
#include "private.h"

#include <assert.h>
/*--------------------------------------------------------- */
ucli_view_t *
ucli_shell_find_create_view(ucli_shell_t *this,
                             const char    *name,
                             const char    *prompt)
{
	ucli_view_t *view = lub_bintree_find(&this->view_tree,name);

	if(NULL == view) 
	{
		/* create a view */
		view = ucli_view_new(name,prompt);
		assert(view);
		ucli_shell_insert_view(this,view);
	}
	else
	{
		if(prompt)
		{
			/* set the prompt */
			ucli_view__set_prompt(view,prompt);
		}
	}
	return view;
}
/*--------------------------------------------------------- */
