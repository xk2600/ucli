/*
 * shell.h - private interface to the shell class
 */
#include "libucli/shell.h"
#include "libucli/pargv.h"

#include "lub/argv.h"
#include "lub/bintree.h"
#include "lub/dump.h"
#include "lub/string.h"

#include "tinyrl/history.h"
#include "tinyrl/tinyrl.h"

#include <assert.h>
#include <dirent.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>





/******************************************************************************/
/** PRIVATE TYPES *************************************************************/

/*
 * if CLISH_PATH is unset in the environment then this is the value used. 
 */
const char *default_path = "/etc/ucli;~/.ucli";

typedef enum {
 SHELL_STATE_INITIALISING,
 SHELL_STATE_READY,
 SHELL_STATE_HELPING,
 SHELL_STATE_SCRIPT_ERROR,
 SHELL_STATE_CLOSING
} shell_state_t;

/* iterate around commands */
typedef struct {

   const ucli_command_t     *last_cmd_local;
   const ucli_command_t     *last_cmd_global;
} ucli_shell_iterator_t;

/* this is used to maintain a stack of file handles */
typedef struct ucli_shell_file_s ucli_shell_file_t;
struct ucli_shell_file_s {

   ucli_shell_file_t        *next;
   FILE                      *file;
   bool_t                     stop_on_error;  /* stop on error for file input */
};

struct ucli_shell_s {

   lub_bintree_t              view_tree;      /* Maintain a tree of views */
   lub_bintree_t              ptype_tree;     /* Maintain a tree of ptypes */
   const ucli_shell_hooks_t *client_hooks;   /* Client callback hooks */
   void                      *client_cookie;  /* Client callback cookie */
   ucli_view_t              *global;         /* Reference to the global view. */
   ucli_view_t              *view;           /* Reference to the current view.*/
   ucli_command_t           *startup;        /* This is the startup command */
   ucli_shell_iterator_t     iter;           /* used for iterating commands */
   shell_state_t              state;          /* The current state */
   char                      *overview;       /* Overview text for this shell. */
   char                      *viewid;         /* The current view ID string */
   tinyrl_t                  *tinyrl;         /* Tiny readline instance */
   ucli_shell_file_t        *current_file;   /* file currently in use for input */
};


/* 
 * The context structure is used to simplify the cleanup of a CLI session when a 
 * thread is cancelled.
 */
typedef struct _context context_t;
struct _context {

    pthread_t                  pthread;
    const ucli_shell_hooks_t *hooks;
    void                      *cookie;
    FILE                      *istream;
    ucli_shell_t             *shell;
    const ucli_command_t     *command;
    ucli_pargv_t             *pargv;
    char                      *prompt;
};

/* This is used to hold context during tinyrl callbacks... changed to reuse 
 * _context defined above */

/* typedef struct _context context_t;
   struct _context {
  
      ucli_shell_t         *shell;
      const ucli_command_t *command;
      ucli_pargv_t         *pargv;
   }; */





/******************************************************************************/
/** PRIVATE LOCAL DECLARATIONS ************************************************/

/* Initialise a command iterator structure */
void ucli_shell_iterator_init(ucli_shell_iterator_t *iter);

/* get the next command which is an extension of the specified line */
const ucli_command_t *     ucli_shell_find_next_completion(
                              const ucli_shell_t    *instance, 
                              const char             *line, 
                              ucli_shell_iterator_t *iter);

/* Push the specified file handle on to the stack of file handles for this 
 * shell. The specified file will become the source of commands, until it is 
 * exhausted.
 *
 * Returns
 *
 *      BOOL_TRUE - the file was successfully associated with the shell.
 *      BOOL_FALSE - there was insufficient resource to associate this file. */
bool_t                      ucli_shell_push_file(
                              ucli_shell_t          *instance, 
                              FILE                   *file, 
                              bool_t                  stop_on_error);

/* Pop the current file handle from the stack of file handles, shutting the 
 * file down and freeing any associated memory. The next file handle in the 
 * stack becomes associated with the input stream for this shell.
 * 
 * Returns
 *
 *      BOOL_TRUE - the current file handle has been replaced.
 *      BOOL_FALSE - there is only one handle on the stack which cannot be replaced. */
bool_t ucli_shell_pop_file(ucli_shell_t *instance);

      ucli_view_t *        ucli_shell_find_view(
                              ucli_shell_t          *instance, 
                              const char             *name);

void                        ucli_shell_insert_view(
                              ucli_shell_t          *instance, 
                              ucli_view_t           *view);

      ucli_pargv_status_t  ucli_shell_parse(
                              const ucli_shell_t    *instance, 
                              const char             *line, 
                              const ucli_command_t **cmd,
                              ucli_pargv_t         **pargv);

char *                      ucli_shell_word_generator(
                              ucli_shell_t          *instance, 
                              const char             *line, 
                              unsigned int            offset, 
                              unsigned int            state);

const ucli_command_t *     ucli_shell_resolve_command(
                              const ucli_shell_t    *instance, 
                              const char             *line);

const ucli_command_t *     ucli_shell_resolve_prefix(
                              const ucli_shell_t    *instance,
                              const char             *line);

const ucli_command_t *     ucli_shell_getfirst_command(
                              ucli_shell_t          *instance,
                              const char             *line);

const ucli_command_t *     ucli_shell_getnext_command(
                              ucli_shell_t          *instance,
                              const char             *line);

void                        ucli_shell_insert_ptype(
                              ucli_shell_t          *instance,
                              ucli_ptype_t          *ptype);

void                        ucli_shell_tinyrl_history(
                              ucli_shell_t          *instance,
                              unsigned int           *limit);

tinyrl_t *                  ucli_shell_tinyrl_new(
                              FILE                   *instream,
                              FILE                   *outstream,
                              unsigned int            stifle);

void                        ucli_shell_tinyrl_delete(
                              tinyrl_t               *instance);


static void                 ucli_shell_cleanup(
                              context_t              *context);






/******************************************************************************/
/** PUBLIC ATTRIBUTES *********************************************************/

