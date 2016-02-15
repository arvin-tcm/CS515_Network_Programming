/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc_clnt_v3.c
 *  Function : This program is a client for a network based calculator.
 *             It uses IO multiplexing to support batch mode operation.
 *  Version  : 3
 *  Usage:   : netcalc_clnt_v3 serv-hostname
 * ------------------------------------------------------------------------
 */

#include    "unp.h"
#include    "netcalc.h"

char    ioBuf[BUF_SIZE+10]; /* the ioBuf is always null-terminated */
char    msgBuf[BUF_SIZE+10];

struct parseBuf_t {
    int     curSize;        /* the current size of the buf with valid data */
    char    buf[BUF_SIZE*2];
};

struct parseBuf_t       psBuf;

static void
initParseBuf(void)
{
    psBuf.curSize = 0;
}

static void
appendParseBuf(char *buf, int len)
{
    if (len + psBuf.curSize > BUF_SIZE)
        err_quit("parseBuf overflow");

    bcopy(buf, psBuf.buf+psBuf.curSize, len);
    psBuf.curSize += len;

#if 1
    DUMP_BUF(psBuf.buf, psBuf.curSize);
#endif 
}

/*
 * return the size of the buffer which contains the applicaton message.
 * Return 0 if no applicaiton message found. 
 *
 * note: this implementation is not efficient.
 */
static int
getAppMsg(char *buf)
{
    int     i;
    char    *ptr;

    if (psBuf.curSize == 0)
        return 0;

#if 0
    DUMP_BUF(psBuf.buf, psBuf.curSize);
#endif 

    for (i = 0, ptr = psBuf.buf; i < psBuf.curSize; i++, ptr++)
    {
        if (*ptr == MSG_TERMINATOR)
            break;
    }

    /* not found */
    if (i == psBuf.curSize)
        return 0;

    /* found it */
    else
    {
        /* adjust to 1-base size */
        i++;

        /* copy it to the user buffer */
        bcopy(psBuf.buf, buf, i);

        /* adjust the leftover */
        bcopy(ptr+1, psBuf.buf, psBuf.curSize-i);

        /* adjust the size */
        psBuf.curSize -= i;

        return (i);
    }
}

void
doClient(int sockFd)
{
    int                 n;
    int                 msgLen;
    struct pollfd       fdArray[2];
    int                 nready;
    int                 writeShut =0;

    /* set up the poll fd arrays */
    fdArray[0].fd     = STDIN_FILENO;
    fdArray[0].events = POLLIN;

    fdArray[1].fd     = sockFd;
    fdArray[1].events = POLLIN;

    initParseBuf();

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
				//printf("**** to call shutdown\n");
				//sleep(15);
                Shutdown(sockFd, SHUT_WR);  /* no more write to the socket */
				//Close(sockFd);
                writeShut = 1;
    		    fdArray[0].fd = -1; /* no need to check stdin anymore */
            } 
            else if (buildReq(ioBuf, strlen(ioBuf), msgBuf, &msgLen) 
                     == SUCCESS)
            {
                /* send request to the server */
                Write(sockFd, msgBuf, msgLen);
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
            /* read from the socket only once */
            msgLen = read(sockFd, msgBuf, BUF_SIZE);

            if (msgLen < 0)
                err_quit("read error");

            else if (msgLen == 0)
            {
                if (writeShut == 1)
                    break;      /* normal shutdown of the sockets */
                else
                    err_quit("read error");
            }

            /* got data */
            {
                /* save it to the parse buffer */
                appendParseBuf(msgBuf, msgLen);
                
                /* extract application message from the parse buffer */
                while ((msgLen = getAppMsg(msgBuf)) != 0)
                {
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
    }
}
