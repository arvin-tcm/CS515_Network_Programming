/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt.c
 *  Function : This program is a client for a network based calculator.  It
 *             is specially designed for testing.
 *  Version  : 1
 *  Usage:   : netcalc_clnt serv-ip-address test-number
 * ------------------------------------------------------------------------
 */


#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <poll.h>
#include    <strings.h>
#include    "netcalc.h"

char    msgBuf[BUF_SIZE+10];

#define     TWO_SECONDS     2000

/* test 1 data */
char    test1_data1[] = {'Q', '9', '9', '5', '7', '+'};
char    test1_data2[] = {'3', '='};

char    test3_data[]  = {'Q', '1', '0', '0', '0', '+', '1', '=', 
                         'Q', '1', '0', '0', '0', '+', '2', '=', 
                         'Q', '1', '0', '0', '0', '+', '3', '=', 
                         'Q', '1', '0', '0', '0', '+', '4', '=', 
                         'Q', '1', '0', '0', '0', '+', '5', '=', 
                         'Q', '1', '0', '0', '0', '+', '6', '=', 
                         'Q', '1', '0', '0', '0', '+', '7', '=', 
                         'Q', '1', '0', '0', '0', '+', '8', '=', 
                         'Q', '1', '0', '0', '0', '+', '9', '='};


int
main(int argc, char *argv[])
{
    int                 sockFd;
    struct sockaddr_in  servAddr;
    int                 n;
    int                 msgLen;
    int                 testNum;
    struct pollfd       fdArray[1];       
    int                 nReady;

    /* a simple command line argument processing */
    if (argc != 3)
        err_quit("Usage: %s <IPaddress> <test-number>", argv[0]);

    testNum = atoi(argv[2]);

    /* open a socket */
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error");

    /* setup the server address and connect to it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(NETCALC_SERV_PORT);
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        err_sys("connect error");
    
    switch (testNum)
    {
        case 1:
            /* send the first portion of the app data */
            if (write(sockFd, test1_data1, sizeof(test1_data1)) <0)
                err_sys("write error");

            /* send the second portion of the app data */
            if (write(sockFd, test1_data2, sizeof(test1_data2)) <0)
                err_sys("write error");
            
            break;

        case 2:
            /* send the first portion of the app data */
            if (write(sockFd, test1_data1, sizeof(test1_data1)) <0)
                err_sys("write error");

            sleep(2);

            /* send the second portion of the app data */
            if (write(sockFd, test1_data2, sizeof(test1_data2)) <0)
                err_sys("write error");

            break;

        case 3:
            if (write(sockFd, test3_data, sizeof(test3_data)) <0)
                err_sys("write error");

            break;

        default:
            printf("unknown test number\n");
            break;
    }

    fdArray[0].fd = sockFd;
    fdArray[0].events = POLLIN;

    /* wait for the response */
    for (;;)
    {
        nReady = poll(fdArray, 1, TWO_SECONDS);

        /* poll error ?*/
        if (nReady < 0)
            err_sys("poll error");

        /* timeout ? */
        else if (nReady == 0)
        {
            printf("No response from the server\n");
            break;
        }
        /* we get data from the socket */
        else
        {
            if ((n = read(sockFd, msgBuf, BUF_SIZE)) <= 0)
                printf("No data from the server\n");
            else
            {
                msgBuf[n] = '\n';
                msgBuf[n+1] = '\0';
                fputs(msgBuf, stdout);
            }
        }
    }

    close(sockFd);

    exit(0);
}
