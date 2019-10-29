/*
 * param.c
 *
 * This file provides the implementation of the "param" class  
 */
#include "libucli/param.h"
#include "lub/dump.h"
#include "lub/string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/** PRIVATE TYPES *************************************************************/
struct ucli_param_s {

    char          *name;
    char          *text;
    ucli_ptype_t *ptype;  /* The type of instance parameter */ 
    char          *prefix; /* if non NULL the name of the identifying switch parameter */
    char          *defval; /* default value to use for instance parameter */
};



/** PRIVATE TYPES *************************************************************/
/*---------------------------------------------------------
 * PRIVATE METHODS
 *--------------------------------------------------------- */
static void
ucli_param_init(ucli_param_t *instance,
                 const char    *name,
                 const char    *text,
                 ucli_ptype_t *ptype) {


    /* initialise the help part */
    instance->name  = lub_string_dup(name);
    instance->text  = lub_string_dup(text);
    instance->ptype = ptype;

    /* set up defaults */
    instance->prefix = NULL;
    instance->defval = NULL;
}

static void
ucli_param_fini(ucli_param_t *instance) {

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
ucli_param_t *
ucli_param_new(const char    *name,
                const char    *text,
                ucli_ptype_t *ptype) {

    ucli_param_t *instance = malloc(sizeof(ucli_param_t));

    if(instance) {    

        ucli_param_init(instance,name,text,ptype);
    }
    return instance;
}



/*---------------------------------------------------------
 * PUBLIC METHODS
 *--------------------------------------------------------- */
void
ucli_param_delete(ucli_param_t *instance) {

    ucli_param_fini(instance);
    free(instance);
}



/*---------------------------------------------------------
 * PUBLIC ATTRIBUTES
 *--------------------------------------------------------- */
const char *
ucli_param__get_name(const ucli_param_t *instance) {

    return instance->name;
}

const char *
ucli_param__get_text(const ucli_param_t *instance) {

    return instance->text;
}

const char *
ucli_param__get_range(const ucli_param_t *instance) {

    return ucli_ptype__get_range(instance->ptype);
}

ucli_ptype_t *
ucli_param__get_ptype(const ucli_param_t *instance) {

    return instance->ptype;
}

void
ucli_param__set_prefix(ucli_param_t *instance,
                        const char    *prefix) {

    assert(NULL == instance->prefix);
    instance->prefix = lub_string_dup(prefix);
}

const char *
ucli_param__get_prefix(const ucli_param_t *instance) {

    return instance->prefix;
}

void
ucli_param__set_default(ucli_param_t *instance,
                         const char    *defval) {

    assert(NULL == instance->defval);
    instance->defval = lub_string_dup(defval);
}

const char *
ucli_param__get_default(const ucli_param_t *instance) {

    return instance->defval;
}

char *
ucli_param_validate(const ucli_param_t *instance,
                     const char          *text) {

    return ucli_ptype_translate(instance->ptype,text);
}

void
ucli_param_help(const ucli_param_t *instance,
                 size_t               offset) {

    const char *range = ucli_ptype__get_range(instance->ptype);
    printf("%*c\n",offset,'^');
    printf("%s  %s",
           instance->name, 
           ucli_ptype__get_text(instance->ptype));
    if(NULL != range) {    

            printf(" (%s)",range);
    }
    printf("\n");
}

/** DEBUGGING *****************************************************************/

void
ucli_param_dump(const ucli_param_t *instance) {

    lub_dump_printf("param(%p)\n",instance);

    lub_dump_indent();
    lub_dump_printf("name   : %s\n",instance->name);
    lub_dump_printf("text   : %s\n",instance->text);
    lub_dump_printf("ptype  : %s\n",ucli_ptype__get_name(instance->ptype));
    lub_dump_printf("prefix : %s\n",instance->prefix ? instance->prefix : "(null)");
    lub_dump_printf("default: %s\n",instance->defval ? instance->defval : "(null)");
    lub_dump_undent();
}

