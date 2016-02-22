/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp.h
 * Author: johnson5414
 *
 * Created on February 18, 2016, 9:04 PM
 */

/* server port number */
#define SRCP_SERV_PORT 51513

/* opcode */
#define OPCODE_UPLOAD_TO_SERVER 'U'
#define OPCODE_DOWNLOAD_TO_CLIENT 'D'
#define OPCODE_REJECT 'R'

/* request terminator */
#define REQ_TERMINATOR 'T'
#define REQ_DEFAULT 'D'

/* buff size */
#define BUF_SIZE 16

#define SUCCESS 1
#define NOT_SUCCESS 0

/* application layer functions */
extern int doTransfer(int fd, char *filePath);
extern int doReceive(int fd, char *filePath);
