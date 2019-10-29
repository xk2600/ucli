/*
 * shell_parse.c
 */
#include "private.h"
#include "lub/string.h"

#include <string.h>
/*----------------------------------------------------------- */
ucli_pargv_status_t
ucli_shell_parse(const ucli_shell_t    *this,
                  const char             *line,
                  const ucli_command_t **cmd,
                  ucli_pargv_t         **pargv)
{
    ucli_pargv_status_t result = ucli_BAD_CMD;
    size_t             offset;
    char              *prompt = ucli_view__get_prompt(this->view,this->viewid);
    
    /* track the offset of each parameter on the command line */
    offset = strlen(prompt)+1;
    
    /* cleanup */
    lub_string_free(prompt);

    *cmd = ucli_shell_resolve_command(this,line);
    if(NULL != *cmd)
    {
        /*
         * Now construct the parameters for the command
         */
        *pargv = ucli_pargv_new(*cmd,line,offset,&result);
    }
    return result;
}
/*----------------------------------------------------------- */
