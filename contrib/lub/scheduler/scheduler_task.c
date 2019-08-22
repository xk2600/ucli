#include <assert.h>
#include "private.h"
/*--------------------------------------------------------- */
void
lub_scheduler_task_init(lub_scheduler_task_t  *self,
                        lub_scheduler_t       *parent,
                        unsigned               priority,
                       lub_scheduler_task_fn  *fn,
                       void                   *arg)
{
    self->parent   = parent;
    self->priority = priority;
    self->fn       = fn;
    self->arg      = arg;
}
/*--------------------------------------------------------- */
void
lub_scheduler_task_fini(lub_scheduler_task_t *self)
{
    /* tidy up for safety */
    self->priority = 0;
    self->fn       = NULL;
    self->arg      = NULL;
}
/*--------------------------------------------------------- */
