#ifndef _TASK_H_
#define _TASK_H_

#define TASK_IS_USE    0
#define TASK_NO_USE    1


#pragma pack(1)

typedef struct
{
    INT32  nodeId;
    INT32  taskId;
} TID;

typedef struct
{
    TID    tid;
    INT8   use;
    INT8   status;
    pthread_t systid;
    Que_T  *msgQue;

    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    
} TASK_T;

#pragma pack(4)


#endif
