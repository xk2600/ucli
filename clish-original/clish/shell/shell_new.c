/*
 * shell_new.c
 */
#include "private.h"

#include <assert.h>
#include <stdlib.h>
/*-------------------------------------------------------- */
static void
ucli_shell_init(ucli_shell_t             *this,
                 const ucli_shell_hooks_t *hooks,
                 void                      *cookie,
                 FILE                      *istream)
{
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
/*-------------------------------------------------------- */
ucli_shell_t *
ucli_shell_new(const ucli_shell_hooks_t *hooks,
                void                      *cookie,
                FILE                      *istream)
{
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
/*-------------------------------------------------------- */
