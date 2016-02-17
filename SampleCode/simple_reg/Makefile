#
# makefile for simple_reg
# 

# variables
CFLAGS  	=
CC      	= gcc

# client variables
OBJS_C    	= sreg_clnt.o
TARGET_C 	= sreg_clnt

# server variables
OBJS_S    	= sreg_srv.o
TARGET_S 	= sreg_srv

# all targets
all:    		$(TARGET_C) $(TARGET_S)

# rule to make the client executable
$(TARGET_C):    $(OBJS_C)
	$(CC) $(CFLAGS) -o $(TARGET_C) $(OBJS_C) 

# rule to make the server executable
$(TARGET_S):    $(OBJS_S)
	$(CC) $(CFLAGS) -o $(TARGET_S) $(OBJS_S)

# use implicit rule for the .o files
sreg_clnt.o:        sreg.h
sreg_srv.o:         sreg.h

# rule to clean up
clean:
	rm -f $(OBJS_S) $(TARGET_S) $(OBJS_C) $(TARGET_C)
