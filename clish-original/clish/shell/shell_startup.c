/*
 * shell_startup.c
 */
#include "private.h"
#include <assert.h>

/*----------------------------------------------------------- */
bool_t
ucli_shell_startup(ucli_shell_t *this)
{
    const char    *banner;
    ucli_pargv_t *dummy = NULL;
    
    assert(this->startup);
    
    banner = ucli_command__get_detail(this->startup);
    
    if(NULL != banner)
    {
        tinyrl_printf(this->tinyrl,"%s\n",banner);
    }
    return ucli_shell_execute(this,this->startup,&dummy);
}
/*----------------------------------------------------------- */
