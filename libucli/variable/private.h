/*
 * private.h
 */
#include "../variable.h"

/*--------------------------------------------------------- */
typedef struct context_s context_t;
struct context_s
{
    const char            *viewid;
    const ucli_command_t *cmd;
    ucli_pargv_t         *pargv;
};
/*--------------------------------------------------------- */
