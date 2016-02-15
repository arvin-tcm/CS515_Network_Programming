#
# makefile for a few simple test programs
# 
# Author : Bin Zhang
#

all:    test_inet_ntoa \
        test_inet_pton \
        test_srcp_filename \
        test_byte_order \
        file_cp \
        async_io

# rule to make the executable
test_inet_ntoa:    test_inet_ntoa.c
	gcc -o test_inet_ntoa test_inet_ntoa.c 

test_inet_pton:    test_inet_pton.c
	gcc -o test_inet_pton test_inet_pton.c 

async_io:    async_io.c
	gcc -o async_io async_io.c

file_cp:    file_cp.c
	gcc -o file_cp file_cp.c

test_srcp_filename:    test_srcp_filename.c
	gcc -o test_srcp_filename test_srcp_filename.c

test_byte_order:    test_byte_order.c
	gcc -o test_byte_order test_byte_order.c

# rule to clean up
clean:
	rm -f test_inet_ntoa async_io test_inet_pton file_cp test_srcp_filename \
          test_byte_order
