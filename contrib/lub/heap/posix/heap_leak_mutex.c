#include <pthread.h>
#include <stdio.h>

#include "../private.h"
#include "../context.h"

static pthread_mutex_t leak_mutex = PTHREAD_MUTEX_INITIALIZER;

static lub_heap_leak_t my_instance;

/*--------------------------------------------------------- */
lub_heap_leak_t *
lub_heap_leak_instance(void)
{
    int status = pthread_mutex_lock(&leak_mutex);
    if(0 != status)
    {
        perror("pthread_mutex_lock() failed");
    }
    return &my_instance;
}
/*--------------------------------------------------------- */
void
lub_heap_leak_release(lub_heap_leak_t *instance)
{
    int status = pthread_mutex_unlock(&leak_mutex);
    if(0 != status)
    {
        perror("pthread_mutex_unlock() failed");
    }
}
/*--------------------------------------------------------- */
bool_t
lub_heap_leak_query_node_tree()
{
    return my_instance.m_node_tree.root ? BOOL_TRUE : BOOL_FALSE;
}
/*--------------------------------------------------------- */
bool_t
lub_heap_leak_query_clear_node_tree()
{
    return my_instance.m_clear_node_tree.root ? BOOL_TRUE : BOOL_FALSE;
}
/*--------------------------------------------------------- */
