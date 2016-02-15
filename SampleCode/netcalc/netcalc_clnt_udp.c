/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt_udp.c
 *  Function : This program is a client for a network based calculator.  
 *             It operates on UDP instead of TCP.
 *  Version  : 1
 *  Usage:   : netcalc_clnt_udp serv-hostname
 *  Note     : This version of the client only works with the servers that
 *             supports UDP mode. 
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

/* the doUdpClient is similar to the doClient of netcalc_clnt_v3.c */
char    ioBuf[BUF_SIZE+10]; /* the ioBuf is always null-terminated */
char    msgBuf[BUF_SIZE+10];

static void
doUdpClient(int sockFd, struct sockaddr_in *servAddr)
{
    int                 n;
    int                 msgLen;
    struct pollfd       fdArray[2];
    int                 nready;
    int                 writeShut =0;
    struct  sockaddr_in realServAddr;
    int                 adrLen;


    /* set up the poll fd arrays */
    fdArray[0].fd     = STDIN_FILENO;
    fdArray[0].events = POLLIN;

    fdArray[1].fd     = sockFd;
    fdArray[1].events = POLLIN;

    /* loop forever */
    for (;;) 
    {
        nready = Poll(fdArray, 2, INFTIM);

        /* input on stdin ?*/
        if (fdArray[0].revents & (POLLIN | POLLERR))
        {
            /* no more input */
            if (fgets(ioBuf, BUF_SIZE, stdin) == NULL) 
            {
                break;
            } 
            else if (buildReq(ioBuf, strlen(ioBuf), msgBuf, &msgLen) 
                     == SUCCESS)
            {
                /* send request to the server */
                Sendto(sockFd, msgBuf, msgLen, 0,
                       (struct sockaddr *)servAddr, 
                       sizeof(struct sockaddr_in));

                /* the address is not bound, only the port num is valid */
                dumpLocalSockAddr(sockFd);
            }
            else
            {
                printf("!!%s: buildReq failed\n", __FUNCTION__);
                DUMP_BUF(ioBuf, strlen(ioBuf));
            }
        }
        /* input on socket ?*/
        if (fdArray[1].revents & (POLLIN | POLLERR))
        {
            adrLen = sizeof(realServAddr);
            msgLen = Recvfrom(sockFd, msgBuf, BUF_SIZE+10, 0,
                              (struct sockaddr *)&realServAddr, &adrLen);
            dumpHostAndAddr(DUMP_HDR_PEER, &realServAddr);

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
    }
}

int
main(int argc, char *argv[])
{
    int                 sockFd;
    struct sockaddr_in  servAddr;
    struct  hostent     *pHostEnt;
    int                 n;
    int                 msgLen;
    const int 			on = 1;
	int					setBcastOpt;
	char				*dest;

    /* a simple command line argument processing */
	if (argc == 2)
	{
		setBcastOpt = 0;
		dest = argv[1];
	} else if (argc == 3 && strcmp(argv[1], "-b") == 0)
	{
		setBcastOpt = 1;
		dest = argv[2];
	} else
        err_quit("Usage: %s -b <hostname>", argv[0]);

    /* open a socket */
    sockFd = Socket(AF_INET, SOCK_DGRAM, 0);

    /* get the server ip address info */
    if ((pHostEnt = gethostbyname(dest)) == NULL)
        err_quit("gethostbyname failed for host %s", dest);

    /* setup the server address */
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(NETCALC_SERV_PORT);
    bcopy(pHostEnt->h_addr_list[0], &servAddr.sin_addr, 
          sizeof(struct in_addr));

    /* print out server info */
    dumpAddr(DUMP_HDR_PEER, pHostEnt->h_name, &servAddr);

	if (setBcastOpt)
        Setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    doUdpClient(sockFd, &servAddr);

    Close(sockFd);

    exit(0);
}
