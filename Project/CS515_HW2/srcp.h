/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   srcp.h
 * Author: johnson5414
 *
 * Created on February 15, 2016, 2:44 AM
 */
/* server port number */
#define SRCP_SERV_PORT  30010

/* opcode */
#define OPCODE_UPLOAD 'U'
#define OPCODE_DOWNLOAD 'D'
#define OPCODE_REJECT 'X'

/* request terminator */
#define REQ_TERMINATOR '='

/* buff size */
#define BUF_SIZE 2048

/* misc. defines */
#define SUCCESS 1
#define NO_SUCESS 0


/* application layer functions */
extern int doTransfer(int fd, char *filePath);
extern int doReceive(int fd, char *filePath);




