/*
 * shell_set_context.c
 */
#include "private.h"

#include <assert.h>
/*--------------------------------------------------------- */
void
ucli_shell_set_context(ucli_shell_t *this,
                        const char    *viewname)
{
 	this->view   = ucli_shell_find_view(this,viewname);
 	assert(this->view);
 	assert(this->global);
}
/*--------------------------------------------------------- */
