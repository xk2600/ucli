/*
 * view.h
 */
#include "libucli/view.h"
#include "lub/bintree.h"

/*---------------------------------------------------------
 * PRIVATE TYPES
 *--------------------------------------------------------- */
struct ucli_view_s
{
    lub_bintree_t      tree;
    lub_bintree_node_t bt_node;
    char              *name;
    char              *prompt;
};
