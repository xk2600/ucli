#include <string.h>
#include <stdlib.h>

#include "private.h"
/*--------------------------------------------------------- */
void
lub_scheduler_init(lub_scheduler_t            *self,
                   const lub_scheduler_attr_t *attr,
                   lub_scheduler_vtable_t     *vtable)
{
    if(attr)
    {
        /* copy the spec across */
        self->attr = *attr;
    }
    else
    {
        /* set up some defaults */
        lub_scheduler_attr_init(&self->attr);
    }
    self->vtable = vtable;
}
/*--------------------------------------------------------- */
void
lub_scheduler_delete(lub_scheduler_t *self)
{
    lub_scheduler_fini(self);

    /* release memory */
    free(self);
}
/*--------------------------------------------------------- */
void
lub_scheduler_fini(lub_scheduler_t *self)
{
    self->vtable->fini(self);

    /* tidy up for safety */
    self->vtable = NULL;
}
/*--------------------------------------------------------- */
bool_t
lub_scheduler_register_task(lub_scheduler_t       *self,
                            unsigned               priority,
                            lub_scheduler_task_fn *fn,
                            void                  *arg)
{
    lub_scheduler_task_t *task = 
        lub_scheduler_async_register_task(self,
                                          priority,
                                          fn,
                                          arg);
    if(task)
    {
        lub_scheduler_async_schedule_task(task);
    }
    return task ? BOOL_TRUE : BOOL_FALSE;
}
/*--------------------------------------------------------- */
lub_scheduler_task_t *
lub_scheduler_async_register_task(lub_scheduler_t       *self,
                                  unsigned               priority,
                                  lub_scheduler_task_fn *fn,
                                  void                  *arg)
{
    lub_scheduler_task_t *task;
    if(priority < self->attr.priority_min)
    {
        priority = self->attr.priority_min;
    }
    else if(priority > self->attr.priority_max)
    {
        priority = self->attr.priority_max;
    }
    task = lub_scheduler_new_task(self,
                                  priority,
                                  fn,
                                  arg);
    if(task);
    {
        ++self->pending;
    }
    return task;
}
/*--------------------------------------------------------- */
void
lub_scheduler_async_schedule_task(lub_scheduler_task_t *task)
{
    lub_scheduler_t *self = task->parent;
    self->vtable->register_task(self,task);
}
/*--------------------------------------------------------- */
bool_t
lub_scheduler_run_task(lub_scheduler_t *self)
{
    bool_t ran_task = BOOL_FALSE;
    lub_scheduler_task_t *task = self->vtable->get_task(self);
    if(task)
    {
        task->fn(task->arg);
        lub_scheduler_delete_task(self,task);
        --self->pending;
        ran_task = BOOL_TRUE;
    }
    return ran_task;
}
/*--------------------------------------------------------- */
bool_t
lub_scheduler_is_pending(const lub_scheduler_t *self)
{
    return self->pending ? BOOL_TRUE : BOOL_FALSE;
}
/*--------------------------------------------------------- */
lub_scheduler_task_t *
lub_scheduler_new_task(lub_scheduler_t       *self,
                       unsigned               priority,
                       lub_scheduler_task_fn *fn,
                       void                  *arg)
{
    return self->vtable->new_task(self,priority,fn,arg);
}
/*--------------------------------------------------------- */
void
lub_scheduler_delete_task(lub_scheduler_t      *self,
                          lub_scheduler_task_t *task)
{
    self->vtable->delete_task(self,task);
}
