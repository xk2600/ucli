/*
 * shell__get_tinyrl.c
 */
#include "private.h"

/*--------------------------------------------------------- */
tinyrl_t *
ucli_shell__get_tinyrl(const ucli_shell_t *this)
{
	return this->tinyrl;
}
/*--------------------------------------------------------- */
