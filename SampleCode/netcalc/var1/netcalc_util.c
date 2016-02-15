/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_util.c
 *  Function : This file contains the utility functions used by both the
 *             server and the client, and both the sockets and xti.
 * ------------------------------------------------------------------------
 */

#include    <stdio.h>
#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <netdb.h>
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

