/*
 * param.c
 *
 * This file provides the implementation of the "param" class  
 */
#include "clish/param.h"
#include "lub/dump.h"
#include "lub/string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/** PRIVATE TYPES *************************************************************/
struct clish_param_s {

    char          *name;
    char          *text;
    clish_ptype_t *ptype;  /* The type of instance parameter */ 
    char          *prefix; /* if non NULL the name of the identifying switch parameter */
    char          *defval; /* default value to use for instance parameter */
};



/** PRIVATE TYPES *************************************************************/
/*---------------------------------------------------------
 * PRIVATE METHODS
 *--------------------------------------------------------- */
static void
clish_param_init(clish_param_t *instance,
                 const char    *name,
                 const char    *text,
                 clish_ptype_t *ptype) {


    /* initialise the help part */
    instance->name  = lub_string_dup(name);
    instance->text  = lub_string_dup(text);
    instance->ptype = ptype;

    /* set up defaults */
    instance->prefix = NULL;
    instance->defval = NULL;
}

static void
clish_param_fini(clish_param_t *instance) {

    /* deallocate the memory for instance instance */
    lub_string_free(instance->prefix);
    instance->prefix = NULL;
    lub_string_free(instance->defval);
    instance->defval = NULL;
    lub_string_free(instance->name);
    instance->name = NULL;
    lub_string_free(instance->text);
    instance->text = NULL;
}



/*---------------------------------------------------------
 * PUBLIC META FUNCTIONS
 *--------------------------------------------------------- */
clish_param_t *
clish_param_new(const char    *name,
                const char    *text,
                clish_ptype_t *ptype) {

    clish_param_t *instance = malloc(sizeof(clish_param_t));

    if(instance) {    

        clish_param_init(instance,name,text,ptype);
    }
    return instance;
}



/*---------------------------------------------------------
 * PUBLIC METHODS
 *--------------------------------------------------------- */
void
clish_param_delete(clish_param_t *instance) {

    clish_param_fini(instance);
    free(instance);
}



/*---------------------------------------------------------
 * PUBLIC ATTRIBUTES
 *--------------------------------------------------------- */
const char *
clish_param__get_name(const clish_param_t *instance) {

    return instance->name;
}

const char *
clish_param__get_text(const clish_param_t *instance) {

    return instance->text;
}

const char *
clish_param__get_range(const clish_param_t *instance) {

    return clish_ptype__get_range(instance->ptype);
}

clish_ptype_t *
clish_param__get_ptype(const clish_param_t *instance) {

    return instance->ptype;
}

void
clish_param__set_prefix(clish_param_t *instance,
                        const char    *prefix) {

    assert(NULL == instance->prefix);
    instance->prefix = lub_string_dup(prefix);
}

const char *
clish_param__get_prefix(const clish_param_t *instance) {

    return instance->prefix;
}

void
clish_param__set_default(clish_param_t *instance,
                         const char    *defval) {

    assert(NULL == instance->defval);
    instance->defval = lub_string_dup(defval);
}

const char *
clish_param__get_default(const clish_param_t *instance) {

    return instance->defval;
}

char *
clish_param_validate(const clish_param_t *instance,
                     const char          *text) {

    return clish_ptype_translate(instance->ptype,text);
}

void
clish_param_help(const clish_param_t *instance,
                 size_t               offset) {

    const char *range = clish_ptype__get_range(instance->ptype);
    printf("%*c\n",offset,'^');
    printf("%s  %s",
           instance->name, 
           clish_ptype__get_text(instance->ptype));
    if(NULL != range) {    

            printf(" (%s)",range);
    }
    printf("\n");
}

/** DEBUGGING *****************************************************************/

void
clish_param_dump(const clish_param_t *instance) {

    lub_dump_printf("param(%p)\n",instance);

    lub_dump_indent();
    lub_dump_printf("name   : %s\n",instance->name);
    lub_dump_printf("text   : %s\n",instance->text);
    lub_dump_printf("ptype  : %s\n",clish_ptype__get_name(instance->ptype));
    lub_dump_printf("prefix : %s\n",instance->prefix ? instance->prefix : "(null)");
    lub_dump_printf("default: %s\n",instance->defval ? instance->defval : "(null)");
    lub_dump_undent();
}

