/*
 * ptype_dump.c
 */
#include "private.h"
#include "lub/dump.h"

/*--------------------------------------------------------- */
void
ucli_ptype_dump(ucli_ptype_t *this)
{
    lub_dump_printf("ptype(%p)\n",this);
    lub_dump_indent();
    lub_dump_printf("name       : %s\n",
                    ucli_ptype__get_name(this));
    lub_dump_printf("text       : %s\n",
                    ucli_ptype__get_text(this));
    lub_dump_printf("pattern    : %s\n",
                    this->pattern            );
    lub_dump_printf("method     : %s\n",
                    ucli_ptype_method__get_name(this->method));
    lub_dump_printf("postprocess: %s\n",
                    ucli_ptype_preprocess__get_name(this->preprocess));
    lub_dump_undent();
}
/*--------------------------------------------------------- */
