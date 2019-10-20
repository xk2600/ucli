/* heap_symShow.c - convert addresses to line number and function name */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../private.h"
#include "../context.h"


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */


// gcc vs clang for extern void.

#ifdef _gcc
#  define __weak__ __attribute__ ((weak))
#else
#  define __weak__
#endif

extern void __weak__ sysheap_suppress_leak_detection(void);
extern void __weak__ sysheap_restore_leak_detection(void);

/* Simple vanilla dump of symbols address */
void 
lub_heap_symShow(unsigned address) {

    printf(" 0x%08x",address);
}

/* Dummy address name matching function */
bool_t 
lub_heap_symMatch(unsigned    address,
                  const char *string) {

    return BOOL_FALSE;
}

/* dummy initialisation call */
void
lub_heap_init(const char *file_name) {

    file_name = file_name;
    lub_heap__set_framecount(MAX_BACKTRACE);
}

