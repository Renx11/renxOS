#include <stdlib.h>


void *OsMalloc(size_t size)
{
    INT8 *buf;

    if(size <= 0)
    {
        return NULL;
    }

    buf = malloc(size);
    if(buf == NULL)
    {
        return NULL;
    }
    
    memset(buf, 0, size);
    return buf;
}

void OsFree(void *buf)
{
    if( buf )
        free(buf);

    return;
}
