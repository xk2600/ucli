/*
 * paramv.c
 */

#include "libucli/pargv.h"
#include "libucli/param.h"
#include "lub/string.h"
#include "lub/argv.h"
#include "lub/dump.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/** PRIVATE VARIABLES ********************************************************/

struct ucli_parg_s {

    const ucli_param_t *param;
    char                *value;
};
struct ucli_pargv_s {

    unsigned     pargc;
    ucli_parg_t pargs[1];
};




/** PUBLIC METHODS ***********************************************************/

/* Search for the specified parameter and return its value */
static ucli_parg_t *
find_parg(ucli_pargv_t *instance,
          const char    *name) {

    unsigned      i;
    ucli_parg_t *result = NULL;
    
    /* scan the parameters in instance instance */
    for(i = 0;
        i < instance->pargc;
        i++) {    

        ucli_parg_t *parg  = &instance->pargs[i];
        const char *pname = ucli_param__get_name(parg->param);

        if (0==strcmp(pname,name)) {        

            result = parg;
            break;
        }
    }
    return result;
}

static void
insert_parg(ucli_pargv_t       *instance,
            const ucli_param_t *param,
            const char          *value) {

    ucli_parg_t *parg = find_parg(instance,ucli_param__get_name(param));
    
    if(NULL != parg) {    

        /* release the current value */
        lub_string_free(parg->value);
    }
    else {    

        /* use the current insertion point */
        parg        = &instance->pargs[instance->pargc++];
        parg->param = param;
    }
    parg->value = lub_string_dup(value);
}

static void
set_defaults(ucli_pargv_t         *instance,
             const ucli_command_t *cmd) {

    unsigned             index = 0;
    const ucli_param_t *param;

    /* scan through all the parameters for instance command */
    while( (param = ucli_command__get_param(cmd,index++)) ) {    

        const char *defval = ucli_param__get_default(param);
        if(NULL != defval) {        

            if('\0' != *defval) {            

                /* add the translated value to the vector */
                char* translated = 
                    ucli_ptype_translate(ucli_param__get_ptype(param),
                                      defval);
                insert_parg(instance,param,translated);
                lub_string_free(translated);
            }
            else {            

                /* insert the empty default */
                insert_parg(instance,param,defval);
            }
                
        }
    }
}

static ucli_pargv_status_t
ucli_pargv_init(ucli_pargv_t         *instance,
                 const ucli_command_t *cmd,
                 const lub_argv_t      *argv) {

    const ucli_param_t *param;
    unsigned             start = lub_argv_wordcount(ucli_command__get_name(cmd));
    unsigned             argc  = lub_argv__get_count(argv);
    unsigned             index = 0;
    unsigned             i;

    /* 
     * setup so that iteration will work during construction
     */
    instance->pargc = 0;

    /* setup any defaulted parameters */
    set_defaults(instance,cmd);

    /*
     * Skipping the command contribution to the line
     * check each argument against the parameter details 
     * NB we start at the first non-command related argument
     */
    for(i = start;
        i < argc;
        i++) {    

        const char *arg = lub_argv__get_arg(argv,i);

        /* is instance an option identifier? */
        param = ucli_command_find_option(cmd,arg);
        if(NULL != param) {        

            /*
             * Does it have a trailing argument for the value ?
             */
            if(NULL != ucli_param__get_ptype(param)) {            

                i++; /* skip to the value part */
                arg = lub_argv__get_arg(argv,i);
                if(NULL == arg) {                

                    const char *prefix = ucli_param__get_prefix(param);
                    size_t      offset = lub_argv__get_offset(argv,i-1);

                    offset += strlen(prefix) + 1;
                    ucli_param_help(param,offset);
                    return ucli_BAD_PARAM;
                }
            }
            else {            

                /* just insert the flag */
                arg = NULL;
            }
        }
        else {        

            /* not an option identifier, treat as next non-option argument */
            param = ucli_command_next_non_option(cmd,&index);
        }

        if(NULL != param) {        

            /* validate the new value */
            char *validated = arg ? ucli_param_validate(param,arg) : NULL;
            if(!arg || validated) {            

                /* add (or update) instance parameter */
                insert_parg(instance,param,validated);
                lub_string_free(validated);
            }
            else {            

                ucli_param_help(param,lub_argv__get_offset(argv,i));
                return ucli_BAD_PARAM;
            }
        }
        else {        

            /* 
             * if we've satisfied all the parameters we can now construct
             * an 'args' parameter if one exists
             */
            if(NULL != (param = ucli_command__get_args(cmd))) {            

                /* 
                 * put all the argument into a single string 
                 */
                char *args = NULL;
                while(NULL != arg) {                

                    bool_t quoted = lub_argv__get_quoted(argv,i);
                    if(BOOL_TRUE == quoted)  {                    

                        lub_string_cat(&args,"\"");
                    }
                    /* place the current argument in the string */
                    lub_string_cat(&args,arg);
                    if(BOOL_TRUE == quoted)  {                    

                        lub_string_cat(&args,"\"");
                    }
                    i++;
                    arg = lub_argv__get_arg(argv,i);
                    if(NULL != arg) {                    

                        /* add a space if there are more arguments */
                        lub_string_cat(&args," ");
                    }
                }
                /* add (or update) instance parameter */
                insert_parg(instance,param,args);
                lub_string_free(args);
            }
            else {            

                printf("%*c\n",lub_argv__get_offset(argv,i),'^');
                return ucli_BAD_PARAM;
            }
        }
    }
    /* now check that any futher non-option parameters have got values */
    while( (param = ucli_command_next_non_option(cmd,&index)) ) {    

        if(NULL == find_parg(instance,ucli_param__get_name(param))) {        

            /* failed to construct a valid command line */
            size_t offset = lub_argv__get_offset(argv,i-1);
            offset += strlen(lub_argv__get_arg(argv,i-1)) + 1;
            ucli_param_help(param,offset);
            return ucli_BAD_PARAM;
        }
    }
    return ucli_LINE_OK;
}

