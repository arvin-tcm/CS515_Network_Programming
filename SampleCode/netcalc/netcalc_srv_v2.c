/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_v2.c
 *  Function : This program is a server for a network based calculator.  It
 *             waits for the request from the client, does the calculation
 *             and sends the result back to the client.
 *  Version  : 2
 *  Usage:   : netcalc_srv_v2 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

static void
servChild(int sockFd)
{
    int                 resultLen;
    int                 n;

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
}

void
doServer(int listenFd)
{
    int                 connFd;
    struct sockaddr_in  cliAddr;
    socklen_t           len;

    /* loop forever */
    for (;;) 
    {
        len = sizeof(cliAddr);
//printf("stalling for 35 seconds\n");
//sleep(35);
//printf("move on\n");
        /* wait for the connection */
        connFd = Accept(listenFd, (struct sockaddr *) &cliAddr, &len);

        /* print out local info of connFd */
        dumpLocalSockAddr(connFd);

        /* dump peer addr info */
        dumpPeerSockAddr(connFd);

        servChild(connFd);

        Close(connFd);
    }
}
