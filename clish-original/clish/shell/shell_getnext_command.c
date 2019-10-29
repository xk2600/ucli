/*
 * shell_getnext_command.c
 */
#include "private.h"

/*--------------------------------------------------------- */
const ucli_command_t *
ucli_shell_getnext_command(ucli_shell_t *this,
                            const char    *line)
{
    return ucli_shell_find_next_completion(this,line,&this->iter);
}
/*--------------------------------------------------------- */
