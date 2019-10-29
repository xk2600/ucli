/*
 * shell__get_client_cookie.c
 */
#include "private.h"

/*--------------------------------------------------------- */
void *
ucli_shell__get_client_cookie(const ucli_shell_t *this)
{
	return this->client_cookie;
}
/*--------------------------------------------------------- */
