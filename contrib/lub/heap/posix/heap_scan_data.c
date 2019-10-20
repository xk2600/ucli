#include "../private.h"
#include "../node.h"


extern char etext, edata;
char *lub_heap_data_start = &etext;
char *lub_heap_data_end   = &edata;

void
lub_heap_scan_data(void) {

    /* now scan the memory */
    lub_heap_scan_memory(lub_heap_data_start,
                         lub_heap_data_end-lub_heap_data_start);
}
