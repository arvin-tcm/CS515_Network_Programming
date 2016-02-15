/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt_main.c
 *  Function : This file contains the main function for the clients.  
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

int
main(int argc, char *argv[])
{
    int                 sockFd;
    struct sockaddr_in  servAddr;
    struct hostent      *pHostEnt;
    int                 n;
    int                 msgLen;

    /* a simple command line argument processing */
    if (argc != 2)
        err_quit("Usage: %s <hostname>", argv[0]);

    /* open a socket */
    sockFd = Socket(AF_INET, SOCK_STREAM, 0);

    /* get the server ip address info */
    if ((pHostEnt = gethostbyname(argv[1])) == NULL)
        err_quit("gethostbyname failed for host %s", argv[1]);

    /* setup the server address and connect to it */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(NETCALC_SERV_PORT);
    bcopy(pHostEnt->h_addr_list[0], &servAddr.sin_addr, 
          sizeof(struct in_addr));

    Connect(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr));

    /* print out server info */
    dumpAddr("Peer", pHostEnt->h_name, &servAddr);

    /* print out the local host and ip addr info */
    dumpLocalSockAddr(sockFd);

    doClient(sockFd);

    Close(sockFd);

    exit(0);
}