/* shell__get_client_cookie.c */

void *
ucli_shell__get_client_cookie(const ucli_shell_t *this) {

	return this->client_cookie;
}
/* shell__get_tinyrl.c */

tinyrl_t *
ucli_shell__get_tinyrl(const ucli_shell_t *this) {
	return this->tinyrl;
}
/* shell__get_view.c */

const ucli_view_t *
ucli_shell__get_view(const ucli_shell_t *this) {

	return this->view;
}
/* shell__get_viewid.c */

const char *
ucli_shell__get_viewid(const ucli_shell_t *this) {
	return this->viewid;
}








/******************************************************************************/
/** PRIVATE LOCAL FUNCTIONS ***************************************************/

/** word_generator **/
void
ucli_shell_iterator_init(ucli_shell_iterator_t *iter) {

    iter->last_cmd_local  = NULL;
    iter->last_cmd_global = NULL;
}

const ucli_command_t *
ucli_shell_find_next_completion(const ucli_shell_t    *this,
                                 const char             *line,
                                 ucli_shell_iterator_t *iter) {

    const ucli_command_t *result=NULL,*cmd1,*cmd2;
    int                    diff;

    /* ask the local view for it's next command */
    cmd1 = ucli_view_find_next_completion(this->view,
                                           iter->last_cmd_local,
                                           line);
    /* ask the global view for it's next command */
    cmd2 = ucli_view_find_next_completion(this->global,
                                           iter->last_cmd_global,
                                           line);
    /* compare the two results */
    diff = ucli_command_diff(cmd1,cmd2);
    if(diff > 0) {
        result = iter->last_cmd_global = cmd2;
    } else {

        if(0 == diff) {

            /* local view may override global command */
            iter->last_cmd_global = cmd2;
        }
        result = iter->last_cmd_local = cmd1;
    }
    return result;
}

static char *
ucli_shell_param_generator(const ucli_shell_t   *this,
                            const ucli_command_t *cmd,
                            const char            *line,
                            unsigned               offset,
                            unsigned               state) {

    char                *result      = NULL;
    const char          *name        = ucli_command__get_name(cmd);
    char                *text        = lub_string_dup(&line[offset]);
    unsigned             index;
    const ucli_param_t *param;
    ucli_ptype_t       *ptype;
    
    /* get the index of the current parameter */
    index = lub_argv_wordcount(line) - lub_argv_wordcount(name);

    if((0 != index) || (line[offset-1] == ' '))
    {
        if((0 != index) && (text[0] != '\0'))
        {
            /* if there is some text for the parameter then adjust the index */
            index--;
        }
        /* get the parameter definition */
        param = ucli_command__get_param(cmd,index);
        if(NULL != param)
        {
            /* get the parameter type */
            ptype = ucli_param__get_ptype(param);
            /* pass the parameter completion onto the ptype class */
            result = ucli_ptype_word_generator(ptype,text,state);

            if((state == 0) && (NULL == result))
            {
                /* only show an error if this is a required parameter */
                if (NULL == ucli_param__get_default(param))
                {
                    tinyrl_crlf(this->tinyrl);
                    /* the extra '1' is because the ucli_param_help()
                     * takes a one-based offset and that passed into here
                     * is zero based 
                     */
                    ucli_param_help(param,
                                     1 + offset + strlen(tinyrl__get_prompt(this->tinyrl)));
                    /* make sure we reset the line state */
                    tinyrl_crlf(this->tinyrl);
                    tinyrl_reset_line_state(this->tinyrl);
                }
                tinyrl_completion_error_over(this->tinyrl);
            }
        }
        else if((0 == state) && (NULL != ucli_command__get_args(cmd)))
        {
            result = lub_string_dup(text);
        }
            
    }
    else if(0 == state)
    {
        /* simply return the command name */
        result = lub_string_dup(ucli_command__get_suffix(cmd));
    }
    lub_string_free(text);
    return result;
}

static char *
ucli_shell_command_generator(ucli_shell_t *this,
                              const char    *line,
                              unsigned       offset,
                              unsigned       state) {

    char                  *result = NULL;
    const ucli_command_t *cmd    = NULL;
    if(0 == state)
    {
        cmd = ucli_shell_getfirst_command(this,line);
    }
    else
    {
        cmd = ucli_shell_getnext_command(this,line);
    }

    if(NULL != cmd)
    {
        result = lub_string_dup(ucli_command__get_suffix(cmd));
    }
    /* keep the compiler happy */
    offset = offset;
    return result;
}

char *
ucli_shell_word_generator(ucli_shell_t *this,
                           const char    *line,
                           unsigned       offset,
                           unsigned       state) {

    char                  *result     = NULL;
    const ucli_command_t *cmd, *next = NULL;

    /* try and resolve a command which is a prefix of the line */
    cmd  = ucli_shell_resolve_command(this,line);
    if(NULL != cmd)
    {
        ucli_shell_iterator_t iter;
        /* see whether there is an extended extension */
        ucli_shell_iterator_init(&iter);
        next  = ucli_shell_find_next_completion(this,
                                                line,
                                                &iter);
    }
    if((NULL != cmd) && (NULL == next))
    {
        /* this needs to be completed as a parameter */
        result = ucli_shell_param_generator(this,cmd,line,offset,state);
    }
    else
    {
        /* this needs to be completed as a command */
        result = ucli_shell_command_generator(this,line,offset,state);
    }
    if(0 == state)
    {
        /* reset the state from a help perspective */
        this->state = SHELL_STATE_READY;
    }
    return result;
}



/*
 * shell_execute.c
 */


/*
 * These are the internal commands for this framework.
 */
static ucli_shell_builtin_fn_t 
    ucli_close,
    ucli_overview,
    ucli_source,
    ucli_source_nostop,
    ucli_history;

static ucli_shell_builtin_t ucli_cmd_list[] = {

    {"ucli_close",         ucli_close},
    {"ucli_overview",      ucli_overview},
    {"ucli_source",        ucli_source},
    {"ucli_source_nostop", ucli_source_nostop},
    {"ucli_history",       ucli_history},
    {NULL,NULL}
};

