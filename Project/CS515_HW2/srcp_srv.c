/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp_srv.c
 * Author: johnson5414
 *
 * Created on February 15, 2016, 2:44 AM
 */

#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "srcp.h"
/*
 * 
 */

static char reqBuf[BUF_SIZE + 10];
static char filePathBuf[BUF_SIZE + 10];

int parseReq(char *inBuf, int inBufLen, char *outBuf);

int main(int argc, char** argv) {
    int listenFd, connFd;
    struct sockaddr_in servAddr, cliAddr;
    int len, resultLen, n;

    /* a simple command line argument processing */
    if (argc != 1) {
        fprintf(stderr, "Usage: ");
        exit(1);
    }

    /* open a socket */
    if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(1);
    }

    /* setup the server address and bind it */
    bzero(&servAddr, sizeof (servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = INADDR_ANY;
    servAddr.sin_port = htons(SRCP_SERV_PORT);

    if (bind(listenFd, (struct sockaddr *) &servAddr, sizeof (servAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(listenFd, 16) < 0) {/* use a small queue limit */
        perror("listen error");
        exit(1);
    }
    printf("start to listen\n");
    /* loop forever */
    while (1) {
        len = sizeof (cliAddr);

        /* wait for connection */
        if ((connFd = accept(listenFd, (struct sockaddr*) &cliAddr, (socklen_t *) &len) < 0)) {
            perror("accept error");
            exit(1);
        }
	printf("len: %d\n", len);
        printf("connection accepted \n");
        /* key to handle file transfer */
	if (getpeername(connFd, (struct sockaddr*) &cliAddr, &len) < 0 ){
		perror("getpeername failed");
		exit(1);
	}
	printf("len: %d\n", len);
        if ((n = read(connFd, reqBuf, BUF_SIZE)) > 0) {
            printf("first package received:\n%s\n", reqBuf);
            if (parseReq(reqBuf, n, filePathBuf)) {
                char *buffer = 'U';
                write(connFd, buffer, 1);
            }
            printf("acknowledge sent\n");
            doTransfer(connFd, filePathBuf);
        }
        close(connFd);
        printf("reset\n");
    }
    close(listenFd);
    return (EXIT_SUCCESS);
}

int parseReq(char *inBuf, int inBufLen, char *outBuf) {
    if (inBufLen >= 1) {
        strcpy(outBuf, inBuf + 1);
        return SUCCESS;
    }
    return NO_SUCESS;
}
