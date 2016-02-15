/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt_v2.c
 *  Function : This program is a client for a network based calculator.  It
 *             reads input from stdin, sends the request to the server, waits
 *             for the result to come back, and writes the output to stdout.  
 *  Version  : 2
 *  Usage:   : netcalc_clnt_v2 serv-hostname (ip address also works)
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    ioBuf[BUF_SIZE+10]; /* the ioBuf is always null-terminated */
char    msgBuf[BUF_SIZE+10];

void
doClient(int sockFd)
{
    int                 n;
    int                 msgLen;

    while ((fgets(ioBuf, BUF_SIZE, stdin)) != NULL) 
    {
        if (buildReq(ioBuf, strlen(ioBuf), msgBuf, &msgLen) == SUCCESS)
        {
            /* send request to the server */
            Write(sockFd, msgBuf, msgLen);

            /* wait for response from the server */
            msgLen = readAppMsg(sockFd, msgBuf, BUF_SIZE);

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
}
