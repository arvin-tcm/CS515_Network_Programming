/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp_clnt.c
 * Author: johnson5414
 *
 * Created on February 15, 2016, 3:42 AM
 */

#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "srcp.h"

/*
 * 
 */
char ipBuf[BUF_SIZE + 10];
char filePathBuf[BUF_SIZE + 10];

int parseReq(char *inBuf, char opCode);

int main(int argc, char** argv) {
    int sockFd;
    struct sockaddr_in servAddr;
    int n, msgLen;
    char buffer[5];

    /* a simple command line argument processing */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPaddress>", argv[0]);
        exit(1);
    }

    /* data process */
    parseReq(argv[1], 'U');

    /* open a socket */
    if ((sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    /* setup the server address and connect to it */
    bzero(&servAddr, sizeof (servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(SRCP_SERV_PORT);
    servAddr.sin_addr.s_addr = inet_addr(ipBuf);

    if (connect(sockFd, (struct sockaddr *) &servAddr, sizeof (servAddr)) < 0) {
        perror("connect error");
        exit(1);
    }

    /* main */
    printf("connect built\n");
    printf("package sending:\n%s, %d\n", filePathBuf, strlen(filePathBuf));
    if (write(sockFd, filePathBuf, strlen(filePathBuf)) < 0) {
        perror("request failed");
        exit(1);
    }

    printf("wait for acknowledge\n");
    if (read(sockFd, buffer, 1) < 0) {
        perror("request rejected");
        exit(1);
    }
    printf("acknowledge received\n");
    if (buffer[0] == OPCODE_UPLOAD) {
        printf("start file transfer\n");
        doTransfer(sockFd, filePathBuf);
    }
    return (EXIT_SUCCESS);
}

int parseReq(char *inBuf, char opCode) {
    char *p;
    p = strchr(inBuf, ':');
    if (p == NULL) {
        printf("Invalid srcp file name\n");
        exit(1);
    }
    *p = 0;
    strcpy(ipBuf, inBuf);
    filePathBuf[0] = opCode;
    strcpy(filePathBuf + 1, p + 1);
    return SUCCESS;
}
