/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include<fcntl.h>
#include<stdio.h>
#include "srcp.h"

int doTransfer(int fd, char *filePath) {
    int in_fd, rd_count, wr_count;
    char buffer[BUF_SIZE];

    /* open the input file */
    in_fd = open(filePath, O_RDONLY);
    if (in_fd < 0) {
        printf("open failed: %s\n", filePath);
        exit(1);
    }

    /* the copy loop */
    while (1) {
        /* read a block */
        rd_count = read(in_fd, buffer + 1, BUF_SIZE - 1);
        buffer[0] = '!';
        if (rd_count < 0) {
            write(fd, buffer, 1);
            break; /* end of a file*/
        }
        /* write a block */
        wr_count = write(fd, buffer, rd_count);
        if (wr_count < 0) {
            perror("write failed\n");
            exit(1);
        }
    }
    close(in_fd);
    return SUCCESS;
}

int doReceive(int fd, char *filePath) {
    int out_fd, rd_count, wr_count;
    char buffer[BUF_SIZE];
    /* open the output file */
    out_fd = open(filePath, O_WRONLY | O_CREAT, 6044);
    if (out_fd < 0) {
        printf("open failed: %s\n", filePath);
        exit(1);
    }

    while (1) {
        /* read a block */
        rd_count = read(fd, buffer, BUF_SIZE);
        if (buffer[0] == '=') {
            break;
        }

        wr_count = write(out_fd, buffer + 1, rd_count - 1);
        if (wr_count < 0) {
            perror("write error\n");
            exit(1);
        }
    }
    close(out_fd);
    return SUCCESS;
}
