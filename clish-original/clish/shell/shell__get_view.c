/*
 * shell__get_view.c
 */
#include "private.h"

/*--------------------------------------------------------- */
const ucli_view_t *
ucli_shell__get_view(const ucli_shell_t *this)
{
	return this->view;
}
/*--------------------------------------------------------- */
