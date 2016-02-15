/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_v4.c
 *  Function : This program is a server for a network based calculator.  It
 *             uses I/O multiplexing to serve multiple clients.
 *  Version  : 4
 *  Usage:   : netcalc_srv_v4 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

/*
 * may use RLIMIT_NOFILE for real a server
 */
#define     OPEN_MAX        8

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

/* client management information */
struct pollfd       client[OPEN_MAX+1];
int                 maxClient;  /* max index into client[] array */
int                 curClient;

static void 
initClientInfo(int sockFd)
{
    int         i;

    /* the first fd is the listen fd */
    client[0].fd     = sockFd;
    client[0].events = POLLIN;

    /* init the rest of the client array */
    for (i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1;      /* -1 indicates available entry */

    maxClient = 0;
    curClient = 0;
}

static void 
addClient(int sockFd)
{
    int     i;

    /* search for a slot for new client */
    for (i = 1; i < OPEN_MAX; i++)
    {
        /* found a free slot ? */
        if (client[i].fd < 0) {
            client[i].fd = sockFd;
            break;
        }
    }

    if (i == OPEN_MAX)
        err_quit("Too many clients");

    client[i].events = POLLIN;

    /* update maxClient */
    if (i > maxClient)
        maxClient = i;  

    curClient++;

    printf("**%s: sockFd %d added to entry %d (curClient %d, maxClient %d)\n",
            __FUNCTION__, sockFd, i, curClient, maxClient);
}

static void 
removeClient(int sockFd, int index)
{
    Close(sockFd);
    client[index].fd = -1;  /* indicate no client */
    curClient--;
    printf("**%s: sockFd %d removed from entry %d (curClient %d, "
           "maxClient %d)\n", 
           __FUNCTION__, sockFd, index, curClient, maxClient);
}

void
doServer(int listenFd)
{
    int                 connFd, sockFd;
    struct sockaddr_in  cliAddr;
    int                 nready; /* the number of fd's where input is ready */
    socklen_t           len;
    int                 resultLen;
    int                 i, n;

    initClientInfo(listenFd);

    /* loop forever */
    for (;;) 
    {
        nready = Poll(client, maxClient+1, INFTIM);

        /* new client connection ? */
        if (client[0].revents & POLLIN) 
        { 
            len = sizeof(cliAddr);

            /* wait for the connection */
            connFd = Accept(listenFd, (struct sockaddr *) &cliAddr, &len);

            /* print out local info of connFd */
            dumpLocalSockAddr(connFd);

            /* dump peer addr info */
            dumpPeerSockAddr(connFd);

            addClient(connFd);

            /* no more input ready file descriptors ? */
            if (--nready <= 0)
                continue;               
        }

        /* loop through all the clients */
        for (i = 1; i <= maxClient; i++) 
        {
            /* existing socket fd for a client ? */
            if ((sockFd = client[i].fd) < 0)
                continue;

            /* input on the socket fd ?*/
            if (client[i].revents & (POLLIN | POLLERR)) 
            {
                if ((n = read(sockFd, reqBuf, BUF_SIZE)) < 0) 
                {
                    /* connection reset by client ? */
                    if (errno == ECONNRESET) 
                    {
                        removeClient(sockFd, i);
                    } 
                    else
                        err_sys("read error");
                } 
                /* connection closed by client? */
                else if (n == 0) 
                {
                    removeClient(sockFd, i);
                } 
                /* got data from the client */
                else
                {
                    printf("**%s: data from socket fd %d\n", 
                           __FUNCTION__, sockFd);

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

                /* no more readable descriptors ? */
                if (--nready <= 0)
                    break;              
            }
        }
    }
}
