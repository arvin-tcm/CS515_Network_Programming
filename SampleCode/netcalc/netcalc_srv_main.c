/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_srv_main.c
 *  Function : This program contains the main function for the server.
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

main(int argc, char *argv[])
{
    int                 listenFd;
    struct sockaddr_in  servAddr;

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
    
    /* print out local info of listenFd */
    dumpLocalSockAddr(listenFd);

    Listen(listenFd, LISTENQ);

    doServer(listenFd);

    Close(listenFd);
}
