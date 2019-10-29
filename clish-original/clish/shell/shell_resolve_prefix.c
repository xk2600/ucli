/*
 * shell_resolve_prefix.c
 */
#include "private.h"

/*--------------------------------------------------------- */
const ucli_command_t *
ucli_shell_resolve_prefix(const ucli_shell_t *this,
                           const char          *line)
{
	ucli_command_t *cmd1,*cmd2;
	
	/* search the current view and global view */
	cmd1 = ucli_view_resolve_prefix(this->view,line);
	cmd2 = ucli_view_resolve_prefix(this->global,line);

	/* choose the longest match */
	return ucli_command_choose_longest(cmd1,cmd2);
}
/*----------------------------------------------------------- */
