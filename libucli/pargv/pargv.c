/*
 * paramv.c
 */
#include "private.h"
#include "lub/string.h"
#include "lub/argv.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*--------------------------------------------------------- */
/*
 * Search for the specified parameter and return its value
 */
static ucli_parg_t *
find_parg(ucli_pargv_t *this,
          const char    *name)
{
    unsigned      i;
    ucli_parg_t *result = NULL;
    
    /* scan the parameters in this instance */
    for(i = 0;
        i < this->pargc;
        i++)
    {
        ucli_parg_t *parg  = &this->pargs[i];
        const char *pname = ucli_param__get_name(parg->param);

        if (0==strcmp(pname,name))
        {
            result = parg;
            break;
        }
    }
    return result;
}
/*--------------------------------------------------------- */
static void
insert_parg(ucli_pargv_t       *this,
            const ucli_param_t *param,
            const char          *value)
{
    ucli_parg_t *parg = find_parg(this,ucli_param__get_name(param));
    
    if(NULL != parg)
    {
        /* release the current value */
        lub_string_free(parg->value);
    }
    else
    {
        /* use the current insertion point */
        parg        = &this->pargs[this->pargc++];
        parg->param = param;
    }
    parg->value = lub_string_dup(value);
}
/*--------------------------------------------------------- */
static void
set_defaults(ucli_pargv_t         *this,
             const ucli_command_t *cmd)
{
    unsigned             index = 0;
    const ucli_param_t *param;

    /* scan through all the parameters for this command */
    while( (param = ucli_command__get_param(cmd,index++)) )
    {
        const char *defval = ucli_param__get_default(param);
        if(NULL != defval)
        {
            if('\0' != *defval)
            {
                /* add the translated value to the vector */
                char* translated = 
                    ucli_ptype_translate(ucli_param__get_ptype(param),
                                      defval);
                insert_parg(this,param,translated);
                lub_string_free(translated);
            }
            else
            {
                /* insert the empty default */
                insert_parg(this,param,defval);
            }
                
        }
    }
}
/*--------------------------------------------------------- */
static ucli_pargv_status_t
ucli_pargv_init(ucli_pargv_t         *this,
                 const ucli_command_t *cmd,
                 const lub_argv_t      *argv)
{
    const ucli_param_t *param;
    unsigned             start = lub_argv_wordcount(ucli_command__get_name(cmd));
    unsigned             argc  = lub_argv__get_count(argv);
    unsigned             index = 0;
    unsigned             i;

    /* 
     * setup so that iteration will work during construction
     */
    this->pargc = 0;

    /* setup any defaulted parameters */
    set_defaults(this,cmd);

    /*
     * Skipping the command contribution to the line
     * check each argument against the parameter details 
     * NB we start at the first non-command related argument
     */
    for(i = start;
        i < argc;
        i++)
    {
        const char *arg = lub_argv__get_arg(argv,i);

        /* is this an option identifier? */
        param = ucli_command_find_option(cmd,arg);
        if(NULL != param)
        {
            /*
             * Does it have a trailing argument for the value ?
             */
            if(NULL != ucli_param__get_ptype(param))
            {
                i++; /* skip to the value part */
                arg = lub_argv__get_arg(argv,i);
                if(NULL == arg)
                {
                    const char *prefix = ucli_param__get_prefix(param);
                    size_t      offset = lub_argv__get_offset(argv,i-1);

                    offset += strlen(prefix) + 1;
                    ucli_param_help(param,offset);
                    return ucli_BAD_PARAM;
                }
            }
            else
            {
                /* just insert the flag */
                arg = NULL;
            }
        }
        else
        {
            /* not an option identifier, treat as next non-option argument */
            param = ucli_command_next_non_option(cmd,&index);
        }

        if(NULL != param)
        {
            /* validate the new value */
            char *validated = arg ? ucli_param_validate(param,arg) : NULL;
            if(!arg || validated)
            {
                /* add (or update) this parameter */
                insert_parg(this,param,validated);
                lub_string_free(validated);
            }
            else
            {
                ucli_param_help(param,lub_argv__get_offset(argv,i));
                return ucli_BAD_PARAM;
            }
        }
        else
        {
            /* 
             * if we've satisfied all the parameters we can now construct
             * an 'args' parameter if one exists
             */
            if(NULL != (param = ucli_command__get_args(cmd)))
            {
                /* 
                 * put all the argument into a single string 
                 */
                char *args = NULL;
                while(NULL != arg)
                {
                    bool_t quoted = lub_argv__get_quoted(argv,i);
                    if(BOOL_TRUE == quoted) 
                    {
                        lub_string_cat(&args,"\"");
                    }
                    /* place the current argument in the string */
                    lub_string_cat(&args,arg);
                    if(BOOL_TRUE == quoted) 
                    {
                        lub_string_cat(&args,"\"");
                    }
                    i++;
                    arg = lub_argv__get_arg(argv,i);
                    if(NULL != arg)
                    {
                        /* add a space if there are more arguments */
                        lub_string_cat(&args," ");
                    }
                }
                /* add (or update) this parameter */
                insert_parg(this,param,args);
                lub_string_free(args);
            }
            else
            {
                printf("%*c\n",lub_argv__get_offset(argv,i),'^');
                return ucli_BAD_PARAM;
            }
        }
    }
    /* now check that any futher non-option parameters have got values */
    while( (param = ucli_command_next_non_option(cmd,&index)) )
    {
        if(NULL == find_parg(this,ucli_param__get_name(param)))
        {
            /* failed to construct a valid command line */
            size_t offset = lub_argv__get_offset(argv,i-1);
            offset += strlen(lub_argv__get_arg(argv,i-1)) + 1;
            ucli_param_help(param,offset);
            return ucli_BAD_PARAM;
        }
    }
    return ucli_LINE_OK;
}
/*--------------------------------------------------------- */
ucli_pargv_t *
ucli_pargv_new(const ucli_command_t *cmd,
                const char            *line,
                size_t                 offset,
                ucli_pargv_status_t  *status)
{
    ucli_pargv_t *this;
    lub_argv_t   *argv       = lub_argv_new(line,offset);
    unsigned      max_params = lub_argv__get_count(argv);
    unsigned      cmd_params = ucli_command__get_param_count(cmd);
    size_t        size;

    if(max_params < cmd_params)
    {
        max_params = cmd_params;
    }

    /* -1 to account for the ucli_parg_t present in ucli_pargv_t */
    size = sizeof(ucli_parg_t)*(max_params-1) + sizeof(ucli_pargv_t);

    this = malloc(size);

    if(NULL != this)
    {
        *status = ucli_pargv_init(this,cmd,argv);
        switch(*status)
        {
            case ucli_LINE_OK:
                break;
            case ucli_BAD_CMD:
            case ucli_BAD_PARAM:
            case ucli_BAD_HISTORY:
                /* commit suicide */
                ucli_pargv_delete(this);
                this = NULL;
                break;
        }
    }

    /* cleanup */
    lub_argv_delete(argv);

    return this;
}
/*--------------------------------------------------------- */
static void
ucli_pargv_fini(ucli_pargv_t *this)
{
    unsigned i;
    
    /* cleanup time */
    for(i = 0; 
        i < this->pargc;
        i++)
    {
        lub_string_free(this->pargs[i].value);
        this->pargs[i].value = NULL;
    }
}
/*--------------------------------------------------------- */
void
ucli_pargv_delete(ucli_pargv_t *this)
{
    ucli_pargv_fini(this);
    free(this);
}
/*--------------------------------------------------------- */
const char *
ucli_parg__get_value(const ucli_parg_t *this)
{
    return this->value;
}
/*--------------------------------------------------------- */
const char *
ucli_parg__get_name(const ucli_parg_t *this)
{
    return ucli_param__get_name(this->param);
}
/*--------------------------------------------------------- */
const ucli_ptype_t *
ucli_parg__get_ptype(const ucli_parg_t *this)
{
    return ucli_param__get_ptype(this->param);
}
/*--------------------------------------------------------- */
const ucli_parg_t *
ucli_pargv_find_arg(ucli_pargv_t *this,
                   const char  *name)
{
    return find_parg(this,name);
}
/*--------------------------------------------------------- */
