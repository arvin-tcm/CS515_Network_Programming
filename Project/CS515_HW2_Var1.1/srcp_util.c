/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <fcntl.h>
#include <stdio.h>
#include "srcp.h"

int doTransfer(int fd, char *filePath) {
    int in_fd, rd_count, wr_count;
    char buffer[BUF_SIZE + 10];
    /* open the input file */
    in_fd = open(filePath, O_RDONLY);
    if (in_fd < 0) {
        printf("open input file failed: %s\n", filePath);
        exit(1);
    }
    /* the write loop */
    while (1) {
        /* read a block from file */
        rd_count = read(in_fd, buffer + 1, BUF_SIZE - 1);
        buffer[0] = REQ_DEFAULT;
        printf("packet: %s\n", buffer);
        if (rd_count < BUF_SIZE - 1) {
            buffer[0] = REQ_TERMINATOR;
            buffer[rd_count + 1] = '\0';
            printf("last packet: %s\n", buffer);
            write(fd, buffer, strlen(buffer));
            break; /* end of the file */
        }
        /* write a block to socket */
        wr_count = write(fd, buffer, BUF_SIZE);
        if (wr_count < 0) {
            perror("write failed\n");
            exit(1);
        }
        printf("packet send\n");
    }
    close(in_fd);
    return SUCCESS;
}

int doReceive(int fd, char *filePath) {
    int out_fd, rd_count, wr_count;
    char buffer[BUF_SIZE + 10];
    /* open the output file */
    out_fd = open(filePath, O_WRONLY | O_CREAT);
    if (out_fd < 0) {
        printf("open output file failed: %s\n", filePath);
        exit(1);
    }
    /* the read loop */
    while (1) {
        /* read a block from socket */
        rd_count = read(fd, buffer, BUF_SIZE);
        printf("packet: %s\n", buffer);
        if (buffer[0] == REQ_TERMINATOR) {
            write(out_fd, buffer + 1, rd_count - 1);
            break;
        }
        wr_count = write(out_fd, buffer + 1, rd_count - 1);
        if (wr_count < 0) {
            perror("write failed\n");
            exit(1);
        }
    }
    close(out_fd);
    return SUCCESS;
}