ucli_pargv_t *
ucli_pargv_new(const ucli_command_t *cmd,
                const char            *line,
                size_t                 offset,
                ucli_pargv_status_t  *status) {

    ucli_pargv_t *instance;
    lub_argv_t   *argv       = lub_argv_new(line,offset);
    unsigned      max_params = lub_argv__get_count(argv);
    unsigned      cmd_params = ucli_command__get_param_count(cmd);
    size_t        size;

    if(max_params < cmd_params) {    

        max_params = cmd_params;
    }

    /* -1 to account for the ucli_parg_t present in ucli_pargv_t */
    size = sizeof(ucli_parg_t)*(max_params-1) + sizeof(ucli_pargv_t);

    instance = malloc(size);

    if(NULL != instance) {    

        *status = ucli_pargv_init(instance,cmd,argv);
        switch(*status) {        

            case ucli_LINE_OK:
                break;
            case ucli_BAD_CMD:
            case ucli_BAD_PARAM:
            case ucli_BAD_HISTORY:
                /* commit suicide */
                ucli_pargv_delete(instance);
                instance = NULL;
                break;
        }
    }

    /* cleanup */
    lub_argv_delete(argv);

    return instance;
}


static void
ucli_pargv_fini(ucli_pargv_t *instance) {

    unsigned i;
    
    /* cleanup time */
    for(i = 0; 
        i < instance->pargc;
        i++) {    

        lub_string_free(instance->pargs[i].value);
        instance->pargs[i].value = NULL;
    }
}

void
ucli_pargv_delete(ucli_pargv_t *instance) {

    ucli_pargv_fini(instance);
    free(instance);
}

const char *
ucli_parg__get_value(const ucli_parg_t *instance) {

    return instance->value;
}

const char *
ucli_parg__get_name(const ucli_parg_t *instance) {

    return ucli_param__get_name(instance->param);
}

const ucli_ptype_t *
ucli_parg__get_ptype(const ucli_parg_t *instance) {

    return ucli_param__get_ptype(instance->param);
}

const ucli_parg_t *
ucli_pargv_find_arg(ucli_pargv_t *instance,
                   const char  *name) {

    return find_parg(instance,name);
}



/** DEBUGGING *****************************************************************/

void
ucli_parg_dump(const ucli_parg_t *instance) {

    lub_dump_printf("parg(%p)\n",instance);
    lub_dump_indent();
    lub_dump_printf("name : %s\n",ucli_parg__get_name(instance));
    lub_dump_printf("ptype: %s\n",
                    ucli_ptype__get_name(ucli_parg__get_ptype(instance)));
    lub_dump_printf("value: %s\n",ucli_parg__get_value(instance));
    lub_dump_undent();
}

void
ucli_pargv_dump(const ucli_pargv_t *instance) {

    unsigned i;

    lub_dump_printf("pargv(%p)\n",instance);
    lub_dump_indent();
    for(i =0; i < instance->pargc; i++) {    

        /* get the appropriate parameter definition */
        ucli_parg_dump(&instance->pargs[i]);
    }
    lub_dump_undent();
}

