/*
 * pargv_dump.c
 */
#include "private.h"
#include "lub/dump.h"

/*--------------------------------------------------------- */
void
ucli_parg_dump(const ucli_parg_t *this)
{
    lub_dump_printf("parg(%p)\n",this);
    lub_dump_indent();
    lub_dump_printf("name : %s\n",ucli_parg__get_name(this));
    lub_dump_printf("ptype: %s\n",
                    ucli_ptype__get_name(ucli_parg__get_ptype(this)));
    lub_dump_printf("value: %s\n",ucli_parg__get_value(this));
    lub_dump_undent();
}
/*--------------------------------------------------------- */
void
ucli_pargv_dump(const ucli_pargv_t *this)
{
    unsigned i;

    lub_dump_printf("pargv(%p)\n",this);
    lub_dump_indent();
    for(i =0;
        i < this->pargc;
        i++)
    {
        /* get the appropriate parameter definition */
        ucli_parg_dump(&this->pargs[i]);
    }
    lub_dump_undent();
}
/*--------------------------------------------------------- */
