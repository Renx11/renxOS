#ifndef _TASK_H_
#define _TASK_H_

#define TASK_IS_USE    0
#define TASK_NO_USE    1

#define TASK_STATUS_IDLE  0
#define TASK_STATUS_USE   1
#define TASK_STATUS_BLOCK 2


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
    Task_Entry func;
    Que_T  *msgQue;

    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    
} TASK_T;

typedef struct
{
    INT8  type;
    INT8  event;
    TID   sender;
    TID   reciever;
    INT32 contentLength;
}msgHead_T;


#pragma pack(4)


#endif
