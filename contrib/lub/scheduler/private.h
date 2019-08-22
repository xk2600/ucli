#include "lub/scheduler.h"
#include "lub/fifo.h"

typedef struct _lub_scheduler_vtable lub_scheduler_vtable_t;
struct _lub_scheduler_vtable
{
    void
        (*fini)(lub_scheduler_t *self);
    void 
        (*register_task)(lub_scheduler_t      *self,
                         lub_scheduler_task_t *task);
    lub_scheduler_task_t *
        (*new_task)(lub_scheduler_t       *self,
                    unsigned               priority,
                    lub_scheduler_task_fn *fn,
                    void                  *arg);
    lub_scheduler_task_t *
        (*get_task)(lub_scheduler_t *self);
    void
        (*delete_task)(lub_scheduler_t      *self,
                       lub_scheduler_task_t *task);
};
struct _lub_scheduler
{
    lub_scheduler_vtable_t *vtable;
    lub_scheduler_attr_t    attr;
    unsigned                pending;
};

void
lub_scheduler_init(lub_scheduler_t            *self,
                   const lub_scheduler_attr_t *attr,
                   lub_scheduler_vtable_t     *vtable);
void
    lub_scheduler_fini(lub_scheduler_t *self);

struct _lub_scheduler_task
{
    lub_scheduler_t       *parent;
    unsigned               priority;
    lub_scheduler_task_fn *fn;
    void                  *arg;
};

lub_scheduler_task_t *
    lub_scheduler_new_task(lub_scheduler_t       *self,
                           unsigned               priority,
                           lub_scheduler_task_fn *fn,
                           void                  *arg);
void
    lub_scheduler_delete_task(lub_scheduler_t      *self,
                              lub_scheduler_task_t *task);
void
    lub_scheduler_task_init(lub_scheduler_task_t *self,
                            lub_scheduler_t      *parent,
                            unsigned               priority,
                            lub_scheduler_task_fn *fn,
                            void                  *arg);
void
    lub_scheduler_task_fini(lub_scheduler_task_t *self);

void
    lub_scheduler_attr_init(lub_scheduler_attr_t *self);

typedef struct _lub_scheduler_fifo lub_scheduler_fifo_t;
