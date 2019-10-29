/*
 * shell_find_view.c
 */
#include "private.h"

/*--------------------------------------------------------- */
ucli_view_t *
ucli_shell_find_view(ucli_shell_t *this,
                      const char    *name)
{
	return lub_bintree_find(&this->view_tree,name);
}  
/*--------------------------------------------------------- */
