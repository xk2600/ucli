#include "../private.h"
#include "../node.h"


extern char edata, end;


void
lub_heap_scan_bss(void) {
    /* now scan the memory */
    lub_heap_scan_memory(&edata,&end-&edata);
}
