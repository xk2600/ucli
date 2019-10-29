/*
 * command.h
 */
#include "libucli/command.h"

/*---------------------------------------------------------
 * PRIVATE TYPES
 *--------------------------------------------------------- */
struct ucli_command_s
{
    lub_bintree_node_t bt_node;
    char           *name;
    char           *text;
    unsigned        paramc;
    ucli_param_t **paramv;
    char           *action;
    ucli_view_t   *view;
    char           *viewid;
    char           *detail;
    char           *builtin;
    char           *escape_chars;
    ucli_param_t  *args;
};
