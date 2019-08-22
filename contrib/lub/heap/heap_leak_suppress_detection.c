#include "private.h"
/*--------------------------------------------------------- */
void 
lub_heap_leak_suppress_detection(lub_heap_t  *this)
{
    ++this->suppress;
}
/*--------------------------------------------------------- */
