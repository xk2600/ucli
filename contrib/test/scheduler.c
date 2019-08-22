#include <stdlib.h>
#include <stdio.h>

#include "lub/scheduler.h"
#include "lub/test.h"

/*************************************************************
 * TEST CODE
 ************************************************************* */

static int testseq;

static lub_scheduler_t *scheduler;

static lub_scheduler_task_fn test_task,test_task2;
/*--------------------------------------------------------------- */
static void
test_task(void *arg)
{
    int i = (int)arg;
    printf("test_task: %d\n",i);
    (void)lub_scheduler_register_task(scheduler,
                                      0,
                                      test_task2,
                                      (void*)2);
}
/*--------------------------------------------------------------- */
static void
test_task2(void *arg)
{
    int i = (int)arg;
    printf("test_task2: %d\n",i);
}
/*--------------------------------------------------------------- */
/* This is the main entry point for this executable
 */
int main(int argc, const char *argv[])
{
    int                   status;
    lub_scheduler_task_t *task;
    
    lub_test_parse_command_line(argc,argv);
    
    lub_test_begin("lub_scheduler");

    lub_test_seq_begin(++testseq,"check initial state");
    scheduler = lub_scheduler_fifo_new(NULL);
    
    lub_test_check(BOOL_FALSE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has no pending tasks");
    lub_test_check(BOOL_FALSE == lub_scheduler_run_task(scheduler),
                   "check scheduler doesn't run anything");
    
    lub_test_seq_begin(++testseq,"add and run a synchronous task tree");
    lub_test_check(BOOL_TRUE == lub_scheduler_register_task(scheduler,
                                                            0,
                                                            test_task,
                                                            (void*)1),
                   "Check we can schedule a task");
    
    lub_test_check(BOOL_TRUE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has pending tasks");
    lub_test_check(BOOL_TRUE == lub_scheduler_run_task(scheduler),
                   "check scheduler runs a task");
    lub_test_check(BOOL_TRUE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has pending tasks");
    lub_test_check(BOOL_TRUE == lub_scheduler_run_task(scheduler),
                   "check scheduler runs a task");
    lub_test_check(BOOL_FALSE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has no pending tasks");
    lub_test_check(BOOL_FALSE == lub_scheduler_run_task(scheduler),
                   "check scheduler doesn't run anything");
    
    lub_test_seq_begin(++testseq,"add and run an asynchronous task");
    task = lub_scheduler_async_register_task(scheduler,
                                             0,
                                             test_task2,
                                             (void*)1);
    lub_test_check(NULL != task,"Check task was created");
    lub_test_check(BOOL_TRUE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has pending tasks");
    lub_test_check(BOOL_FALSE == lub_scheduler_run_task(scheduler),
                   "check scheduler doesn't run anything");
    lub_scheduler_async_schedule_task(task);
    lub_test_check(BOOL_TRUE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has pending tasks");
    lub_test_check(BOOL_TRUE == lub_scheduler_run_task(scheduler),
                   "check scheduler runs a task");
    lub_test_check(BOOL_FALSE == lub_scheduler_is_pending(scheduler),
                   "check scheduler has no pending tasks");
    lub_test_check(BOOL_FALSE == lub_scheduler_run_task(scheduler),
                   "check scheduler doesn't run anything");
    
    lub_test_seq_begin(++testseq,"finalise the scheduler");

    lub_scheduler_delete(scheduler);
    
    lub_test_seq_end();
        
    /* tidy up */
    status = lub_test_get_status();
    lub_test_end();
       
    return status;
}
