/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp_srv.c
 * Author: johnson5414
 *
 * Created on February 18, 2016, 9:08 PM
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
#include <ctype.h>
#include "srcp.h"

static char str[80];
char get_time_str();
void srcp_server(int sock_fd);
void srcp_process_client(int conn_fd);
int srcp_process_client_data(char* buffer, int msg_size);

/*
 * 
 */
int main(int argc, char** argv) {
    int sock_fd;
    struct sockaddr_in serv_addr;
    int serv_addr_len = sizeof (serv_addr);
    int socket_option;

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

    /* make the socket reusable*/
    /*
    socket_option = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &socket_option, sizeof (socket_option)) < 0) {
        printf("failed to set socket option\n");
        exit(-1);
    }
     */
    /* setup the server address structure */
    bzero(&serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SRCP_SERV_PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    /* bind it */
    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("bind error");
        exit(1);
    }

    /* listen for the client*/
    if (listen(sock_fd, 16) < 0) {
        perror("listen error\n");
        exit(1);
    }

    srcp_server(sock_fd);


    /* close server sock_fd*/
    close(sock_fd);
    return (EXIT_SUCCESS);
}

char get_time_str() {
    time_t t;
    int n;

    t = time(NULL);
    n = sprintf(str, "\n%s", ctime(&t));
    str[n - 1] = '\0';
    return str;
}

void srcp_server(int sock_fd) {
    char buffer[BUF_SIZE];
    int i, n;
    int conn_fd;
    time_t t;
    struct sockaddr_in clnt_addr;
    int clnt_addr_len = sizeof (clnt_addr);

    /* main server loop to accept all the client */
    while (1) {
        conn_fd = accept(sock_fd, (struct sockaddr *) &clnt_addr, (socklen_t *) & clnt_addr_len);
        if (conn_fd < 0) {
            perror("accept error");
            exit(1);
        }

        /* record the connection */
        (void) inet_ntop(AF_INET, &clnt_addr.sin_addr, buffer, BUF_SIZE);
        printf("%s -- %s.%d\n", get_time_str(), buffer, ntohs(clnt_addr.sin_port));

        srcp_process_client(conn_fd);
    }
}

void srcp_process_client(int conn_fd) {
    int n;
    char buffer[BUF_SIZE];
    char *rtn_msg;

    n = read(conn_fd, buffer, BUF_SIZE);
    if (n < 0) {
        perror("socket read failed\n");
        exit(1);
    }

    if (srcp_process_client_data(buffer, n)) {
        rtn_msg = "OK";
    } else {
        rtn_msg = "NOK";
    }

    if (write(conn_fd, rtn_msg, strlen(rtn_msg)) < 0) {
        perror("socket write failed\n");
        exit(1);
    }
}

int srcp_process_client_data(char* buffer, int msg_size) {
    int i;
    int result = 1;

    if (msg_size > BUF_SIZE - 2) {
        msg_size = BUF_SIZE - 2;
    }

    for (i = 0; i < msg_size; i++) {
        if (isprint(buffer[i])) {
            /* replace newline with - */
            if (buffer[i] == '\n') {
                buffer[i] = '-';
            }
        } else {
            /* replace non-printable char with - */
            buffer[i] = '-';
        }
    }

    buffer[msg_size + 1] = '\0';
    printf("message from client: %s\n", buffer);
    return result;
}