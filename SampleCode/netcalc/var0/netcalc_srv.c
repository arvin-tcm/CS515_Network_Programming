/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv.c
 *  Function : This program is a server for a network based calculator.  It
 *             waits for the request from the client, does the calculation
 *             and sends the result back to the client.
 *  Version  : 1
 *  Usage:   : netcalc_srv 
 *  ClientVer: 1
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    reqBuf[BUF_SIZE+10];
char    resBuf[BUF_SIZE+10];

main(int argc, char *argv[])
{
    int                 listenFd, connFd;
    struct sockaddr_in  servAddr, cliAddr;
    socklen_t           len;
    int                 resultLen;
    int                 n;


    /* a simple command line argument processing */
    if (argc != 1)
        err_quit("Usage: %s");

    /* open a socket */
    listenFd = Socket(AF_INET, SOCK_STREAM, 0);

    /* setup the server address and bind it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(NETCALC_SERV_PORT);

    Bind(listenFd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    
    Listen(listenFd, LISTENQ);

    /* loop forever */
    for (;;) 
    {
        len = sizeof(cliAddr);

        /* wait for the connection */
        connFd = Accept(listenFd, (struct sockaddr *) &cliAddr, &len);

        /* read input here */
        while ((n = read(connFd, reqBuf, BUF_SIZE)) > 0) 
        {
            DUMP_BUF(reqBuf, n); /* for debugging purpose */

            if (doCalc(reqBuf, n, resBuf, &resultLen) == SUCCESS)
            {
                Write(connFd, resBuf, resultLen);
            }
            else
            {
                printf("!!%s: doCalc failed\n", __FUNCTION__);
            }
        }

        Close(connFd);
    }

    Close(listenFd);
}
