#include "private.h"
/*--------------------------------------------------------- */
void 
lub_heap_leak_restore_detection(lub_heap_t  *this)
{
    --this->suppress;
}
/*--------------------------------------------------------- */
