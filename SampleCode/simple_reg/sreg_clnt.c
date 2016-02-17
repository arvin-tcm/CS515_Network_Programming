/*
 * ---------------------------------------------
 *  sreg_clnt.c -- simple registration client
 * ---------------------------------------------
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/socket.h>
#include    <sys/types.h>
#include    <string.h>
#include    <netdb.h>
#include    "sreg.h"

#define     DATA_BUF_SIZE   128

int
main(int argc, char *argv[])
{
    int                 sock_fd;
    int                 n;
    struct sockaddr_in  serv_addr;
    char                *msg = "name = John Smith; id = 12345js";
    char                buf[DATA_BUF_SIZE];

    if (argc != 2) { 
        printf("%s [server_ip]\n", argv[0]);
        return(1);
    }

    /* open a socket */
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error\n");
        exit(1);
    }

    /* setup the server address and connect to it */
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SREG_PORT);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0) {
        perror("connect error\n");
        exit(1);
    }

    /* send the registration message to the server */
    if (write(sock_fd, msg, strlen(msg)) < 0) {
        perror("socket write failed\n");
        exit(1);
    }

    /* wait for the reply from the server */
    n = read(sock_fd, buf, DATA_BUF_SIZE); 
    if (n < 0) {
        perror("socket read failed\n");
        exit(1);
    }

    buf[n] = '\0';
    printf("Received answer from the server: %s\n", buf);

    close(sock_fd);

    return(0);
}
