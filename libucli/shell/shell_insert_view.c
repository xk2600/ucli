/*
 * shell_insert_view.c
 */
#include "private.h"

/*--------------------------------------------------------- */
void
ucli_shell_insert_view(ucli_shell_t *this,
                        ucli_view_t  *view)
{
    (void)lub_bintree_insert(&this->view_tree,view);
}  
/*--------------------------------------------------------- */
