#
# makefile for srcp
#

# variables
CFLAGS = -g
CC = gcc

# server files
O_SRCP_SRV = srcp_srv.o srcp_util.o
T_SRCP_SRV = srcp_srv

# client files
O_SRCP_CLNT = srcp_clnt.o srcp_util.o
T_SRCP_CLNT = srcp_clnt

# libraries
LIBS = 

# executables
PROGS = $(T_SRCP_SRV) $(T_SRCP_CLNT)
a : $(PROGS)

# .o files are created by default rules
$(T_SRCP_SRV): $(O_SRCP_SRV)
	$(CC) $(CFLAGS) -o $@ $(O_SRCP_SRV) $(LIBS)

$(T_SRCP_CLNT): $(O_SRCP_CLNT)
	$(CC) $(CFLAGS) -o $@ $(O_SRCP_CLNT) $(LIBS)

# other rules 
clean:
	rm -f $(PROGS) *.o core


