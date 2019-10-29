/*
 * shell_insert_view.c
 */
#include "private.h"

/*--------------------------------------------------------- */
void
ucli_shell_insert_ptype(ucli_shell_t *this,
                         ucli_ptype_t *ptype)
{
    (void)lub_bintree_insert(&this->ptype_tree,ptype);
}  
/*--------------------------------------------------------- */
