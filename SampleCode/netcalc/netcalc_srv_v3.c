/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_v3.c
 *  Function : This program is a server for a network based calculator.  It
 *             forks a child server to service each client.
 *  Version  : 3
 *  Usage:   : netcalc_srv_v3 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

int     curClient = 0;

static void
sigChildHandler(int signo)
{
    pid_t   pid;
    int     stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) 
    {
        curClient--;
        /* the printf may lock the parent if the child dies too fast and the
         * parent is also doing the printf */
        printf("**%s: child %d terminated (total %d)\n", 
               __FUNCTION__, pid, curClient);
    }
    return;
}

static void
servChild(int sockFd)
{
    int         n;
    int         resultLen;

    /* read input here */
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
    int                 childPid;
    struct sigaction    act, oldAct;

    /* set up the signal handler for SIGCHLD */
    act.sa_handler = sigChildHandler;/* point to the signal handler */
    sigemptyset(&act.sa_mask);       /* mask nothing */
    act.sa_flags = SA_RESTART;   /* always restart interrupted system call*/
    if (sigaction(SIGCHLD, &act, &oldAct) < 0)
        err_sys("sigaction failed\n");

    /* loop forever */
    for (;;) 
    {
        len = sizeof(cliAddr);

        /* wait for the connection */
        if ((connFd = accept(listenFd, (struct sockaddr *) &cliAddr, &len)) 
            < 0)
        {
            if (errno == EINTR)
                continue;       
            else
                err_sys("accept failed");
        }

        /* fork a child to service the request */
        if ((childPid = Fork()) == 0) /* the child context */
        {
            Close(listenFd);

            /* print out local info of connFd */
            dumpLocalSockAddr(connFd);

            /* dump peer addr info */
            dumpPeerSockAddr(connFd);

            servChild(connFd);

            Close(connFd);

            /* see comments in sigChildHandler */
            sleep(1);

            exit(0);
        }
        else    /* the parent context */
        {
            curClient++;

            printf("**%s: Created child %d (total %d)\n", 
                   __FUNCTION__, childPid, curClient);

            Close(connFd);
        }
    }
}
