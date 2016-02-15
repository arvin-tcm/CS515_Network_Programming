/*
 * ------------------------------------------------------------------------
 *  Name     : test_inet_nota.
 *  Function : Test the use of inet_nota.
 * ------------------------------------------------------------------------
 */
#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>

int
main(int argc, char **argv)
{
	struct in_addr	addr;

    addr.s_addr = 0x010203ff;

    printf("The address is %s\n", inet_ntoa(addr));

	exit(0);
}
