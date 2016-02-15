/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_util_soc.c
 *  Function : This file contains the utility functions used by the socket 
 *             applications.  It serves both the socket server and the client.
 * ------------------------------------------------------------------------
 */

#include    <stdio.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <netdb.h>
#include    "unp.h"
#include    "netcalc.h"

#define     COL_IN_LINE             8

/*
 * ------------------------------------------------------------------------
 *  Name     : dumpAddr
 *  Function : This function dumps the address info and the hostname.
 * ------------------------------------------------------------------------
 */

void 
dumpAddr(char *header, char *hostName, struct sockaddr_in  *addr)
{
    printf ("**%s: %s = %s, %s, %d\n", 
            __FUNCTION__, header , hostName, 
            inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
}

/*
 * ------------------------------------------------------------------------
 *  Name     : dumpHostAndAddr
 *  Function : This function dumps the hostname and the ip address info.  
 *             It tries to retrieve the hostname using the address.
 * ------------------------------------------------------------------------
 */

void 
dumpHostAndAddr(char *header, struct sockaddr_in *addrPtr)
{
    size_t              addrLen;
    struct  hostent     *pHostEnt;

    addrLen = sizeof(*addrPtr);

    /* serach the host name using the ip addr */
    if ((pHostEnt = gethostbyaddr((void *)&addrPtr->sin_addr, addrLen, 
                                  AF_INET)) == NULL)
    {
        dumpAddr(header, "---", addrPtr);
    }
    else
    {
        dumpAddr(header, pHostEnt->h_name, addrPtr);
    }
}

/*
 * ------------------------------------------------------------------------
 *  Name     : dumpBuf
 *  Function : This function dumps the buffer for debugging purpose.
 * ------------------------------------------------------------------------
 */

int  debugFlag = 1;

void 
debugOn(void)
{
    debugFlag = 1;
}

void 
debugOff(void)
{
    debugFlag = 0;
}

void 
dumpBuf(char *buf, int bufLen)
{
    int     i;
    int     col = 0;
    unsigned char    c;

    printf ("**%s: len  : %d\n", __FUNCTION__, bufLen);

    for (i = 0; i < bufLen; i++)
    {
        /* print line header */
        if (col == 0)
        {
            printf ("**%s: data : ", __FUNCTION__);
        }

        c = buf[i];
        if (isprint(c))
        {
            printf("%02x(%c) ", c, c);
        }
        else
        {
            printf("%02x( ) ", c);
        }

        if (++col == COL_IN_LINE)
        {
            printf("\n");
            col = 0;
        }
    }
    
    if (col != 0)
        printf("\n");
}


/*
 * ------------------------------------------------------------------------
 *  Name     : dumpLocalSockAddr
 *  Function : This function dumps the local hostname and the ip address
 *             It uses gethostname to get the hostname and getsockname to
 *             get the ip address from the socket.
 * ------------------------------------------------------------------------
 */

#define     HOST_NAME_LEN        512

void 
dumpLocalSockAddr(int sockFd)
{
    struct sockaddr_in  myAddr;
    socklen_t           myAddrLen;
    char                hostName[HOST_NAME_LEN];
    struct  hostent     *pHostEnt;

    /* get local sock address info */
    myAddrLen = sizeof(myAddr);

    if (getsockname(sockFd, (struct sockaddr *)&myAddr, &myAddrLen) < 0)
        err_quit("getsockname failed");

    if (gethostname(hostName, HOST_NAME_LEN) < 0)
        err_quit("gethostname failed");

    dumpAddr(DUMP_HDR_LOCAL, hostName, &myAddr);
}


/*
 * ------------------------------------------------------------------------
 *  Name     : dumpPeerSockAddr
 *  Function : This function dumps the peer hostname and the ip address
 *             It uses getpeername to get the ip address from the socket fd,
 *             and uses gethostbyaddr to get the hostname from the ip address.
 * ------------------------------------------------------------------------
 */

void 
dumpPeerSockAddr(int sockFd)
{
    struct sockaddr_in  peerAddr;
    socklen_t           peerAddrLen;
    struct  hostent     *pHostEnt;

    /* get local sock address info */
    peerAddrLen = sizeof(peerAddr);

    /* get peer sokcet info */
    if (getpeername(sockFd, (struct sockaddr *)&peerAddr, &peerAddrLen) < 0)
        err_quit("getpeername failed");

    dumpHostAndAddr(DUMP_HDR_PEER, &peerAddr);
}


/*
 * ------------------------------------------------------------------------
 *  Name     : readAppMsg
 *  Function : This function reads in an application message (as defined 
 *             by the application layer).  It is based on readline from 
 *             the text book. 
 *  Note     : This function should only be used in a blocking setting.
 * ------------------------------------------------------------------------
 */

ssize_t
readAppMsg(int fd, void *bufPtr, size_t bufLen)
{
    ssize_t n, rc;
    char    c, *ptr;

    ptr = bufPtr;
    for (n = 1; n < bufLen; n++) 
    {
        if ((rc = read(fd, &c, 1)) == 1) 
        {
            *ptr++ = c;
			//printf("-- received one char -- %c\n", c);
            if (c == MSG_TERMINATOR)
                break;
        } 
        else if (rc == 0) 
        {
            if (n == 1)
                return(0);  /* EOF, no data read */
            else 
                break;      /* EOF, some data was read */
        } else
            return(-1); /* error */
    }

    return(n);
}
