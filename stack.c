#include <stdlib.h>

typedef struct
{
    INT32 size;
    INT32 sum;
    INT32 count;
    INT32 head;
    INT32 tail;
    INT8 *data;
    pthread_mutex_t mutex;
} Que_T;

INT8 *InitQue(INT32 sum)
{
    Que_T *pQue = NULL;

    if(sum <= 0)
    {
        return NULL;
    }

    pQue = (Que_T*)OsMalloc(sizeof(Que_T));
    pQue->data = (INT8 *)OsMalloc(sum * sizeof(void *));
    if(pQue->data == NULL)
    {
        OsFree(pQue);
        return NULL;
    }

    pQue->size = sizeof(INT8 *);
    pQue->sum = sum;
    pQue->head = 0;
    pQue->tail = 0;
    pQue->count = 0;
    pthread_mutex_init(&pQue->mutex, NULL);

    return pQue; 
}

INT32 InQue(Que_T *pQue, void *pData)
{
    if(pQue->count == pQue->sum)
    {
        return RENX_OS_FAIL;
    }

    *(void **)((INT8 *)pQue->data + pQue->head * pQue->size) = pData;
    pQue->count++;
    pQue->head = (pQue->head+1) % pQue->sum;

    return RENX_OS_SUCCESS;
}

INT32 OutQue(Que_T *pQue, void **pData)
{
    if(pQue->count == 0)
    {
        return RENX_OS_FAIL;
    }

    *pData = *(void **)((INT8 *)pQue->data + pQue->tail * pQue->size);
    pQue->count--;
    pQue->tail = (pQue->tail+1) % pQue->sum;

    return RENX_OS_SUCCESS;
}

void DestroyQue(Que_T *pQue)
{
    free(pQue->data);
}

