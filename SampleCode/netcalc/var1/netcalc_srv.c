/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv.c
 *  Function : This program is a server for a network based calculator.  It
 *             waits for the request from the client, does the calculation
 *             and sends the result back to the client.
 *  Version  : 1 (Variation 1) : it does not use the wrapper socket 
 *             functions from the text book.
 *  Usage:   : netcalc_srv 
 * ------------------------------------------------------------------------
 */


#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <stdio.h>
#include    <strings.h>
#include    "netcalc.h"
#include    "myerror.h"

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

main(int argc, char *argv[])
{
    int                 listenFd, connFd;
    struct sockaddr_in  servAddr, cliAddr;
    int         len;
    int         resultLen;
    int         n;


    /* a simple command line argument processing */
    if (argc != 1)
        err_quit("Usage: %s");

    /* open a socket */
    if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    /* setup the server address and bind it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(NETCALC_SERV_PORT);

    if (bind(listenFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        err_sys("bind error");
    
    if (listen(listenFd, 16) < 0) /* use a small queue limit */
        err_sys("listen error");

    /* loop forever */
    for (;;) 
    {
        len = sizeof(cliAddr);

        /* wait for the connection */
        if ((connFd = accept(listenFd, (struct sockaddr *) &cliAddr, &len)) 
            < 0)
            err_sys("accept error");

        /* read input here */
        while ((n = read(connFd, reqBuf, BUF_SIZE)) > 0) 
        {
            DUMP_BUF(reqBuf, n); /* for debugging purpose */

            if (doCalc(reqBuf, n, resBuf, &resultLen) == SUCCESS)
            {
                if (write(connFd, resBuf, resultLen) < 0)
                    err_sys("write error");
            }
            else
            {
                printf("!!%s: doCalc failed\n", __FUNCTION__);
                DUMP_BUF(reqBuf, n);
            }
        }

        close(connFd);
    }

    close(listenFd);
}
