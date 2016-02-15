/*
 * ------------------------------------------------------------------------
 *  Name     : netcalc.h
 *  Function : This file contains the common definitions for the netcalc
 *             clients and servers.
 * ------------------------------------------------------------------------
 */

/* 
 * Protocol description
 *
 *  This is an ASCII protocol.  The message starts with an one byte opcode
 *  and ends with character '='.  
 *
 *      +--------+-------------------------------------+
 *      | opcode | data (variable length)          | = |
 *      +--------+-------------------------------------+
 *
 *  opcode: 
 *          'Q' : Question
 *          'A' : Answer
 *          'X' : Rejected question
 *
 *  The data format for 'Q' (from client to server)
 *      (1) a + b = 
 *      (2) a - b =
 *          where a and b are numbers in ASCII (e.g., "86", "5", etc.)
 *
 *  The data format for 'A' (from server to client)
 *      c = 
 *          where c is a number in ASCII
 *
 *  The data format for 'X' (from server to client)
 *      =
 *          no user data
 */

/* server port number */
#define     NETCALC_SERV_PORT        21597 /* 20000 + my-unix-id */

/* xti device */
#define     XTI_TCP                  "/dev/tcp"
#define     XTI_UDP                  "/dev/udp"

/* opcode */
#define     OPCODE_QUESTION          'Q'
#define     OPCODE_ANSWER            'A'
#define     OPCODE_REJECT            'X'

/* message/record terminator */ 
#define     MSG_TERMINATOR           '='

/* misc. defines */
#define     BUF_SIZE                 2048

#define     SUCCESS                  1
#define     NO_SUCCESS               0

/* utility stuff (netcalc_util.c) */

void        debugOn(void);
void        debugOff(void);
void        dumpBuf(char *qBuf, int bufLen);
extern int  debugFlag; 

#define     DUMP_BUF(buf, len)      if (debugFlag) dumpBuf(buf, len)

/* header strings for the dump functions */
#define     DUMP_HDR_LOCAL          "Local"
#define     DUMP_HDR_PEER           "Peer"

/* dump functions */
extern void dumpAddr(char *header, char *hostName, struct sockaddr_in *addr);
extern void dumpHostAndAddr(char *header, struct sockaddr_in *addr);
extern void dumpLocalSockAddr(int sockFd);
extern void dumpPeerSockAddr(int sockFd);

/* application layer functions */
extern ssize_t readAppMsg(int fd, void *bufPtr, size_t bufLen);
extern int parseRes(char *inBuf, int inBufLen, char *outBuf);
extern int buildReq(char *inBuf, int inBufLen, char *outBuf, int *outBufLen);
extern int doCalc(char *qBuf, int qBufLen, char *aBuf, int *aBufLen);

/* entry function to the server */
void    doServer(int sockFd);

/* entry function to the client */
void    doClient(int sockFd);
