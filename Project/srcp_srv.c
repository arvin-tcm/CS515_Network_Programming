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

char reqBuf[BUF_SIZE + 10];
char filePathBuf[BUF_SIZE + 10];

int parseReq(char *inBuf, int inBufLen, char *outBuf);

int main(int argc, char** argv) {
    int listenFd, connFd;
    struct sockaddr_in servAddr, cliAddr;
    int len, resultLen, n;

    /* a simple command line argument processing */
    if (argc != 1) {
        fprintf(stderr, "Usage: %s");
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
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SRCP_SERV_PORT);

    if (bind(listenFd, (struct sockaddr *) &servAddr, sizeof (servAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(listenFd, 16) < 0) {/* use a small queue limit */
        perror("listen error");
        exit(1);
    }

    /* loop forever */
    while (1) {
        len = sizeof (cliAddr);

        /* wait for connection */
        if ((connFd = accept(listenFd, (struct sockaddr*) &cliAddr, &len) < 0)) {
            perror("accept error");
            exit(1);
        }

        /* key loop to handle file transfer */
        while ((n = read(connFd, reqBuf, BUF_SIZE)) > 0) {
            if (parseReq(reqBuf, n, filePathBuf)) {
                write(connFd, 'U', 1);
            }
            doTransfer(connFd, filePathBuf);
        }
        close(connFd);
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
