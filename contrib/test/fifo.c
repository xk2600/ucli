#include <stdlib.h>

#include "lub/test.h"
#include "lub/fifo.h"

/*************************************************************
 * TEST CODE
 ************************************************************* */

static int testseq;

typedef struct _test_node test_node_t;
struct _test_node
{
    int             i;
    lub_fifo_node_t fifo_node;
};
/*--------------------------------------------------------------- */
int node_id = 0;
static
test_node_t *
new_node(void)
{
    test_node_t *self = malloc(sizeof(test_node_t));
    self->i = ++node_id;
    return self;
}
/*--------------------------------------------------------------- */
/* This is the main entry point for this executable
 */
int main(int argc, const char *argv[])
{
    int status;
    lub_fifo_t   fifo;
    test_node_t *node;
    
    lub_test_parse_command_line(argc,argv);
    lub_test_begin("lub_fifo");

    lub_test_seq_begin(++testseq,"lub_fifo_init()");
    
    lub_fifo_init(&fifo,
                  offsetof(test_node_t,fifo_node));

    lub_test_check(BOOL_TRUE == lub_fifo_is_empty(&fifo),
                   "check FIFO is empty");
    
    lub_test_seq_begin(++testseq,"insert several nodes");
    lub_fifo_push(&fifo,new_node());

    lub_test_check(BOOL_FALSE == lub_fifo_is_empty(&fifo),
                   "check FIFO is not empty");

    lub_fifo_push(&fifo,new_node());
    lub_fifo_push(&fifo,new_node());
    
    lub_test_seq_begin(++testseq,"lub_fifo_pull() remove the nodes");
    node = lub_fifo_pull(&fifo);
    lub_test_check_int(1,node->i,"check node");
    free(node);
    node = lub_fifo_pull(&fifo);
    lub_test_check_int(2,node->i,"check node");
    free(node);
    node = lub_fifo_pull(&fifo);
    lub_test_check_int(3,node->i,"check node");
    free(node);
    lub_test_check(BOOL_TRUE == lub_fifo_is_empty(&fifo),
                   "check FIFO is not empty");

    lub_test_seq_begin(++testseq,"lub_fifo_fini()");
    lub_fifo_fini(&fifo);

    lub_test_seq_end();
        
    /* tidy up */
    status = lub_test_get_status();
    lub_test_end();
       
    return status;
}
