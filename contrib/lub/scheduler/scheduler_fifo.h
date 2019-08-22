#include "lub/dblockpool.h"
struct _lub_scheduler_fifo
{
    lub_scheduler_t  base;
    lub_dblockpool_t task_pool;
    lub_fifo_t      *queues;
};

typedef struct _lub_scheduler_fifo_task lub_scheduler_fifo_task_t;
struct _lub_scheduler_fifo_task
{
    lub_scheduler_task_t base;
    lub_fifo_node_t      fifo_node;    
};

void
    lub_scheduler_fifo_init(lub_scheduler_fifo_t       *self,
                            const lub_scheduler_attr_t *attr);
void
    lub_scheduler_fifo_fini(lub_scheduler_t *base);
void
    lub_scheduler_fifo_register_task(lub_scheduler_t      *base,
                                     lub_scheduler_task_t *task);
lub_scheduler_task_t *
    lub_scheduler_fifo_new_task(lub_scheduler_t       *base,
                                unsigned               priority,
                                lub_scheduler_task_fn *fn,
                                void                  *arg);
lub_scheduler_task_t *
    lub_scheduler_fifo_get_task(lub_scheduler_t *base);
void
    lub_scheduler_fifo_delete_task(lub_scheduler_t      *base,
                                   lub_scheduler_task_t *task_base);
