/*
 * pargv.h
 */
#include "libucli/pargv.h"
#include "libucli/param.h"

/*--------------------------------------------------------- */
struct ucli_parg_s
{
    const ucli_param_t *param;
    char                *value;
};
struct ucli_pargv_s
{
    unsigned     pargc;
    ucli_parg_t pargs[1];
};
/*--------------------------------------------------------- */
