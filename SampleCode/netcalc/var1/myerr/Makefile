# the makefile for all the code in this dir

MY_LIB  = libmyerror.a

all:    $(MY_LIB)

CFLAGS  = -g
CC      = gcc

# rule for the library
$(MY_LIB): myerror.o
		ar rv $@ $?

clean:
		rm -f $(MY_LIB) *.o
