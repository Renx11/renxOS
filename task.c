#include <stdlib.h>
#include <pthread.h>

#define  TASK_MAX_NUM  500

pthread_key_t   g_selfTidKey;
INT32   g_localNode = 3;
TASK_T  *g_taskList;

typedef void *(*Task_Entry)(void *param);

INT32 init_task(INT32 num)
{
    INT32 i = 0;
    
	if( num <= 0 || num > TASK_MAX_NUM)
	{
        return RENX_OS_FAIL;
	}

    pthread_key_create(&g_selfTidKey, NULL);

    g_taskList = (TASK_T *)OsMalloc( num * sizeof(TASK_T));
    if(g_taskList == NULL)
    {
        return RENX_OS_FAIL;
    }


    return RENX_OS_SUCCESS;
}

INT32 CreateTask(INT32 id, Task_Entry func, INT32 stackSzie)
{
    pthread_t systid;
    
    if( tid <0 || id >= TASK_MAX_NUM
        ||g_taskList[id].use == TASK_IS_USE )
    {
        return RENX_OS_FAIL;
    }

    g_taskList[id].tid.nodeId = g_localNode;
    g_taskList[id].tid.taskId = id;

    /*创建队列*/
    g_taskList[id].msgQue = InitQue(100);
    pthread_mutex_init(&g_taskList[id].mutex, NULL);
    pthread_cond_init(&g_taskList[id].cond, NULL);

    if( RENX_OS_SUCCESS != OsCreateThread(&systid, func, &id, stackSzie) )
    {
        memset(g_taskList[id], 0, sizeof(TASK_T));
        return RENX_OS_FAIL;
    }

    g_taskList[id].systid = systid;

    return RENX_OS_SUCCESS;
   
}

INT32 OsCreateThread(pthread_t *sysid, Task_Entry func, void *param, INT32 stackSzie)
{
    INT32  err;
    pthread_attr_t  attr;

    err = pthread_attr_init(&attr);
    if(0 != err)
    {
        return RENX_OS_FAIL;
    }

    pthread_attr_destroy(&attr);

    do
    {
        err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if(0 != err)
        {
            break;
        }

        stackSzie = stackSzie < PTHREAD_STACK_MIN ? PTHREAD_STACK_MIN:stackSzie;
        
        err = pthread_attr_setstacksize(&attr, stackSzie);
        if(0 != err)
        {
            break;
        }   

        err = pthread_create(sysid, &attr, func, param);
        if(0 != err)
        {
            break;
        }   

        err = RENX_OS_SUCCESS;

   } while (0)
    
   pthread_attr_destroy(&attr);
    
   return err; 
}

void TaskThread(void *param)
{
    INT32 tid = *(INT32 *)param;
    INT8 *pMsg = NULL;
    INT32 iRet = 0;

    pthread_setspecific(g_selfTidKey, (void *)&tid);

    while(1)
    {
        while(1)
        {
            iRet = OutQue(&g_taskList[tid].msgQue, &pMsg);
            if(RENX_OS_SUCCESS != iRet)
            {
                break;
            }

            /*处理message*/
        }

        waitMsg(g_taskList[tid].mutex, g_taskList[tid].cond);
    }

    return;
}

INT32 ASendx(INT8 *Buf, INT32 bufLen, TID tid)
{
    if(tid.nodeId == g_localNode)
    {
        return SendToLocalTask(Buf, bufLen, tid);
    }
    else
    {
        return SendToRemoteTask(Buf, bufLen, tid);
    }
}

typedef struct
{
    INT8  type;
    INT8  event;
    TID   sender;
    TID   reciever;
    INT32 contentLength;
}msgHead_T;

INT32 SendToLocalTask(INT8 *Buf, INT32 bufLen, TID sender, TID tid)
{
    msgHead_T msg = {0}
    INT8 *pBuf = NULL;
    
    msg.type = 0;
    msg.event = 1;
    msg.sender.nodeId = sender.nodeId;
    msg.sender.taskId = sender.taskId;
    msg.reciever.nodeId = tid.nodeId;
    msg.reciever.taskId = tid.taskId;
    msg.contentLength = bufLen;

    pBuf = OsMalloc(sizeof(msgHead_T)+bufLen);
    memcpy(pBuf, *msg, sizeof(msgHead_T));
    memcpy(pBuf+sizeof(msgHead_T), Buf, bufLen);

    InQue(g_taskList[sender.taskId], pBuf);

    setMsg(g_taskList[tid].mutex, g_taskList[tid].cond);

    return RENX_OS_SUCCESS;
}

INT32 SendToRemoteTask(INT8 *Buf, INT32 bufLen, TID tid)
{
    return RENX_OS_SUCCESS;
}

