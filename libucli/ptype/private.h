/*
 * ptype.h
 */
#include "../pargv.h"
#include "lub/bintree.h"
#include "lub/argv.h"

#include <sys/types.h>
#include <regex.h>

typedef struct ucli_ptype_integer_s ucli_ptype_integer_t;
struct ucli_ptype_integer_s
{
    int min;
    int max;
};

typedef struct ucli_ptype_select_s ucli_ptype_select_t;
struct ucli_ptype_select_s
{
    lub_argv_t *items;
};

struct ucli_ptype_s
{
    lub_bintree_node_t       bt_node;
    char                    *name;
    char                    *text;
    char                    *pattern;
    char                    *range;
    ucli_ptype_method_e     method;
    ucli_ptype_preprocess_e preprocess;
    unsigned                 last_name; /* index used for auto-completion */
    union
    {
        regex_t               regexp;
        ucli_ptype_integer_t integer;
        ucli_ptype_select_t  select;
    } u;
};
