/*
 * ------------------------------------------------------------------------
 *  Name     : test_srcp_filename
 *  Function : Test the parsing of the file name for srcp
 *  Usage    : test_srcp_filename filename
 * ------------------------------------------------------------------------
 */
#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

int main(int argc, char **argv)
{
    char *addr, *name;
    char *p;

    if (argc !=2 ) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    p = strchr(argv[1], ':');
    if (p == NULL) {
        printf("Invalid srcp file name: %s\n", argv[1]);
    } else {
        *p = 0; 
        addr = argv[1];
        name = p+1;
        printf("Host address: %s\n", addr);
        printf("File name: %s\n", name);
    }
}
