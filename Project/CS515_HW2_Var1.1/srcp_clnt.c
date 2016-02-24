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
static char serverFilePathBuff[BUF_SIZE + 10];
static char clientFilePathBuff[BUF_SIZE + 10];
static char opCode;
char parsePath(char *source, char *destination);
int hostname_to_ip(char *hostName);

/*
 * 
 */
int main(int argc, char** argv) {
    int sock_fd;
    int n;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        printf("%s sourceFilePath destinationfilePath\n", argv[0]);
        exit(1);
    }
    /* parse the argument */
    parsePath(argv[1], argv[2]);

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
    if (write(sock_fd, serverFilePathBuff, strlen(serverFilePathBuff)) < 0) {
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
    //printf("Received answer from the server: %s\n", buffer);
    if (opCode != buffer[0]) {
        printf("operation confirm failed\n");
        exit(1);
    }
    /* start file operation */
    switch (opCode) {
        case OPCODE_DOWNLOAD_TO_CLIENT:
            doReceive(sock_fd, clientFilePathBuff);
            break;
        case OPCODE_UPLOAD_TO_SERVER:
            doTransfer(sock_fd, clientFilePathBuff);
            break;
        default:
            break;
    }
    close(sock_fd);
    return (EXIT_SUCCESS);
}

char parsePath(char *source, char *destination) {
    char *s, *d;
    s = strchr(source, ':');
    d = strchr(destination, ':');
    if (s != NULL && d == NULL) {
        opCode = OPCODE_DOWNLOAD_TO_CLIENT;
        *s = '\0';
        serverFilePathBuff[0] = opCode;
        strcpy(serverFilePathBuff + 1, s + 1);
        strcpy(clientFilePathBuff, destination);
        
        /* local VM cannot resolve the server IP*/
        strcpy(ipBuff, source);
        //hostname_to_ip(source);
    } else if (s == NULL && d != NULL) {
        opCode = OPCODE_UPLOAD_TO_SERVER;
        *d = '\0';
        serverFilePathBuff[0] = opCode;
        strcpy(serverFilePathBuff + 1, source);
        strcpy(clientFilePathBuff, d + 1);
        
        /* local VM cannot resolve the server IP*/
        strcpy(ipBuff, destination);
        //hostname_to_ip(destination);
    } else {
        printf("invalid file path\n");
        exit(1);
    }
    return opCode;
}

int hostname_to_ip(char *hostName) {
    struct hostent *he;
    struct in_addr **addr_list;
    char buffer[strlen(hostName) + 1];
    int i;
    strcpy(buffer, hostName);
    if ((he = gethostname(buffer)) == NULL) {
        herror("get host by name");
        exit(1);
    }
    addr_list = (struct in_addr*) he->h_addr_list;
    strcpy(ipBuff, inet_ntoa(*addr_list[0]));
    return SUCCESS;
}
