/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt_v2.c
 *  Function : This program is a client for a network based calculator.  It
 *             uses threads to support batch mode operation.
 *  Version  : 4
 *  Usage:   : netcalc_clnt_v4 serv-hostname 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"


static int  writeShut = 0;

static void *
doRequest(void *arg)
{
    int                 sockFd;
    int                 msgLen;
    char                ioBuf[BUF_SIZE+10]; 
    char                msgBuf[BUF_SIZE+10];

    sockFd = *(int *)arg;

    /* this thread services the request (stdio->socket) */
    while ((fgets(ioBuf, BUF_SIZE, stdin)) != NULL) 
    {
        if (buildReq(ioBuf, strlen(ioBuf), msgBuf, &msgLen) == SUCCESS)
        {
            /* send request to the server */
            Write(sockFd, msgBuf, msgLen);
        }
        else
        {
            printf("!!%s: buildReq failed\n", __FUNCTION__);
            DUMP_BUF(ioBuf, strlen(ioBuf));
        }
    }

    Shutdown(sockFd, SHUT_WR);  /* no more write to the socket */
    writeShut = 1;

    return (NULL);
}

void
doClient(int sockFd)
{
    int                 n;
    int                 msgLen;
    pthread_t           tid;
    char                ioBuf[BUF_SIZE+10]; 
    char                msgBuf[BUF_SIZE+10];

    if ((n = pthread_create(&tid, NULL, doRequest, &sockFd)) != 0)
    {
        errno = n;
        err_sys("pthread_create error");
    }

    printf("**%s: thread %d created\n", __FUNCTION__, tid);

    /* this thread services the response (socket->stdio) */
    while ((msgLen = readAppMsg(sockFd, msgBuf, BUF_SIZE)) >0)
    {
        if (parseRes(msgBuf, msgLen, ioBuf) == SUCCESS)
        {
            fputs(ioBuf, stdout);
        }
        else
        {
            printf("!!%s: parseRes failed\n", __FUNCTION__);
            DUMP_BUF(msgBuf, msgLen);
        }
    }

    if (writeShut == 0)
        err_quit("server terminated prematurely");
}
