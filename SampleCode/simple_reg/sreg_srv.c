/*
 * -------------------------------------------
 *  sreg_srv.c -- simple registration server
 * -------------------------------------------
 */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include "sreg.h"

#define BUF_SIZE            512
#define DFT_CHAR            '-'     /* default char */
#define FD_ARR_MAX          20      /* at most 19 concurreny clients */

static char *log_file_name = "sreg_log";

static char str[80];    /* not multithread safe */
static char *
get_time_str (void)
{
    time_t t;
    int n;

    t = time(NULL);
    n = sprintf(str, "\n%s", ctime(&t));
    str[n-1] = '\0';  /* remove the newline */
    return str;
}

static int
sreg_process_reg_data (char *buf, int msg_size)
{
    int     i;
    int     result = 1;
    FILE    *log_file;

    if (msg_size > BUF_SIZE - 2) {
        msg_size = BUF_SIZE - 2;
    }

    for (i = 0; i < msg_size; i++) {
        if (isprint(buf[i])) {
            /* replace newline with - */
            if (buf[i] == '\n') {
                buf[i] = DFT_CHAR;
            }
        } else {
            /* replace non-printable char with - */
            buf[i] = DFT_CHAR;
        }
    }

    buf[msg_size+1] = '\0';

    /* the valid message must have the keyword name and id */
    if (strstr(buf, "name") && strstr(buf, "id")) {
        result = 1;
    } else {
        result = 0;
    }

    /* record the msg in the log file */
    if ((log_file = fopen(log_file_name, "a")) == NULL) {
        perror("failed to open log file\n");
        exit(1);
    }
    fprintf(log_file, "Received msg -- %s\n", buf);
    fclose(log_file);

    return result;
}

static  char *ok_msg = "OK";
static  char *nok_msg = "NOK";

static void
sreg_process_client (int fd)
{
    int     n;
    char    buf[BUF_SIZE];
    char    *rtn_msg;

    n = read(fd, buf, BUF_SIZE);
    if (n < 0) {
        perror("socket read failed\n");
        exit(1);
    }

    if (sreg_process_reg_data(buf, n)) {
        rtn_msg = ok_msg;
    } else {
        rtn_msg = nok_msg;
    }

    if (write(fd, rtn_msg, strlen(rtn_msg)) < 0) {
        perror("socket write failed\n");
        exit(1);
    }
}

static void
sreg_server (int sock_fd)
{
    char    buf[BUF_SIZE];
    int     i, n;
    int     conn_fd;
    FILE    *log_file;
    time_t t;
    struct sockaddr_in  clnt_addr;
    int                 clnt_addr_len = sizeof(clnt_addr);

    while (1) {
        conn_fd = accept(sock_fd, (struct sockaddr *) &clnt_addr,
                        (socklen_t *) &clnt_addr_len);

        if (conn_fd < 0) {
            perror("accept errro\n");
            exit(-1);
        }

        /* record the connection to the log file */
        (void) inet_ntop(AF_INET, &clnt_addr.sin_addr, buf, BUF_SIZE);
        if ((log_file = fopen(log_file_name, "a")) == NULL) {
            perror("failed to open log file\n");
            exit(1);
        }
        fprintf(log_file, "%s -- %s.%d\n", get_time_str(), buf, 
                ntohs(clnt_addr.sin_port));
        fclose(log_file);

        sreg_process_client(conn_fd);
    }
}

int
main(int argc, char *argv[])
{
    int                 sock_fd;
    struct sockaddr_in  serv_addr;
    int                 serv_addr_len = sizeof(serv_addr);
    int                 socket_option;

    /* a simple command line argument processing */
    if (argc != 1) {
        printf("Usage: %s\n", argv[0]);
        exit(1);
    }

    /* open a socket */
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error\n");
        exit(1);
    }

    /* make the socket reusable */
    socket_option = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option,
        sizeof(socket_option)) < 0) {
        printf("failed to set socket options\n");
        exit(-1);
    }

    /* setup the server address struct */
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SREG_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* bind it */
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error\n");
        exit(1);
    }

    /* listen for it */
    if (listen(sock_fd, 1024) < 0) {
        perror("listen errro\n");
        exit(-1);
    }

    sreg_server(sock_fd);

    close(sock_fd);

    return(0);
}
