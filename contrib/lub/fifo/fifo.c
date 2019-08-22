#include <assert.h>

#include "private.h"
/*--------------------------------------------------------- */
void
lub_fifo_init(lub_fifo_t *self,
              size_t      node_offset)
{
    self->head        = NULL;
    self->tail        = NULL;
    self->node_offset = node_offset;
}
/*--------------------------------------------------------- */
void
lub_fifo_fini(lub_fifo_t *self)
{
    assert(NULL == self->head);
    /* tidy up for safety */
    self->head        = NULL;
    self->tail        = NULL;
    self->node_offset = 0;
}
/*--------------------------------------------------------- */
void
lub_fifo_push(lub_fifo_t *self,
              void       *clientnode)
{
    lub_fifo_node_t *node = lub_fifo_getnode(self,clientnode);
    node->next = NULL;
    if(NULL == self->head)
    {
        self->tail = self->head = node;
    }
    else
    {
        self->tail = self->tail->next = node;
    }
}
/*--------------------------------------------------------- */
void *
lub_fifo_pull(lub_fifo_t *self)
{
    lub_fifo_node_t *node = self->head;
    if(node)
    {
        self->head = node->next;
        node->next = NULL;
        if(NULL == self->head)
        {
            self->tail = NULL;
        }
    }
    return node ? lub_fifo_getclientnode(self,node) : 0;
}
/*--------------------------------------------------------- */
bool_t
lub_fifo_is_empty(const lub_fifo_t *self)
{
    return self->head ? BOOL_FALSE : BOOL_TRUE;
}
/*--------------------------------------------------------- */
