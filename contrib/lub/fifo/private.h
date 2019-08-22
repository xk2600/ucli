#include "lub/fifo.h"

/*------------------------------------------------------------ */
/* This operation converts a "node" into a "clientnode"
 * subtracting the offset gives the base pointer to the node
 *
 * this - the tree to invoke this operation upon
 * node - the node to convert
 */
#define lub_fifo_getclientnode(self,node)\
(void *)(((char*)node) - self->node_offset)
/*------------------------------------------------------------ */
/* This operation converts a "clientnode" into a "node"
 * adding the offset gives the base pointer to the node
 *
 * self       - the tree to invoke this operation upon
 * clientnode - the clientnode to convert
 */
#define lub_fifo_getnode(fifo,clientnode)\
(lub_fifo_node_t *)(((char*)clientnode) + self->node_offset) /*lint -e826 */
/*------------------------------------------------------------ */
