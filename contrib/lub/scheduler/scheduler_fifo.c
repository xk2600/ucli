#include <stdlib.h>
#include <assert.h>

#include "private.h"
#include "scheduler_fifo.h"
/*--------------------------------------------------------- */
lub_scheduler_t *
lub_scheduler_fifo_new(const lub_scheduler_attr_t *attr)
{
    lub_scheduler_fifo_t *self = malloc(sizeof(lub_scheduler_fifo_t));
    if(self)
    {
        lub_scheduler_fifo_init(self,attr);
    }
    return &self->base;
}
/*--------------------------------------------------------- */
void
lub_scheduler_fifo_init(lub_scheduler_fifo_t       *self,
                        const lub_scheduler_attr_t *attr)
{
    static lub_scheduler_vtable_t vtable = {
        lub_scheduler_fifo_fini,
        lub_scheduler_fifo_register_task,
        lub_scheduler_fifo_new_task,
        lub_scheduler_fifo_get_task,
        lub_scheduler_fifo_delete_task
    };
    lub_scheduler_t *base = (lub_scheduler_t*)self;
    int              i;
    lub_scheduler_init(base,attr,&vtable);
    
    /* setup the blockpool from which to allocate task instances */
    lub_dblockpool_init(&self->task_pool,
                        sizeof(lub_scheduler_fifo_task_t),
                        100, /* blocks per chunk */
                        0);  /* unlimited chunks */

    /* create and initialise the task queues */
    unsigned num_queues 
        = base->attr.priority_max - base->attr.priority_min + 1;
    self->queues = malloc(sizeof(lub_fifo_t) * num_queues);
    assert(self->queues);
    for(i=0; i < num_queues; ++i)
    {
        lub_fifo_t *fifo = &self->queues[i];
        lub_fifo_init(fifo,offsetof(lub_scheduler_fifo_task_t,fifo_node));
    }
}
/*--------------------------------------------------------- */
/* virtual */
void
lub_scheduler_fifo_fini(lub_scheduler_t *base)
{
    lub_scheduler_fifo_t *self = (lub_scheduler_fifo_t *)base;
    int                   i;
    
    /* release the blockpool */
    lub_dblockpool_fini(&self->task_pool);
    
    /* release the task queues */
    unsigned num_queues 
        = base->attr.priority_max - base->attr.priority_min + 1;
    for(i=0; i < num_queues; ++i)
    {
        lub_fifo_t *fifo = &self->queues[i];
        lub_fifo_fini(fifo);
    }
    free(self->queues);
}
/*--------------------------------------------------------- */
/* virtual */
void
lub_scheduler_fifo_register_task(lub_scheduler_t      *base,
                                 lub_scheduler_task_t *task)
{
    lub_scheduler_fifo_t *self = (lub_scheduler_fifo_t *)base;

    lub_fifo_push(&self->queues[task->priority-base->attr.priority_min],task);
}
/*--------------------------------------------------------- */
/* virtual */
lub_scheduler_task_t *
lub_scheduler_fifo_new_task(lub_scheduler_t       *base,
                            unsigned               priority,
                            lub_scheduler_task_fn *fn,
                            void                  *arg)
{
    lub_scheduler_fifo_t      *self = (lub_scheduler_fifo_t *)base;
    lub_scheduler_fifo_task_t *task = lub_dblockpool_alloc(&self->task_pool);
    if(task)
    {
        lub_scheduler_task_init(&task->base,base,priority,fn,arg);
        lub_fifo_node_init(&task->fifo_node);
    }
    return task ? &task->base : 0;
}
/*--------------------------------------------------------- */
/* virtual */
void
lub_scheduler_fifo_delete_task(lub_scheduler_t      *base,
                               lub_scheduler_task_t *task_base)
{
    lub_scheduler_fifo_t      *self = (lub_scheduler_fifo_t *)base;
    lub_scheduler_fifo_task_t *task = (lub_scheduler_fifo_task_t *)task_base;

    lub_fifo_node_fini(&task->fifo_node);
    lub_scheduler_task_fini(task_base);

    lub_dblockpool_free(&self->task_pool,task);
}
/*--------------------------------------------------------- */
/* virtual */
lub_scheduler_task_t *
lub_scheduler_fifo_get_task(lub_scheduler_t *base)
{
    lub_scheduler_fifo_t *self = (lub_scheduler_fifo_t *)base;
    lub_scheduler_task_t *task = NULL;
    int i,num_queues = (base->attr.priority_max - base->attr.priority_min);
    for(i=0; i < num_queues; ++i)
    {
        task = lub_fifo_pull(&self->queues[i]);
        if(task)
        {
            break;
        }
    }
    return task;
}
/*--------------------------------------------------------- */
