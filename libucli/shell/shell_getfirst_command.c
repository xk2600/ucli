/*
 * shell_getfirst_command.c
 */
#include "private.h"

/*--------------------------------------------------------- */
const ucli_command_t *
ucli_shell_getfirst_command(ucli_shell_t *this,
                             const char    *line)
{
    ucli_shell_iterator_init(&this->iter);
    
    /* find the first command for which this is a prefix */
    return ucli_shell_getnext_command(this,line);
}
/*--------------------------------------------------------- */
