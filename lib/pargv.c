/*
 * paramv.c
 */

#include "lib/pargv.h"
#include "lib/param.h"
#include "lub/string.h"
#include "lub/argv.h"
#include "lub/dump.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/** PRIVATE VARIABLES ********************************************************/

struct clish_parg_s {

    const clish_param_t *param;
    char                *value;
};
struct clish_pargv_s {

    unsigned     pargc;
    clish_parg_t pargs[1];
};




/** PUBLIC METHODS ***********************************************************/

/* Search for the specified parameter and return its value */
static clish_parg_t *
find_parg(clish_pargv_t *instance,
          const char    *name) {

    unsigned      i;
    clish_parg_t *result = NULL;
    
    /* scan the parameters in instance instance */
    for(i = 0;
        i < instance->pargc;
        i++) {    

        clish_parg_t *parg  = &instance->pargs[i];
        const char *pname = clish_param__get_name(parg->param);

        if (0==strcmp(pname,name)) {        

            result = parg;
            break;
        }
    }
    return result;
}

static void
insert_parg(clish_pargv_t       *instance,
            const clish_param_t *param,
            const char          *value) {

    clish_parg_t *parg = find_parg(instance,clish_param__get_name(param));
    
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
set_defaults(clish_pargv_t         *instance,
             const clish_command_t *cmd) {

    unsigned             index = 0;
    const clish_param_t *param;

    /* scan through all the parameters for instance command */
    while( (param = clish_command__get_param(cmd,index++)) ) {    

        const char *defval = clish_param__get_default(param);
        if(NULL != defval) {        

            if('\0' != *defval) {            

                /* add the translated value to the vector */
                char* translated = 
                    clish_ptype_translate(clish_param__get_ptype(param),
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

static clish_pargv_status_t
clish_pargv_init(clish_pargv_t         *instance,
                 const clish_command_t *cmd,
                 const lub_argv_t      *argv) {

    const clish_param_t *param;
    unsigned             start = lub_argv_wordcount(clish_command__get_name(cmd));
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
        param = clish_command_find_option(cmd,arg);
        if(NULL != param) {        

            /*
             * Does it have a trailing argument for the value ?
             */
            if(NULL != clish_param__get_ptype(param)) {            

                i++; /* skip to the value part */
                arg = lub_argv__get_arg(argv,i);
                if(NULL == arg) {                

                    const char *prefix = clish_param__get_prefix(param);
                    size_t      offset = lub_argv__get_offset(argv,i-1);

                    offset += strlen(prefix) + 1;
                    clish_param_help(param,offset);
                    return clish_BAD_PARAM;
                }
            }
            else {            

                /* just insert the flag */
                arg = NULL;
            }
        }
        else {        

            /* not an option identifier, treat as next non-option argument */
            param = clish_command_next_non_option(cmd,&index);
        }

        if(NULL != param) {        

            /* validate the new value */
            char *validated = arg ? clish_param_validate(param,arg) : NULL;
            if(!arg || validated) {            

                /* add (or update) instance parameter */
                insert_parg(instance,param,validated);
                lub_string_free(validated);
            }
            else {            

                clish_param_help(param,lub_argv__get_offset(argv,i));
                return clish_BAD_PARAM;
            }
        }
        else {        

            /* 
             * if we've satisfied all the parameters we can now construct
             * an 'args' parameter if one exists
             */
            if(NULL != (param = clish_command__get_args(cmd))) {            

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
                return clish_BAD_PARAM;
            }
        }
    }
    /* now check that any futher non-option parameters have got values */
    while( (param = clish_command_next_non_option(cmd,&index)) ) {    

        if(NULL == find_parg(instance,clish_param__get_name(param))) {        

            /* failed to construct a valid command line */
            size_t offset = lub_argv__get_offset(argv,i-1);
            offset += strlen(lub_argv__get_arg(argv,i-1)) + 1;
            clish_param_help(param,offset);
            return clish_BAD_PARAM;
        }
    }
    return clish_LINE_OK;
}

clish_pargv_t *
clish_pargv_new(const clish_command_t *cmd,
                const char            *line,
                size_t                 offset,
                clish_pargv_status_t  *status) {

    clish_pargv_t *instance;
    lub_argv_t   *argv       = lub_argv_new(line,offset);
    unsigned      max_params = lub_argv__get_count(argv);
    unsigned      cmd_params = clish_command__get_param_count(cmd);
    size_t        size;

    if(max_params < cmd_params) {    

        max_params = cmd_params;
    }

    /* -1 to account for the clish_parg_t present in clish_pargv_t */
    size = sizeof(clish_parg_t)*(max_params-1) + sizeof(clish_pargv_t);

    instance = malloc(size);

    if(NULL != instance) {    

        *status = clish_pargv_init(instance,cmd,argv);
        switch(*status) {        

            case clish_LINE_OK:
                break;
            case clish_BAD_CMD:
            case clish_BAD_PARAM:
            case clish_BAD_HISTORY:
                /* commit suicide */
                clish_pargv_delete(instance);
                instance = NULL;
                break;
        }
    }

    /* cleanup */
    lub_argv_delete(argv);

    return instance;
}


static void
clish_pargv_fini(clish_pargv_t *instance) {

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
clish_pargv_delete(clish_pargv_t *instance) {

    clish_pargv_fini(instance);
    free(instance);
}

const char *
clish_parg__get_value(const clish_parg_t *instance) {

    return instance->value;
}

const char *
clish_parg__get_name(const clish_parg_t *instance) {

    return clish_param__get_name(instance->param);
}

const clish_ptype_t *
clish_parg__get_ptype(const clish_parg_t *instance) {

    return clish_param__get_ptype(instance->param);
}

const clish_parg_t *
clish_pargv_find_arg(clish_pargv_t *instance,
                   const char  *name) {

    return find_parg(instance,name);
}



/** DEBUGGING *****************************************************************/

void
clish_parg_dump(const clish_parg_t *instance) {

    lub_dump_printf("parg(%p)\n",instance);
    lub_dump_indent();
    lub_dump_printf("name : %s\n",clish_parg__get_name(instance));
    lub_dump_printf("ptype: %s\n",
                    clish_ptype__get_name(clish_parg__get_ptype(instance)));
    lub_dump_printf("value: %s\n",clish_parg__get_value(instance));
    lub_dump_undent();
}

void
clish_pargv_dump(const clish_pargv_t *instance) {

    unsigned i;

    lub_dump_printf("pargv(%p)\n",instance);
    lub_dump_indent();
    for(i =0; i < instance->pargc; i++) {    

        /* get the appropriate parameter definition */
        clish_parg_dump(&instance->pargs[i]);
    }
    lub_dump_undent();
}

