/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt.c
 *  Function : This program is a client for a network based calculator.  It
 *             reads input from stdin, sends the request to the server, waits
 *             for the result to come back, and writes the output to stdout.  
 *  Version  : 1
 *  Usage:   : netcalc_clnt serv-ip-address
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    ioBuf[BUF_SIZE+10]; /* the ioBuf is always null-terminated */
char    msgBuf[BUF_SIZE+10];

int
main(int argc, char *argv[])
{
    int                 sockFd;
    struct sockaddr_in  servAddr;
    int                 n;
    int                 msgLen;

    /* a simple command line argument processing */
    if (argc != 2)
        err_quit("Usage: %s <IPaddress>", argv[0]);

    /* open a socket */
    sockFd = Socket(AF_INET, SOCK_STREAM, 0);

    /* setup the server address and connect to it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(NETCALC_SERV_PORT);
    Inet_pton(AF_INET, argv[1], &servAddr.sin_addr);

    Connect(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr));

    /* main loop */
    while ((fgets(ioBuf, BUF_SIZE, stdin)) != NULL) 
    {
        if (buildReq(ioBuf, strlen(ioBuf), msgBuf, &msgLen) == SUCCESS)
        {
            /* send request to the server */
            Write(sockFd, msgBuf, msgLen);

            /* wait for response from the server */
            msgLen = Read(sockFd, msgBuf, BUF_SIZE);

            if (msgLen <= 0)
                break;
            
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
        else
        {
            printf("!!%s: buildReq failed\n", __FUNCTION__);
            DUMP_BUF(ioBuf, strlen(ioBuf));
        }
    }

    Close(sockFd);

    exit(0);
}
