#
# makefile for srcp
#

# variable

CFLAGS		=
CC		= gcc

# client variables
OBJS_C		= srcp_clnt.o srcp_util.o
TARGET_C	= srcp_clnt

# client variables
OBJS_S		= srcp_srv.o srcp_util.o
TARGET_S	= srcp_srv

# all targets 
all:		    $(TARGET_C) $(TARGET_S)

# rule to make the client executable
$(TARGET_C):	$(OBJS_C)
	$(CC) $(CFLAGS) -o $(TARGET_C) $(OBJS_C)

# rule to make the server executable
$(TARGET_S):	$(OBJS_S)
	$(CC) $(CFLAGS) -o $(TARGET_S) $(OBJS_S)

# user implicit rule for the .o file
srcp_clnt.o:	    srcp.h
srcp_srv.o:	    srcp.h

# rule to clean up
clean:
	rm -f $(OBJS_S) $(TARGET_S) $(OBJS_C) $(TARGET_C)






