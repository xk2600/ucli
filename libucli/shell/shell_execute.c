/*
 * shell_execute.c
 */
#include "private.h"
#include "lub/string.h"
#include "lub/argv.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

/*
 * These are the internal commands for this framework.
 */
static ucli_shell_builtin_fn_t 
    ucli_close,
    ucli_overview,
    ucli_source,
    ucli_source_nostop,
    ucli_history;

static ucli_shell_builtin_t ucli_cmd_list[] =
{
    {"ucli_close",         ucli_close},
    {"ucli_overview",      ucli_overview},
    {"ucli_source",        ucli_source},
    {"ucli_source_nostop", ucli_source_nostop},
    {"ucli_history",       ucli_history},
    {NULL,NULL}
};
/*----------------------------------------------------------- */
/*
 Terminate the current shell session 
*/
static bool_t
ucli_close(const ucli_shell_t *shell,
            const lub_argv_t    *argv)
{
    /* the exception proves the rule... */
    ucli_shell_t *this = (ucli_shell_t *)shell;
    
    argv = argv; /* not used */
    this->state = SHELL_STATE_CLOSING;

    return BOOL_TRUE;
}
/*----------------------------------------------------------- */
/*
 Open a file and interpret it as a script in the context of a new
 thread. Whether the script continues after command, but not script, 
 errors depends on the value of the stop_on_error flag.
*/
static bool_t
ucli_source_internal(const ucli_shell_t *shell,
		      const lub_argv_t    *argv,
                      bool_t              stop_on_error)
{
    bool_t      result = BOOL_FALSE;
    const char *filename = lub_argv__get_arg(argv,0);
    struct  stat   fileStat;
    FILE *file;

    /* the exception proves the rule... */
    ucli_shell_t *this = (ucli_shell_t *)shell;

    /*
     * Check file specified is not a directory 
     */
    if (0 == stat((char *) filename, &fileStat))
    {
        if (!S_ISDIR (fileStat.st_mode))
        {
           file = fopen(filename,"r");
           if (NULL != file)
           {

                /* 
                 * push this file onto the file stack associated with this
                 * session. This will be closed by ucli_shell_pop_file() 
                 * when it is finished with. 
                 */
                result = ucli_shell_push_file((ucli_shell_t *) this, file, stop_on_error);
                if (BOOL_FALSE == result)
                {
                    /* close the file here */
                    fclose(file);
                }
            }
        } 
    }
    return result;
}
/*----------------------------------------------------------- */
/*
 Open a file and interpret it as a script in the context of a new
 thread. Invoking a script in this way will cause the script to
 stop on the first error
*/
static bool_t
ucli_source(const ucli_shell_t *shell,
             const lub_argv_t    *argv)
{
    return(ucli_source_internal(shell, argv, BOOL_TRUE));
}
/*----------------------------------------------------------- */
/*
 Open a file and interpret it as a script in the context of a new
 thread. Invoking a script in this way will cause the script to
 continue after command, but not script, errors.
*/
static bool_t
ucli_source_nostop(const ucli_shell_t *shell,
             const lub_argv_t    *argv)
{
    return(ucli_source_internal(shell, argv, BOOL_FALSE));
}
/*----------------------------------------------------------- */
/*
 Show the shell overview
*/
static bool_t
ucli_overview(const ucli_shell_t *this,
               const lub_argv_t    *argv)
{
    argv = argv; /* not used */
    
    tinyrl_printf(this->tinyrl,"%s\n",this->overview);

    return BOOL_TRUE;
}
/*----------------------------------------------------------- */
static bool_t
ucli_history(const ucli_shell_t *this,
              const lub_argv_t    *argv)
{
    tinyrl_history_t             *history=tinyrl__get_history(this->tinyrl);
    tinyrl_history_iterator_t     iter;
    const tinyrl_history_entry_t *entry;
    unsigned                      limit = 0;
    const char                   *arg = lub_argv__get_arg(argv,0);
    
    if((NULL != arg) && ('\0' != *arg))
    {
        limit = (unsigned)atoi(arg);

        if(0 == limit)
        {
            /* unlimit the history list */
            (void)tinyrl_history_unstifle(history);
        }
        else
        {
            /* limit the scope of the history list */
            tinyrl_history_stifle(history,limit);
        }
    }
    for(entry = tinyrl_history_getfirst(history,&iter);
        entry;
        entry = tinyrl_history_getnext(&iter))
    {
        /* dump the details of this entry */
        tinyrl_printf(this->tinyrl,
                      "%5d  %s\n",
                      tinyrl_history_entry__get_index(entry),
                      tinyrl_history_entry__get_line(entry));
    }
    return BOOL_TRUE;
}
/*----------------------------------------------------------- */
/*
 * Searches for a builtin command to execute
 */
static ucli_shell_builtin_fn_t *
find_builtin_callback(const ucli_shell_builtin_t *cmd_list,
                      const char                  *name)
{
    const ucli_shell_builtin_t *result;
    
    /* search a list of commands */
    for(result = cmd_list;
        result && result->name;
        result++)
    {
        if(0 == strcmp(name,result->name))
        {
            break;
        }
    }
    return (result && result->name) ? result->callback : NULL;
}
/*----------------------------------------------------------- */
void
ucli_shell_cleanup_script(void *script)
{
    /* simply release the memory */
    lub_string_free(script);
}
/*----------------------------------------------------------- */
bool_t
ucli_shell_execute(ucli_shell_t         *this,
                    const ucli_command_t *cmd,
                    ucli_pargv_t        **pargv)
{
    bool_t      result = BOOL_TRUE;
    const char *builtin;
    char       *script;

    assert(NULL != cmd);

    builtin = ucli_command__get_builtin(cmd);
    script = ucli_command__get_action(cmd,this->viewid,*pargv);
    /* account for thread cancellation whilst running a script */
    pthread_cleanup_push((void(*)(void*))ucli_shell_cleanup_script,script);
    if(NULL != builtin)
    {
        ucli_shell_builtin_fn_t *callback;
        lub_argv_t               *argv = script ? lub_argv_new(script,0) : NULL;

        result = BOOL_FALSE;
        
        /* search for an internal command */
        callback = find_builtin_callback(ucli_cmd_list,builtin);
        
        if(NULL == callback)
        {
            /* search for a client command */
            callback = find_builtin_callback(this->client_hooks->cmd_list,builtin);
        }
        if(NULL != callback)
        {       
            /* invoke the builtin callback */
            result = callback(this,argv);
        }
        if(NULL != argv)
        {
            lub_argv_delete(argv);
        }
    }
    else if(NULL != script)
    {
        /* now get the client to interpret the resulting script */
        result = this->client_hooks->script_fn(this,script);

    }
    pthread_cleanup_pop(1);
    if(BOOL_TRUE == result)
    {
        /* move into the new view */
        ucli_view_t *view   = ucli_command__get_view(cmd);
        char         *viewid = ucli_command__get_viewid(cmd,this->viewid,*pargv);

        if(NULL != view)
        {
            this->view = view;
        }
        if(viewid)
        {
            /* cleanup */
            lub_string_free(this->viewid);
            this->viewid = viewid;
        }
    }
    if(NULL != *pargv)
    {
        ucli_pargv_delete(*pargv);
        *pargv = NULL;
    }
    return result;
}
/*----------------------------------------------------------- */
