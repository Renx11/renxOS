#include <stdlib.h>
#include <sys/socket.h>

LINK_T *g_LinkList;

INT32 InitLink(int num)
{
    g_LinkList = (LINK_T *)OsMalloc(num);
    
    if(g_LinkList == NULL)
    {
        return RENX_OS_FAIL;
    }
    
    return RENX_OS_SUCCESS;
}

INT32 CommReadThread()
{
    return RENX_OS_SUCCESS;
}

INT32 CommRecvThread()
{
    

    return RENX_OS_SUCCESS;
]