/*
 * ------------------------------------------------------------------------
 *  Name     : test_byte_order
 *  Function : Demonstrate the byte order of an integer
 * ------------------------------------------------------------------------
 */
#include    <stdio.h>
#include    <stdlib.h>

int main(int argc, char **argv)
{
    unsigned int num = 0x1a2b3c4d;
    char *p;

    p = (char *) &num;

    printf("The number is %x (in hex), %u (in decimal)\n", num, num);
    printf("The bytes are (from low addr to high addr) %x %x %x %x\n", 
           *p, *(p+1), *(p+2), *(p+3));
}
