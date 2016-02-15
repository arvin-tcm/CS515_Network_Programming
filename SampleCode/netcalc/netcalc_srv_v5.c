/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_v5.c
 *  Function : This program is a server for a network based calculator.  It
 *             creates a thread to service each client.
 *  Version  : 5
 *  Usage:   : netcalc_srv_v5 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

/* the curClient global count */
int             curClient = 0;
pthread_mutex_t cntLock = PTHREAD_MUTEX_INITIALIZER;

static void
modCurClient(int change)
{
    int     n;

    /* acquire the lock */
    if ((n = pthread_mutex_lock(&cntLock)) != 0)
    {
        errno = n;
        err_sys("pthread_mutex_lock");
    }

    /* modify the count */
    curClient += change;

    /* release the lock */
    if ((n = pthread_mutex_unlock(&cntLock)) != 0)
    {
        errno = n;
        err_sys("pthread_mutex_lock");
    }
}

static void *
servChild(void *arg)
{
    int                 resultLen;
    int                 n;
    int                 sockFd;
    char                reqBuf[BUF_SIZE+10];
    char                resBuf[BUF_SIZE+10];

    sockFd = *(int *)arg;

    while ((n = readAppMsg(sockFd, reqBuf, BUF_SIZE)) > 0) 
    {
        DUMP_BUF(reqBuf, n); /* for debugging purpose */

        if (doCalc(reqBuf, n, resBuf, &resultLen) == SUCCESS)
        {
            Write(sockFd, resBuf, resultLen);
        }
        else
        {
            printf("!!%s: doCalc failed\n", __FUNCTION__);
        }
    }

    Close(sockFd);

    modCurClient(-1);

    printf("**%s: thread %d done (total %d)\n", 
           __FUNCTION__, pthread_self(), curClient);

    return (NULL);
}

void
doServer(int listenFd)
{
    int                 connFd;
    struct sockaddr_in  cliAddr;
    socklen_t           len;
    pthread_t           tid;
    int                 n;

    /* loop forever */
    for (;;) 
    {
        len = sizeof(cliAddr);

        /* wait for the connection */
        connFd = Accept(listenFd, (struct sockaddr *) &cliAddr, &len);

        /* print out local info of connFd */
        dumpLocalSockAddr(connFd);

        /* dump peer addr info */
        dumpPeerSockAddr(connFd);

        if ((n = pthread_create(&tid, NULL, servChild, (void *)&connFd)) != 0)
        {
            errno = n;
            err_sys("pthread_create error");
        }

        modCurClient(1);

        printf("**%s: thread %d created (total %d)\n", 
               __FUNCTION__, tid, curClient);

        /* do not close connFd here */
    }
}
