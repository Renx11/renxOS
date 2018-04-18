#ifndef _LINK_H_
#define _LINK_H_

#pragma pack(1)

typedef struct
{
    INT32  nodeid;
    INT32  linkIndex[15];
    INT32  cur;
    
} Node_T;


typedef struct
{
    socket_t socket;
    INT8  ip[32+1];
    INT32 port;
    INT8  *pSendBuf;
 
} LINK_T;

#pragma pack(4)

#endif
