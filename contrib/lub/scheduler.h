#ifndef _lub_scheduler_h
#define _lub_scheduler_h

#include <stddef.h>

#include "lub/c_decl.h"
#include "lub/ctype.h"

_BEGIN_C_DECL

typedef struct _lub_scheduler_attr lub_scheduler_attr_t;
/**
 * CLIENTS MUSTN'T TOUCH THE CONTENTS OF THIS STRUCTURE
 */
struct _lub_scheduler_attr
{
    /** internal */ unsigned priority_min;
    /** internal */ unsigned priority_max;
};

/**
 * This type represents a scheduler instance.
 */
typedef struct _lub_scheduler lub_scheduler_t;

/**
 * This type represents a scheduler task
 */
typedef struct _lub_scheduler_task lub_scheduler_task_t;

/**
 * This type represents a scheduler instance.
 */
typedef void (lub_scheduler_task_fn)(void *);

/**********************************************************
 * lub_scheduler CLASS methods
 ********************************************************** */ 
/**
 * Create an instance of a FIFO scheduler
 *
 * \pre
 * none
 *
 * \return
 * A scheduler instance
 *
 * \post
 * The calling client is responsible for releasing the instance
 * when finished with it.
 */
lub_scheduler_t *
    lub_scheduler_fifo_new(
        /** 
         * the attributes to be used to create this scheduler
         * A value of NULL means "use default values"
         */
        const lub_scheduler_attr_t *attr
    );
/**
 * Release a scheduler instance
 *
 * \pre
 *
 * \return
 * none
 *
 * \post
 * - The scheduler instance can no longer be used.
 */
void
    lub_scheduler_delete(
        /** 
         * the scheduler instance to delete
         */
        lub_scheduler_t *self
    );
/**
 * Register the specified task for execution.
 *
 * \return
 * BOOL_TRUE  - if the task was successfully scheduled.
 * BOOL_FALSE - if the task was not scheduled.
 *
 * \post
 * The scheduler takes responsibility for executing the
 * task at some point in the future.
 */
bool_t
    lub_scheduler_register_task(
        /** 
         * the scheduler instance to nvoke this operation on
         */
        lub_scheduler_t *self,
        /** 
         * the priority with which to handle this task.
         * This must lie within the bounds provided by
         * lub_scheduler_priority_min() and
         * lub_scheduler_priority_max(), any values outside
         * these will be limited to either the minimum
         * or maximum value appropriately.
         */
        unsigned priority,
        /** 
         * the function to invoke to perform this task.
         */
        lub_scheduler_task_fn *task,
        /** 
         * the argument to pass to the task function
         */
        void *arg
    );
/**
 * Register the specified task but don't schedule it for 
 * execution yet. It will only be placed into the execution
 * queue when lub_scheduler_async_schedule() it later called.
 *
 * By registering asynchronous events before they complete
 * the scheduler is aware of any outstanding tasks, hence 
 * can indicate to its clients whether it is busy or not.
 *
 * \return
 * none
 *
 * \post
 * The scheduler takes responisibility for running the provided
 * task at some point in the future.
 */
lub_scheduler_task_t *
    lub_scheduler_async_register_task(
        /** 
         * the scheduler instance to nvoke this operation on
         */
        lub_scheduler_t *self,
        /** 
         * the priority with which to handle this task.
         * This must lie within the bounds provided by
         * lub_scheduler_priority_min() and
         * lub_scheduler_priority_max(), any values outside
         * these will be limited to either the minimum
         * or maximum value appropriately.
         */
        unsigned priority,
        /** 
         * the function to invoke to perform this task.
         */
        lub_scheduler_task_fn *task,
        /** 
         * the argument to pass to the task function
         */
        void *arg
    );
/**
 * Schedule a previously registered asynchronous task for 
 * execution by the scheduler.
 *
 * \return
 * none
 *
 * \post
 * The scheduler takes responsibility for executing the
 * task at some point in the future.
 */
void
    lub_scheduler_async_schedule_task(
        /** 
         * the asynchronous task to schedule.
         */
        lub_scheduler_task_t *task
    );
/**
 * Run the next appropriate task, from the schedule queue.
 *
 * \return
 * BOOL_TRUE  - if a task has been run.
 * BOOL_FALSE - if no tasks where found in the schedule queue.
 *
 * \post
 * The task at the head of the schedule queue (if present) will
 * have been run and removed from the queue.
 */
bool_t
    lub_scheduler_run_task(
        /** 
         * the scheduler instance to invoke this operation on
         */
        lub_scheduler_t *self
    );
/**
 * Indicates whether there are any outstanding tasks
 * registered with this scheduler.
 * 
 * NB. it is possible to have asynchronous tasks registered
 * but not (yet) scheduled, hence a call to
 * lub_scheduler_run_task() could return BOOL_FALSE yet this 
 * return BOOL_TRUE.
 *
 * \return
 * - BOOL_TRUE  - if there are any tasks currently registered
 * - BOOL_FALSE - if there are no tasks registered.
 *
 * \post
 * The scheduler remains unchanged.
 */
bool_t
    lub_scheduler_is_pending(const lub_scheduler_t *self);

_END_C_DECL

#endif /* _lub_scheduler_h */
