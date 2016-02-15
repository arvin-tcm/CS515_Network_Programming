/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_v6.c
 *  Function : This program is a server for a network based calculator.  It
 *             is a TCP and UDP combined server.  The TCP portion is based
 *             on the version 3 of the server.
 *  Version  : 6
 *  Usage:   : netcalc_srv_v6 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

int     curClient = 0;

/* this function is identical to that in v3 */
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

/* this function is identical to that in v3 */
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

/* this function is similar to doServer in v3 (with the for loop removed) */
static void
doTcpServer(int listenFd)
{
    int                 connFd;
    struct sockaddr_in  cliAddr;
    socklen_t           len;
    int                 childPid;

    len = sizeof(cliAddr);

    /* get the connnection */
    if ((connFd = accept(listenFd, (struct sockaddr *) &cliAddr, &len)) < 0)
    {
        if (errno == EINTR)
            return;
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

static void
doUdpServer(int sockFd)
{
    int                 n;
    int                 resultLen;
    struct sockaddr_in  clientAddr;
    socklen_t           adrLen;

    /* set up the address field first */
    adrLen = sizeof(clientAddr);
    bzero(&clientAddr, adrLen);

    /* read input here */
    n = Recvfrom(sockFd, reqBuf, BUF_SIZE+10, 0,
                 (struct sockaddr *)&clientAddr, &adrLen);
    dumpLocalSockAddr(sockFd);
    dumpHostAndAddr(DUMP_HDR_PEER, &clientAddr);

    DUMP_BUF(reqBuf, n); /* for debugging purpose */

    if (doCalc(reqBuf, n, resBuf, &resultLen) == SUCCESS)
    {
        Sendto(sockFd, resBuf, resultLen, 0,
               (struct sockaddr *)&clientAddr, adrLen);
    }
    else
    {
        printf("!!%s: doCalc failed\n", __FUNCTION__);
    }
}

void doTcpUdpServer(int listenFd, int udpFd)
{
    struct sigaction    act, oldAct;
    struct pollfd       fdArray[2];
    int                 nready;

    /* set up the signal handler for SIGCHLD */
    act.sa_handler = sigChildHandler;/* point to the signal handler */
    sigemptyset(&act.sa_mask);       /* mask nothing */
    act.sa_flags = SA_RESTART;   /* always restart interrupted system call*/
    if (sigaction(SIGCHLD, &act, &oldAct) < 0)
        err_sys("sigaction failed\n");

    /* set up the poll fd arrays */
    fdArray[0].fd     = listenFd;
    fdArray[0].events = POLLIN;

    fdArray[1].fd     = udpFd;
    fdArray[1].events = POLLIN;

    /* loop forever */
    for (;;) 
    {
        nready = poll(fdArray, 2, INFTIM);

        if (nready < 0)
        {
            if (errno == EINTR)
                continue;       
            else
                err_sys("poll failed");
        }

        /* input on TCP ?*/
        if (fdArray[0].revents & (POLLIN | POLLERR))
        {
            doTcpServer(listenFd);
        }
        /* input on UDP ?*/
        if (fdArray[1].revents & (POLLIN | POLLERR))
        {
            doUdpServer(udpFd);
        }
    }
}

main(int argc, char *argv[])
{
    int                 listenFd;
    int                 udpFd;
    struct sockaddr_in  servAddr;

    /* a simple command line argument processing */
    if (argc != 1)
        err_quit("Usage: %s");

    /* open a TCP socket */
    listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    /* setup the server address and bind it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(NETCALC_SERV_PORT);

    Bind(listenFd, (struct sockaddr *) &servAddr, sizeof(servAddr));

    /* open a UDP socket */
    udpFd = Socket(AF_INET, SOCK_DGRAM, 0);

    Bind(udpFd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    
    /* print out local info of listenFd */
    dumpLocalSockAddr(listenFd);

    dumpLocalSockAddr(udpFd);

    Listen(listenFd, LISTENQ);

    doTcpUdpServer(listenFd, udpFd);

    Close(listenFd);
    Close(udpFd);
}
