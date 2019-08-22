#include <assert.h>

#include "private.h"
/*--------------------------------------------------------- */
void
lub_fifo_node_init(lub_fifo_node_t *self)
{
    self->next = NULL;
}
/*--------------------------------------------------------- */
void
lub_fifo_node_fini(lub_fifo_node_t *self)
{
    assert(NULL == self->next);
    /* tidy up for safety */
    self->next = NULL;
}
/*--------------------------------------------------------- */
