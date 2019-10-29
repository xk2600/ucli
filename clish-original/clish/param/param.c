/*
 * param.c
 *
 * This file provides the implementation of the "param" class  
 */
#include "private.h"
#include "lub/string.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------
 * PRIVATE METHODS
 *--------------------------------------------------------- */
static void
ucli_param_init(ucli_param_t *this,
                 const char    *name,
                 const char    *text,
                 ucli_ptype_t *ptype)
{

    /* initialise the help part */
    this->name  = lub_string_dup(name);
    this->text  = lub_string_dup(text);
    this->ptype = ptype;

    /* set up defaults */
    this->prefix = NULL;
    this->defval = NULL;
}
/*--------------------------------------------------------- */
static void
ucli_param_fini(ucli_param_t *this)
{
    /* deallocate the memory for this instance */
    lub_string_free(this->prefix);
    this->prefix = NULL;
    lub_string_free(this->defval);
    this->defval = NULL;
    lub_string_free(this->name);
    this->name = NULL;
    lub_string_free(this->text);
    this->text = NULL;
}
/*---------------------------------------------------------
 * PUBLIC META FUNCTIONS
 *--------------------------------------------------------- */
ucli_param_t *
ucli_param_new(const char    *name,
                const char    *text,
                ucli_ptype_t *ptype)
{
    ucli_param_t *this = malloc(sizeof(ucli_param_t));

    if(this)
    {
        ucli_param_init(this,name,text,ptype);
    }
    return this;
}
/*---------------------------------------------------------
 * PUBLIC METHODS
 *--------------------------------------------------------- */
void
ucli_param_delete(ucli_param_t *this)
{
    ucli_param_fini(this);
    free(this);
}
/*---------------------------------------------------------
 * PUBLIC ATTRIBUTES
 *--------------------------------------------------------- */
const char *
ucli_param__get_name(const ucli_param_t *this)
{
    return this->name;
}
/*--------------------------------------------------------- */
const char *
ucli_param__get_text(const ucli_param_t *this)
{
    return this->text;
}
/*--------------------------------------------------------- */
const char *
ucli_param__get_range(const ucli_param_t *this)
{
    return ucli_ptype__get_range(this->ptype);
}
/*--------------------------------------------------------- */
ucli_ptype_t *
ucli_param__get_ptype(const ucli_param_t *this)
{
    return this->ptype;
}
/*--------------------------------------------------------- */
void
ucli_param__set_prefix(ucli_param_t *this,
                        const char    *prefix)
{
    assert(NULL == this->prefix);
    this->prefix = lub_string_dup(prefix);
}
/*--------------------------------------------------------- */
const char *
ucli_param__get_prefix(const ucli_param_t *this)
{
    return this->prefix;
}
/*--------------------------------------------------------- */
void
ucli_param__set_default(ucli_param_t *this,
                         const char    *defval)
{
    assert(NULL == this->defval);
    this->defval = lub_string_dup(defval);
}
/*--------------------------------------------------------- */
const char *
ucli_param__get_default(const ucli_param_t *this)
{
    return this->defval;
}
/*--------------------------------------------------------- */
char *
ucli_param_validate(const ucli_param_t *this,
                     const char          *text)
{
    return ucli_ptype_translate(this->ptype,text);
}
/*--------------------------------------------------------- */
void
ucli_param_help(const ucli_param_t *this,
                 size_t               offset)
{
    const char *range = ucli_ptype__get_range(this->ptype);
    printf("%*c\n",offset,'^');
    printf("%s  %s",
           this->name, 
           ucli_ptype__get_text(this->ptype));
    if(NULL != range)
    {
            printf(" (%s)",range);
    }
    printf("\n");
}
/*--------------------------------------------------------- */