/*
 Terminate the current shell session 
*/
static bool_t
ucli_close(const ucli_shell_t *shell,
            const lub_argv_t    *argv) {

    /* the exception proves the rule... */
    ucli_shell_t *this = (ucli_shell_t *)shell;
    
    argv = argv; /* not used */
    this->state = SHELL_STATE_CLOSING;

    return BOOL_TRUE;
}

/*
 Open a file and interpret it as a script in the context of a new
 thread. Whether the script continues after command, but not script, 
 errors depends on the value of the stop_on_error flag.
*/
static bool_t
ucli_source_internal(const ucli_shell_t *shell,
		      const lub_argv_t    *argv,
                      bool_t              stop_on_error) {

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

/*
 Open a file and interpret it as a script in the context of a new
 thread. Invoking a script in this way will cause the script to
 stop on the first error
*/
static bool_t
ucli_source(const ucli_shell_t *shell,
             const lub_argv_t    *argv) {

    return(ucli_source_internal(shell, argv, BOOL_TRUE));
}

/*
 Open a file and interpret it as a script in the context of a new
 thread. Invoking a script in this way will cause the script to
 continue after command, but not script, errors.
*/
static bool_t
ucli_source_nostop(const ucli_shell_t *shell,
             const lub_argv_t    *argv) {

    return(ucli_source_internal(shell, argv, BOOL_FALSE));
}

/*
 Show the shell overview
*/
static bool_t
ucli_overview(const ucli_shell_t *this,
               const lub_argv_t    *argv) {

    argv = argv; /* not used */
    
    tinyrl_printf(this->tinyrl,"%s\n",this->overview);

    return BOOL_TRUE;
}

static bool_t
ucli_history(const ucli_shell_t *this,
              const lub_argv_t    *argv) {

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

/*
 * Searches for a builtin command to execute
 */
static ucli_shell_builtin_fn_t *
find_builtin_callback(const ucli_shell_builtin_t *cmd_list,
                      const char                  *name) {

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

void
ucli_shell_cleanup_script(void *script) {

    /* simply release the memory */
    lub_string_free(script);
}

bool_t
ucli_shell_execute(ucli_shell_t         *this,
                    const ucli_command_t *cmd,
                    ucli_pargv_t        **pargv) {

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

/*
 * shell_find_create_ptype.c
 */


ucli_ptype_t *
ucli_shell_find_create_ptype(ucli_shell_t           *this,
                              const char              *name,
                              const char              *text,
                              const char              *pattern,
                              ucli_ptype_method_e     method,
                              ucli_ptype_preprocess_e preprocess) {

    ucli_ptype_t *ptype = lub_bintree_find(&this->ptype_tree,name);

    if(NULL == ptype) 
    {
        /* create a ptype */
        ptype = ucli_ptype_new(name,text,pattern,method,preprocess);
        assert(ptype);
        ucli_shell_insert_ptype(this,ptype);
    }
    else
    {
        if(pattern)
        {
            /* set the pattern */
            ucli_ptype__set_pattern(ptype,pattern,method);
            /* set the preprocess */
            ucli_ptype__set_preprocess(ptype,preprocess);
        }
        if(text)
        {
            /* set the help text */
            ucli_ptype__set_text(ptype,text);
        }
    }
    return ptype;
}

/*
 * shell_find_create_view.c
 */


ucli_view_t *
ucli_shell_find_create_view(ucli_shell_t *this,
                             const char    *name,
                             const char    *prompt) {

	ucli_view_t *view = lub_bintree_find(&this->view_tree,name);

	if(NULL == view) 
	{
		/* create a view */
		view = ucli_view_new(name,prompt);
		assert(view);
		ucli_shell_insert_view(this,view);
	}
	else
	{
		if(prompt)
		{
			/* set the prompt */
			ucli_view__set_prompt(view,prompt);
		}
	}
	return view;
}

/*
 * shell_find_view.c
 */


ucli_view_t *
ucli_shell_find_view(ucli_shell_t *this,
                      const char    *name) {

	return lub_bintree_find(&this->view_tree,name);
}  

/*
 * shell_getfirst_command.c
 */


const ucli_command_t *
ucli_shell_getfirst_command(ucli_shell_t *this,
                             const char    *line) {

    ucli_shell_iterator_init(&this->iter);
    
    /* find the first command for which this is a prefix */
    return ucli_shell_getnext_command(this,line);
}

/*
 * shell_getnext_command.c
 */


const ucli_command_t *
ucli_shell_getnext_command(ucli_shell_t *this,
                            const char    *line) {

    return ucli_shell_find_next_completion(this,line,&this->iter);
}

/*
 * shell_help.c
 */







/*
 * Provide a detailed list of the possible command completions
 */
static void
available_commands(ucli_shell_t         *this,
                   const char            *line,
                   bool_t                 full) {

    char                  *buf = NULL;
    size_t                 max_width = 0;
    const ucli_command_t *cmd;
    if(NULL == ucli_shell_getfirst_command(this,line))
    {
        /*
        * A totally wrong command has been inputed
        * Indicate the point of error and display 
        * a list of possible commands
        */
        char    *prompt       = ucli_view__get_prompt(this->view,
                                                       this->viewid);
        unsigned error_offset = strlen(prompt)+1;
        lub_string_free(prompt);

        /* find the best match... */
        cmd = ucli_shell_resolve_prefix(this,line);
        if(NULL != cmd)
        {
            error_offset += strlen(ucli_command__get_name(cmd))+1;
            /* take a copy for help purposes */
            buf = lub_string_dup(ucli_command__get_name(cmd)); 
        }
        else
        {
            /* show all possible commands */
            buf = lub_string_dup("");
        }
        /* indicate the point of error */
        printf("%*s\n",error_offset,"^");
    }
    else
    {
        /* take a copy */
        buf = lub_string_dup(line); 
    }
    /* iterate round to determine max_width */
    for(cmd = ucli_shell_getfirst_command(this,buf);
        cmd;
        cmd = ucli_shell_getnext_command(this,buf))
    {
            size_t      width;
            const char *name;
            if(full)
            {
                name = ucli_command__get_name(cmd);
            }
            else
            {
               name = ucli_command__get_suffix(cmd);
            }
        width = strlen(name);
        if(width > max_width)
        {
            max_width = width;
        }
    }

    /* now iterate round to print the help */
    for(cmd = ucli_shell_getfirst_command(this,buf);
        cmd;
        cmd = ucli_shell_getnext_command(this,buf))
    {
        const char *name;
        if(full)
        {
            name = ucli_command__get_name(cmd);
        }
        else
        {
            name = ucli_command__get_suffix(cmd);
        }
        printf("%-*s  %s\n",
              max_width,
              name,
              ucli_command__get_text(cmd));
    }
    /* cleanup */
    lub_string_free(buf);
}

void
ucli_shell_help(ucli_shell_t *this,
                 const char    *line) {

    const ucli_command_t *cmd,*next_cmd, *first_cmd;
    
    /* if there are further commands then we need to show them too */
    cmd = ucli_shell_resolve_prefix(this,line);
    if(NULL != cmd)
    {
        ucli_shell_iterator_t iter;
        
        /* skip the command already known about */
        ucli_shell_iterator_init(&iter);

        first_cmd = ucli_shell_find_next_completion(this,
                                                     line,
                                                    &iter);
        next_cmd = ucli_shell_find_next_completion(this,
                                                    line,
                                                    &iter);
    }
    else
    {
        first_cmd = next_cmd = NULL;
    }
    if((NULL != cmd) && (NULL == next_cmd) && (!first_cmd || (first_cmd == cmd)))
    {
        /* we've resolved a particular command */
        switch(this->state)
        {
            case SHELL_STATE_HELPING:
            {
                const char *detail = ucli_command__get_detail(cmd);
                if(NULL != detail)
                {
                    printf("%s\n",detail);
                }
                else
                {
                    /* get the command to describe itself */
                    ucli_command_help(cmd);
                }
                break;
            }
            case SHELL_STATE_READY:
            case SHELL_STATE_SCRIPT_ERROR:
                /* get the command to provide help */
                ucli_command_help(cmd);
                break;
            case SHELL_STATE_INITIALISING:
            case SHELL_STATE_CLOSING:
                /* do nothing */
                break;
        }
    }
    else
    {
        /* dump the available commands */
        available_commands(this,line,BOOL_FALSE);
    }
    /* update the state */
    if(this->state == SHELL_STATE_HELPING)
    {
        this->state = SHELL_STATE_READY;
    }
    else
    {
        this->state = SHELL_STATE_HELPING;
    }
}

/*
 * shell_insert_view.c
 */
void
ucli_shell_insert_ptype(ucli_shell_t *this,
                         ucli_ptype_t *ptype) {

    (void)lub_bintree_insert(&this->ptype_tree,ptype);
}  

/*
 * shell_insert_view.c
 */
void
ucli_shell_insert_view(ucli_shell_t *this,
                        ucli_view_t  *view) {

    (void)lub_bintree_insert(&this->view_tree,view);
}  

/*
 * shell_new.c
 */
static void
ucli_shell_init(ucli_shell_t             *this,
                 const ucli_shell_hooks_t *hooks,
                 void                      *cookie,
                 FILE                      *istream) {

    /* initialise the tree of views */
    lub_bintree_init(&this->view_tree,
                    ucli_view_bt_offset(),
                    ucli_view_bt_compare,
                    ucli_view_bt_getkey);

    /* initialise the tree of views */
    lub_bintree_init(&this->ptype_tree,
                    ucli_ptype_bt_offset(),
                    ucli_ptype_bt_compare,
                    ucli_ptype_bt_getkey);

    assert((NULL != hooks) && (NULL != hooks->script_fn));
    
    /* set up defaults */
    this->client_hooks    = hooks;
    this->client_cookie   = cookie;
    this->view            = NULL;
    this->viewid          = NULL;
    this->global          = NULL;
    this->startup         = NULL;
    this->state           = SHELL_STATE_INITIALISING;
    this->overview        = NULL;
    ucli_shell_iterator_init(&this->iter);
    this->tinyrl          = ucli_shell_tinyrl_new(istream,
                                                   stdout,
                                                   0);
    this->current_file    = NULL;
}

ucli_shell_t *
ucli_shell_new(const ucli_shell_hooks_t *hooks,
                void                      *cookie,
                FILE                      *istream) {

    ucli_shell_t *this = malloc(sizeof(ucli_shell_t));

    if(this)
    {
        ucli_shell_init(this,hooks,cookie,istream);

        if(hooks->init_fn)
        {
            /* now call the client initialisation */
            if(BOOL_TRUE != hooks->init_fn(this))
            {
                this->state = SHELL_STATE_CLOSING;
            }
        }
    }
    return this;
}

/*
 * shell_parse.c
 */
ucli_pargv_status_t
ucli_shell_parse(const ucli_shell_t    *this,
                  const char             *line,
                  const ucli_command_t **cmd,
                  ucli_pargv_t         **pargv) {

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

bool_t
ucli_shell_pop_file(ucli_shell_t *this) {

    bool_t              result = BOOL_FALSE;
    ucli_shell_file_t *node   = this->current_file;
    
    if(node)
    {
        
        /* remove the current file from the stack... */
        this->current_file = node->next;
    
        /* and close the current file...
         */
        fclose(node->file);

        if(node->next)
        {
            /* now switch the terminal's input stream */
            tinyrl__set_istream(this->tinyrl,node->next->file);
            result = BOOL_TRUE;
        }
        /* and free up the memory */
        free(node);
    }
    return result;
}

bool_t 
ucli_shell_push_file(ucli_shell_t *this,
                      FILE          *file,
                      bool_t        stop_on_error) {

    /* allocate a control node */
    ucli_shell_file_t *node   = malloc(sizeof(ucli_shell_file_t));
    bool_t              result = BOOL_TRUE;

    if(NULL != node)
    {
        /* intialise the node */
        node->file = file;
        node->stop_on_error = stop_on_error;
        node->next = this->current_file;

        /* put the node at the top of the file stack */
        this->current_file = node;

        /* now switch the terminal's input stream */
        tinyrl__set_istream(this->tinyrl,file);

        result = BOOL_TRUE;
    }
    return result;
}

/*
 * shell_resolve_command.c
 */


const ucli_command_t *
ucli_shell_resolve_command(const ucli_shell_t *this,
                            const char          *line) {

	ucli_command_t *cmd1,*cmd2;
	
	/* search the current view and global view */
	cmd1 = ucli_view_resolve_command(this->view,line);
	cmd2 = ucli_view_resolve_command(this->global,line);

	/* choose the longest match */
	return ucli_command_choose_longest(cmd1,cmd2);
}

/*
 * shell_resolve_prefix.c
 */


const ucli_command_t *
ucli_shell_resolve_prefix(const ucli_shell_t *this,
                           const char          *line) {

	ucli_command_t *cmd1,*cmd2;
	
	/* search the current view and global view */
	cmd1 = ucli_view_resolve_prefix(this->view,line);
	cmd2 = ucli_view_resolve_prefix(this->global,line);

	/* choose the longest match */
	return ucli_command_choose_longest(cmd1,cmd2);
}

/*
 * shell_set_context.c
 */

void
ucli_shell_set_context(ucli_shell_t *this,
                        const char    *viewname) {

 	this->view   = ucli_shell_find_view(this,viewname);
 	assert(this->view);
 	assert(this->global);
}

/*
 * shell_new.c
 */




/* perform a simple tilde substitution for the home directory
 * defined in HOME
 */
static char *
ucli_shell_tilde_expand(const char *path) {

    char       *home_dir = getenv("HOME");
    char       *result   = NULL;
    const char *p        = path;
    const char *segment  = path;
    int         count    = 0;
    while(*p)
    {
        if('~' == *p)
        {
            if(count)
            {
                lub_string_catn(&result,segment,count);
                segment += (count + 1); /* skip the tilde in the path */
                count = -1;
            }
            lub_string_cat(&result,home_dir);
        }
        count++;
        p++;
    }
    if(count)
    {
        lub_string_catn(&result,segment,count);
    }
    return result;
}

void 
ucli_shell_load_files(ucli_shell_t *this) {

    const char *path = getenv("CLISH_PATH");
    char       *buffer;
    char       *dirname;
    
    if(NULL == path)
    {
        /* use the default path */
        path = default_path;
    }
    /* take a copy of the path */
    buffer = ucli_shell_tilde_expand(path);

    /* now loop though each directory */
    for(dirname = strtok(buffer,";");
        dirname;
        dirname = strtok(NULL,";"))
    {
        DIR           *dir;
        struct dirent *entry;
        
        /* search this directory for any XML files */
        dir = opendir(dirname);
        if(NULL == dir)
        {
            tinyrl_printf(this->tinyrl,
                          "*** Failed to open '%s' directory\n",
                          dirname);
            continue;
        }
        for(entry = readdir(dir);
            entry;
            entry = readdir(dir))
        {
            const char *extension = strrchr(entry->d_name,'.');
            /* check the filename */
            if(NULL != extension)
            {
                if(0 == strcmp(".xml",extension))
                {
                    char *filename = NULL;
                    
                    /* build the filename */
                    lub_string_cat(&filename,dirname);
                    lub_string_cat(&filename,"/");
                    lub_string_cat(&filename,entry->d_name);
                    
                    /* load this file */
                    /* TODO: REPLACE THIS WITH TCL SHELL AND SOURCE */
                    //(void)ucli_shell_xml_read(this,filename);
                    
                    /* release the resource */
                    lub_string_free(filename);
                }
            }
        }
        /* all done for this directory */
        closedir(dir);
    }
    /* tidy up */
    lub_string_free(buffer);
#ifdef DEBUG
        ucli_shell_dump(this);
#endif
}




/** PRIVATE TINYRL FUNCTIONS **************************************************/

/* This is a specialisation of the tinyrl_t class which maps the readline
 * functionality to the CLISH envrionment. */


static bool_t
ucli_shell_tinyrl_key_help(tinyrl_t *this,
                            int       key) {

    bool_t result = BOOL_TRUE;
    if(BOOL_TRUE == tinyrl_is_quoting(this))
    {
        /* if we are in the middle of a quote then simply enter a space */
        result = tinyrl_insert_text(this,"?");
    }
    else
    {
        /* get the context */
        context_t *context = tinyrl__get_context(this);
        
        tinyrl_crlf(this);
        tinyrl_crlf(this);
        ucli_shell_help(context->shell,tinyrl__get_line(this));
        tinyrl_crlf(this);
        tinyrl_reset_line_state(this);
    }
    /* keep the compiler happy */
    key = key;
    
    return result;
}

/* Generator function for command completion.  STATE lets us
 * know whether to start from scratch; without any state
 *  (i.e. STATE == 0), then we start at the top of the list. 
 */
/*lint -e818
  Pointer paramter 'this' could be declared as pointing to const */
static char *
ucli_shell_tinyrl_word_generator (tinyrl_t   *this,
                                   const char *line,
                                   unsigned    offset,
                                   unsigned    state) {

    /* get the context */
    context_t *context = tinyrl__get_context(this);
    
    return ucli_shell_word_generator(context->shell,
                                      line,
                                      offset,
                                      state);
}
/*lint +e818 */

/*
 * Expand the current line with any history substitutions
 */
static ucli_pargv_status_t
ucli_shell_tinyrl_expand(tinyrl_t *this) {

    ucli_pargv_status_t status = ucli_LINE_OK;
    int                  rtn;
    char                *buffer;

    /* first of all perform any history substitutions */
    rtn = tinyrl_history_expand(tinyrl__get_history(this),
                                tinyrl__get_line(this),
                                &buffer);

    switch(rtn)
    {
        case -1:
            /* error in expansion */
            status = ucli_BAD_HISTORY;
            break;
        case 0: 
            /*no expansion */
            break;
        case 1:
            /* expansion occured correctly */
            tinyrl_replace_line(this,buffer,1);
            break;
        case 2:
            /* just display line */
            printf("\n%s",buffer);
            free(buffer);
            buffer = NULL;
            break;
        default:
            break;
    }
    free(buffer);

    return status;
}

/*
 * This is a CLISH specific completion function.
 * If the current prefix is not a recognised prefix then
 * an error is flagged.
 * If it is a recognisable prefix then possible completions are displayed
 * or a unique completion is inserted.
 */
static tinyrl_match_e
ucli_shell_tinyrl_complete(tinyrl_t *this) {

    context_t     *context = tinyrl__get_context(this);
    tinyrl_match_e status;
    
    /* first of all perform any history expansion */
    (void)ucli_shell_tinyrl_expand(this);

    /* perform normal completion */
    status = tinyrl_complete(this);
    
    switch(status)
    {
        case TINYRL_NO_MATCH:
        {
            if(BOOL_FALSE == tinyrl_is_completion_error_over(this))
            {
                /* The user hasn't even entered a valid prefix!!! */
                tinyrl_crlf(this);
                ucli_shell_help(context->shell,
                                 tinyrl__get_line(this));
                tinyrl_crlf(this);
                tinyrl_reset_line_state(this);
            }
            break;
        }
        case TINYRL_MATCH:
        case TINYRL_MATCH_WITH_EXTENSIONS:
        case TINYRL_COMPLETED_MATCH:
        case TINYRL_AMBIGUOUS:
        case TINYRL_COMPLETED_AMBIGUOUS:
        {
            /* the default completion function will have prompted for completions as
             * necessary
             */
            break;
        }
    }
    return status;
}

static bool_t
ucli_shell_tinyrl_key_space(tinyrl_t *this,
                             int       key) {

    bool_t         result  = BOOL_FALSE;
    tinyrl_match_e status;
    
    if(BOOL_TRUE == tinyrl_is_quoting(this))
    {
        /* if we are in the middle of a quote then simply enter a space */
        result = tinyrl_insert_text(this," ");
    }
    else
    {
        /* perform word completion */
        status = ucli_shell_tinyrl_complete(this);

        switch(status)
        {
            case TINYRL_NO_MATCH:
            case TINYRL_AMBIGUOUS:
            {
                /* ambiguous result signal an issue */
                break;
            }
            case TINYRL_COMPLETED_AMBIGUOUS:
            {
                /* perform word completion again in case we just did case
		   modification the first time */
		status = ucli_shell_tinyrl_complete(this);
		if (status == TINYRL_MATCH_WITH_EXTENSIONS)
		{
			/* all is well with the world just enter a space */
			result = tinyrl_insert_text(this," ");
		}
                break;
            }
            case TINYRL_MATCH:
            case TINYRL_MATCH_WITH_EXTENSIONS:
            case TINYRL_COMPLETED_MATCH:
            {
                /* all is well with the world just enter a space */
                result = tinyrl_insert_text(this," ");
                break;
            }
        }
    }
    /* keep compiler happy */
    key = key;
    
    return result;
}

static bool_t
ucli_shell_tinyrl_key_enter(tinyrl_t *this,
                             int       key) {

    context_t             *context = tinyrl__get_context(this);
    const ucli_command_t *cmd = NULL;
    const char            *line = tinyrl__get_line(this);
    bool_t                 result = BOOL_FALSE;
    
    if(*line)
    {
        /* try and parse the command */
        cmd = ucli_shell_resolve_command(context->shell,line);
        if(NULL == cmd)
        {
            tinyrl_match_e status = ucli_shell_tinyrl_complete(this);
            switch(status)
            {
                case TINYRL_NO_MATCH:
                case TINYRL_AMBIGUOUS:
                case TINYRL_COMPLETED_AMBIGUOUS:
                {
                    /* failed to get a unique match... */
                    break;
                }
                case TINYRL_MATCH:
                case TINYRL_MATCH_WITH_EXTENSIONS:
                case TINYRL_COMPLETED_MATCH:
                {
                    /* re-fetch the line as it may have changed
                     * due to auto-completion
                     */
                    line = tinyrl__get_line(this);
                    /* get the command to parse? */
                    cmd = ucli_shell_resolve_command(context->shell,line);
                    if (NULL == cmd)
                    {
                        /*
                         * We have had a match but it is not a command
                         * so add a space so as not to confuse the user
                         */
                        result = tinyrl_insert_text(this," ");
                    }
                    break;
                }
            }
        }
        if(NULL != cmd)
        {
            ucli_pargv_status_t arg_status;
            tinyrl_crlf(this);
            /* we've got a command so check the syntax */
            arg_status = ucli_shell_parse(context->shell,
                                       line,
                                       &context->command,
                                       &context->pargv);
            switch(arg_status)
            {
                case ucli_LINE_OK:
                    tinyrl_done(this);
                    result = BOOL_TRUE;
                    break;
                case ucli_BAD_HISTORY:
                case ucli_BAD_CMD:
                case ucli_BAD_PARAM:
                   tinyrl_crlf(this);
                   tinyrl_reset_line_state(this);
                    break;
            }
        }
    }
    else
    {
        /* nothing to pass simply move down the screen */
        tinyrl_crlf(this);
        tinyrl_reset_line_state(this);
        result = BOOL_TRUE;
    }
    if((BOOL_FALSE == result) && (BOOL_FALSE == tinyrl__get_isatty(this)))
    {
        /* we've found an error in a script */
        context->shell->state = SHELL_STATE_SCRIPT_ERROR;
    }
    /* keep the compiler happy */
    key = key;
    return result;
}

/* This is the completion function provided for CLISH */
static tinyrl_completion_func_t ucli_shell_tinyrl_completion;
static char **
ucli_shell_tinyrl_completion(tinyrl_t   *this,
                              const char *line,
                              unsigned    start,
                              unsigned    end) {

    char **matches;
    
    /* don't bother to resort to filename completion */
    tinyrl_completion_over(this);

    /* perform the matching */
    matches = tinyrl_completion(this,
                                line,
                                start,
                                end,
                                ucli_shell_tinyrl_word_generator);
    return matches;
}

static void
ucli_shell_tinyrl_init(tinyrl_t *this) {
   
    bool_t status;    
    /* bind the '?' key to the help function */
    status = tinyrl_bind_key(this,'?', ucli_shell_tinyrl_key_help);
    assert(BOOL_TRUE == status);

    /* bind the <RET> key to the help function */
    status = tinyrl_bind_key(this,'\r', ucli_shell_tinyrl_key_enter);
    assert(BOOL_TRUE == status);
    status = tinyrl_bind_key(this,'\n', ucli_shell_tinyrl_key_enter);
    assert(BOOL_TRUE == status);
    
    /* bind the <SPACE> key to auto-complete if necessary */
    status = tinyrl_bind_key(this,' ', ucli_shell_tinyrl_key_space);
    assert(BOOL_TRUE == status);
    }

/* 
 * Create an instance of the specialised class
 */
tinyrl_t *
ucli_shell_tinyrl_new(FILE    *istream,
                       FILE    *ostream,
                       unsigned stifle) {

    /* call the parent constructor */
    tinyrl_t *this = tinyrl_new(istream,
                                ostream,
                                stifle,
                                ucli_shell_tinyrl_completion);
    if(NULL != this)
    {
        /* now call our own constructor */
        ucli_shell_tinyrl_init(this);
    }
    return this;
}

void
ucli_shell_tinyrl_fini(tinyrl_t *this) {

    /* nothing to do... yet */
    this=this;
}

void
ucli_shell_tinyrl_delete(tinyrl_t *this) {

    /* call our destructor */
    ucli_shell_tinyrl_fini(this);
    
    /* and call the parent destructor */
    tinyrl_delete(this);
}


bool_t
ucli_shell_readline(ucli_shell_t          *this,
                     const char             *prompt,
                     const ucli_command_t **cmd,
                     ucli_pargv_t         **pargv) {

    char         *line   = NULL;
    bool_t        result = BOOL_FALSE;
    context_t     context;
    
    /* set up the context */
    context.command     = NULL;
    context.pargv       = NULL;
    context.shell       = this;
    
    if(SHELL_STATE_CLOSING != this->state)
    {
        this->state = SHELL_STATE_READY;

        line = tinyrl_readline(this->tinyrl,prompt,&context);
        if(NULL != line)
        {
            tinyrl_history_t *history = tinyrl__get_history(this->tinyrl);

            if(tinyrl__get_isatty(this->tinyrl))
            {
                /* deal with the history list */
                tinyrl_history_add(history,line);
            }
            if(this->client_hooks->cmd_line_fn)
            {
                /* now let the client know the command line has been entered */
                this->client_hooks->cmd_line_fn(this,line);
            }
            free(line);
            result = BOOL_TRUE;
            *cmd   = context.command;
            *pargv = context.pargv;
        }
    }
    return result;
}

/** SHELL STARTUP AND INITIALIZATION ******************************************/

/* This provides the thread of execution for a shell instance */
static void *
ucli_shell_thread(void *arg) {

    context_t     *context = arg;
    bool_t         running = BOOL_TRUE;
    ucli_shell_t *this;
    int            last_type;

    /* make sure we can only be cancelled at controlled points */
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,&last_type);
    /* register a cancellation handler */
    pthread_cleanup_push((void(*)(void*))ucli_shell_cleanup,context);
    
    /* create a shell object... */
    this = context->shell = ucli_shell_new(context->hooks,
                                            context->cookie,
                                            context->istream);
    assert(this);

    /*
     * Check the shell isn't closing down
     */
    if(this && (SHELL_STATE_CLOSING != this->state))
    {
        /*
         * load the XML files found in the current CLISH path 
         */
        ucli_shell_load_files(this);

        /* start off with the default inputs stream */
        (void)ucli_shell_push_file(this,
                                    fdopen(fileno(context->istream),"r"),
                                    BOOL_TRUE);
        
        /* This is the starting point for a shell, if this fails then 
         * we cannot start the shell...
         */
        running = ucli_shell_startup(this);
       
        pthread_testcancel();
            
        /* Loop reading and executing lines until the user quits. */
        while(running)
        {
            const ucli_command_t *cmd;
            const ucli_view_t    *view;

            if((SHELL_STATE_SCRIPT_ERROR == this->state) && 
               (BOOL_TRUE == tinyrl__get_isatty(this->tinyrl)))
            {
                /* interactive session doesn't automatically exit on error */
                this->state = SHELL_STATE_READY;
            }
            /* only bother to read the next line if there hasn't been a script error */
            if(this->state != SHELL_STATE_SCRIPT_ERROR)
            {
                /* obtain the prompt */	
                view = ucli_shell__get_view(this);
                assert(view);
                
                context->prompt = ucli_view__get_prompt(view,ucli_shell__get_viewid(this));
                assert(context->prompt);

                /* get input from the user */
                running = ucli_shell_readline(this,context->prompt,&cmd,&context->pargv);
                lub_string_free(context->prompt);

                context->prompt = NULL;
                
                if(running && cmd && context->pargv)
                {
                    /* execute the provided command */
                    if(BOOL_FALSE == ucli_shell_execute(this,cmd,&context->pargv))
                    {
                        /* there was an error */
                        tinyrl_ding(this->tinyrl);

                        /* 
                         * what we do now depends on whether we are set up to
                         * stop on error on not.
                         */
                        if((BOOL_TRUE == this->current_file->stop_on_error) &&
                        (BOOL_FALSE == tinyrl__get_isatty(this->tinyrl)))
                        {
                            this->state = SHELL_STATE_SCRIPT_ERROR;
                        }
                    }
                }
            }
            if((BOOL_FALSE == running) || 
               (this->state == SHELL_STATE_SCRIPT_ERROR))
            {
                /* we've reached the end of a file (or a script error has occured)
                 * unwind the file stack to see whether 
                 * we need to exit
                 */
                running = ucli_shell_pop_file(this);
            }
            /* test for cancellation */
            pthread_testcancel();
        }
    }
    /* be a good pthread citizen */
    pthread_cleanup_pop(1);

    return (void*)BOOL_TRUE;
}

static context_t *
_ucli_shell_spawn(const pthread_attr_t      *attr,
                   const ucli_shell_hooks_t *hooks,
                   void                      *cookie,
                   FILE                      *istream) {

    int rtn;
    context_t *context = malloc(sizeof(context_t));
    assert(context);
    
    if(context)
    {
        context->hooks   = hooks;
        context->cookie  = cookie;
        context->istream = istream;
        context->shell   = NULL;
        context->prompt  = NULL;
        context->pargv   = NULL;
        
        /* and set it free */
        rtn = pthread_create(&context->pthread,
                             attr,
                             ucli_shell_thread,
                             context);
        if(0 != rtn)
        {
            free(context);
            context = NULL;
        }
    }
    return context;
}

static int
_ucli_shell_spawn_and_wait(const ucli_shell_hooks_t *hooks,
                            void                      *cookie,
                            FILE                      *file) {

    void      *result = NULL;
    context_t *context = _ucli_shell_spawn(NULL,hooks,cookie,file);
    
    if(context)
    {
        /* join the shell's thread and wait for it to exit */
        (void)pthread_join(context->pthread,&result);
    }
    return (int)result;
}

int 
ucli_shell_spawn_and_wait(const ucli_shell_hooks_t *hooks,
                           void                      *cookie) {

    return _ucli_shell_spawn_and_wait(hooks,cookie,stdin);
}

bool_t
ucli_shell_spawn(pthread_t                 *pthread,
                  const pthread_attr_t      *attr,
                  const ucli_shell_hooks_t *hooks,
                  void                      *cookie) {

    context_t *context;
    bool_t     result = BOOL_FALSE;
    
    /* spawn the thread... */
    context = _ucli_shell_spawn(attr,hooks,cookie,stdin);
   
    if(NULL != context)
    {
        result   = BOOL_TRUE;
        if(NULL != pthread)
        {
            *pthread = context->pthread;
        }
    }
    return result;
}

bool_t
ucli_shell_spawn_from_file(const ucli_shell_hooks_t *hooks,
                            void                      *cookie,
                            const char                *filename) {

    bool_t result = BOOL_FALSE;
    if(NULL != filename)
    {
        FILE *file = fopen(filename,"r");
        if (NULL != file)
        {
            /* spawn the thread and wait for it to exit */
            result = _ucli_shell_spawn_and_wait(hooks,cookie,file) ? BOOL_TRUE : BOOL_FALSE;
    
            fclose(file);
        }
    }
    return result;
}

/*
 * shell_startup.c
 */



bool_t
ucli_shell_startup(ucli_shell_t *this) {

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


/** PRIVATE CLEANUP FUNCTIONS *************************************************/

/* This is invoked when the thread ends or is cancelled. */
static void
ucli_shell_cleanup(context_t *context) {

#ifdef __vxworks
    int last_state;
    /* we need to avoid recursion issues that exit in VxWorks */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&last_state);
#endif /* __vxworks */
    
    if(context->shell)
    {
        /*
         * Time to kill off the instance and terminate the thread 
         */
        ucli_shell_delete(context->shell);
        context->shell = NULL;
    }
    if(context->pargv)
    {
        ucli_pargv_delete(context->pargv);
        context->pargv = NULL;
    }
    if(context->prompt)
    {
        lub_string_free(context->prompt);
        context->prompt = NULL;
    }

    /* finished with this */
    free(context);
#ifdef __vxworks
    pthread_setcancelstate(last_state,&last_state);
#endif /* __vxworks */
}


static void
ucli_shell_fini(ucli_shell_t *this) {

	ucli_view_t  *view;
	ucli_ptype_t *ptype;
	

	/* delete each view held  */
	while((view = lub_bintree_findfirst(&this->view_tree)))
	{
		/* remove the command from the tree */
		lub_bintree_remove(&this->view_tree,view);

		/* release the instance */
		ucli_view_delete(view);
	}

	/* delete each ptype held  */
	while((ptype = lub_bintree_findfirst(&this->ptype_tree)))
	{
		/* remove the command from the tree */
		lub_bintree_remove(&this->ptype_tree,ptype);

		/* release the instance */
		ucli_ptype_delete(ptype);
	}
    /* free the textual details */
    lub_string_free(this->overview);
    lub_string_free(this->viewid);
    
	if(NULL != this->startup)
    {
        /* remove the startup command */
        ucli_command_delete(this->startup);
    }
    /* clean up the file stack */
    while(BOOL_TRUE == ucli_shell_pop_file(this))
    {
        /* not alot do do here */
    }
    /* delete the tinyrl object */
    ucli_shell_tinyrl_delete(this->tinyrl);
    

}

void
ucli_shell_delete(ucli_shell_t *this) {

    if(this->client_hooks->fini_fn)
    {
        /* now call the client finalisation */
        this->client_hooks->fini_fn(this);
    }
    
    ucli_shell_fini(this);

    free(this);
}



/** DEBUGGING *****************************************************************/

void
ucli_shell_dump(ucli_shell_t *this) {

    ucli_view_t             *v;
    ucli_ptype_t            *t;	
    lub_bintree_iterator_t   iter;

    lub_dump_printf("shell(%p)\n",this);
    lub_dump_printf("OVERVIEW:\n%s",this->overview);
    lub_dump_indent();
    
    v = lub_bintree_findfirst(&this->view_tree);
    
    /* iterate the tree of views */
    for(lub_bintree_iterator_init(&iter,&this->view_tree,v);
        v;
        v=lub_bintree_iterator_next(&iter))
    {
        ucli_view_dump(v);
    }

    /* iterate the tree of types */
    t = lub_bintree_findfirst(&this->ptype_tree);
    for(lub_bintree_iterator_init(&iter,&this->ptype_tree,t);
        t;
        t=lub_bintree_iterator_next(&iter))
    {
        ucli_ptype_dump(t);
    }
    lub_dump_undent();
}

