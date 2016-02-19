/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp_clnt.c
 * Author: johnson5414
 *
 * Created on February 18, 2016, 9:43 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include "srcp.h"

#define DATA_BUF_SIZE 128
static char ipBuff[BUF_SIZE + 10];
static char filePathBuff[BUF_SIZE + 10];

int parseReq(char *inBuf, char opCode);

/*
 * 
 */
int main(int argc, char** argv) {
    int sock_fd;
    int n;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    if (argc != 2) {
        printf("%s [server_ip]:filepath\n", argv[0]);
        exit(1);
    }

    /* parse the argument */
    parseReq(argv[1], 'U');

    /* open a socket */
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error\n");
        exit(1);
    }

    /* setup the server address and connect to it */
    bzero(&serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SRCP_SERV_PORT);
    inet_pton(AF_INET, ipBuff, &serv_addr.sin_addr);

    if (connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        perror("connect error");
        exit(1);
    }


    /* send the registration message to the server */
    if (write(sock_fd, filePathBuff, strlen(filePathBuff)) < 0) {
        perror("socket write failed\n");
        exit(1);
    }

    /* wait for the reply from the server */
    n = read(sock_fd, buffer, DATA_BUF_SIZE);
    if (n < 0) {
        perror("socket read failed\n");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Received answer from the server: %s\n", buffer);
    
    /* start transfer */
    if(filePathBuff[0] == buffer[0]) {
        printf("transfer confirm\n");
        doReceive(sock_fd, filePathBuff + 1);
    }
    
    
    close(sock_fd);
    return (EXIT_SUCCESS);
}

int parseReq(char *inBuf, char opCode) {
    char *p;
    p = strchr(inBuf, ':');
    if (p == NULL) {
        printf("Invalid srcp file path");
        exit(1);
    }
    *p = '\0';
    strcpy(ipBuff, inBuf);
    filePathBuff[0] = opCode;
    strcpy(filePathBuff + 1, p + 1);
    return SUCCESS;
}