/*
 * shell_delete.c
 */
#include "private.h"
#include "lub/string.h"

#include <stdlib.h>
/*--------------------------------------------------------- */
static void
ucli_shell_fini(ucli_shell_t *this)
{
	ucli_view_t  *view;
	ucli_ptype_t *ptype;
	

	/* delete each view held  */
	while((view = lub_bintree_findfirst(&this->view_tree)))
	{
		/* remove the command from the tree */
		lub_bintree_remove(&this->view_tree,view);

		/* release the instance */
		ucli_view_delete(view);
	}

	/* delete each ptype held  */
	while((ptype = lub_bintree_findfirst(&this->ptype_tree)))
	{
		/* remove the command from the tree */
		lub_bintree_remove(&this->ptype_tree,ptype);

		/* release the instance */
		ucli_ptype_delete(ptype);
	}
    /* free the textual details */
    lub_string_free(this->overview);
    lub_string_free(this->viewid);
    
	if(NULL != this->startup)
    {
        /* remove the startup command */
        ucli_command_delete(this->startup);
    }
    /* clean up the file stack */
    while(BOOL_TRUE == ucli_shell_pop_file(this))
    {
        /* not alot do do here */
    }
    /* delete the tinyrl object */
    ucli_shell_tinyrl_delete(this->tinyrl);
    

}
/*--------------------------------------------------------- */
void
ucli_shell_delete(ucli_shell_t *this)
{
    if(this->client_hooks->fini_fn)
    {
        /* now call the client finalisation */
        this->client_hooks->fini_fn(this);
    }
    
    ucli_shell_fini(this);

    free(this);
}
/*--------------------------------------------------------- */
