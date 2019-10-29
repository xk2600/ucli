/*
 * shell_find_create_ptype.c
 */
#include "private.h"

#include <assert.h>
/*--------------------------------------------------------- */
ucli_ptype_t *
ucli_shell_find_create_ptype(ucli_shell_t           *this,
                              const char              *name,
                              const char              *text,
                              const char              *pattern,
                              ucli_ptype_method_e     method,
                              ucli_ptype_preprocess_e preprocess)
{
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
/*--------------------------------------------------------- */
