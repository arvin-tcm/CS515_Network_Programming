/*
 * ------------------------------------------------------------------------
 *  Name     : test_inet_pton
 *  Function : Test the use of inet_pton
 * ------------------------------------------------------------------------
 */
#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>

int main(int argc, char **argv)
{
    struct in_addr	in_addr;
    static char *addr = "192.168.0.38";

    if (inet_pton(AF_INET, addr, &in_addr) <= 0) {
        printf("inet_pton failed\n");
        exit(1);
    }

    printf("the ip address is 0x%x\n", in_addr.s_addr);
    printf("the ip address is %d\n", in_addr.s_addr);
}
