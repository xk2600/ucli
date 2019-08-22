#ifndef _lub_fifo_h
#define _lub_fifo_h

#include <stddef.h>

#include "lub/c_decl.h"
#include "lub/ctype.h"

_BEGIN_C_DECL

/**
 * This type represents a FIFO "node"
 * Typically the client will have a "clientnode" structure 
 * which contains it's data. A FIFO "node" is made one of the 
 * data elements of that structure. When the FIFO is
 * initialised the client provides the offset into the
 * structure of the "node" which is to be used for the FIFO.
 */
typedef struct _lub_fifo_node lub_fifo_node_t;
/**
 * CLIENTS MUSTN'T TOUCH THE CONTENTS OF THIS STRUCTURE
 */
struct _lub_fifo_node
{
    /** internal */ lub_fifo_node_t *next;
};

/**
 * This type represents a FIFO instance.
 */
typedef struct _lub_fifo lub_fifo_t;
/**
 * CLIENTS MUSTN'T TOUCH THE CONTENTS OF THIS STRUCTURE
 */
struct _lub_fifo
{
    /** internal */ lub_fifo_node_t *head;
    /** internal */ lub_fifo_node_t *tail;
    /** internal */ size_t           node_offset;
};

/**********************************************************
 * lub_fifo_node CLASS methods
 ********************************************************** */ 
void
    lub_fifo_node_init(lub_fifo_node_t *self);
void
    lub_fifo_node_fini(lub_fifo_node_t *self);

/**********************************************************
 * lub_fifo CLASS methods
 ********************************************************** */ 
/**
 * Initialise the specified FIFO instance.
 *
 * \pre
 * none
 *
 * \return
 * none
 *
 * \post
 * The FIFO instance can have other methods invoked on it
 */
void
    lub_fifo_init(
        /** 
         * the "FIFO" instance to initialise
         */
        lub_fifo_t *self,
        /** 
         * the offset of the FIFO "node" structure wihtin the
         * "clientnode" structure. This is typically passed
         * using the offsetof() macro.
         */
        size_t node_offset
    );
/**
 * Finalise a FIFO instance
 *
 * \pre
 * - The FIFO instance must be initialised.
 * - The FIFO instance must be empty
 *
 * \return
 * none
 *
 * \post
 * - The FIFO instance can no longer be used with being
 * reinitialised.
 * - If the FIFO is not empty an assert will fire.
 */
void
    lub_fifo_fini(
        /** 
         * the "FIFO" instance to invoke this operation on
         */
        lub_fifo_t *self
    );
/**
 * Insert the specified client node into the FIFO.
 *
 * \pre
 * The FIFO instance must be initialised.
 *
 * \return
 * none
 *
 * \post
 * The client node will have been added to the tail of the 
 * FIFO.
 */
void
    lub_fifo_push(
        /** 
         * the "FIFO" instance to invoke this operation on
         */
        lub_fifo_t *self,
        /** 
         * the client node to insert
         */
        void *clientnode
    );
/**
 * Pull the first client node out of the FIFO
 *
 * \pre
 * The FIFO instance must be initialised.
 *
 * \return
 * A pointer to the client node at the head of the FIFO
 *
 * \post
 * The client node will have been removed from the FIFO and
 * has become the responsibility of the calling function.
 */
void *
    lub_fifo_pull(
        /** 
         * the "FIFO" instance to invoke this operation on
         */
        lub_fifo_t *self
    );
/**
 * Indicates whether the FIFO is empty or not
 *
 * \pre
 * The FIFO instance must be initialised.
 *
 * \return
 * - BOOL_TRUE - if the FIFO is empty.
 * - BOOL_FALSE - if the FIFO contains one or more nodes.
 *
 * \post
 * The FIFO remains unchanged.
 */
bool_t
    lub_fifo_is_empty(const lub_fifo_t *self);

_END_C_DECL

#endif /* _lub_fifo_h */
