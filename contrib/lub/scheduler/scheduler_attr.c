#include "private.h"
/*--------------------------------------------------------- */
void
lub_scheduler_attr_init(lub_scheduler_attr_t *self)
{
    self->priority_min = 0;
    self->priority_max = 2;
}
/*--------------------------------------------------------- */
