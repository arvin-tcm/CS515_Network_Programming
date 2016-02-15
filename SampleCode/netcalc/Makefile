#
#  makefile for netcalc (for all the versions)
#

# common source files
S_COMMON_SOC  = netcalc_util_soc.c
S_COM_SRV     = netcalc_app_srv.c $(S_COMMON_SOC)
S_COM_CLNT    = netcalc_app_clnt.c $(S_COMMON_SOC)

#
# servers macros
#

# version 1 server files
S_NETCALC_SRV = netcalc_srv.c $(S_COM_SRV)
O_NETCALC_SRV = $(S_NETCALC_SRV:.c=.o)
T_NETCALC_SRV = netcalc_srv

# version 2 server files
S_NETCALC_SRV_V2 = netcalc_srv_v2.c netcalc_srv_main.c $(S_COM_SRV)
O_NETCALC_SRV_V2 = $(S_NETCALC_SRV_V2:.c=.o)
T_NETCALC_SRV_V2 = netcalc_srv_v2

# version 3 server files
S_NETCALC_SRV_V3 = netcalc_srv_v3.c netcalc_srv_main.c $(S_COM_SRV)
O_NETCALC_SRV_V3 = $(S_NETCALC_SRV_V3:.c=.o)
T_NETCALC_SRV_V3 = netcalc_srv_v3

# version 4 server files
S_NETCALC_SRV_V4 = netcalc_srv_v4.c netcalc_srv_main.c $(S_COM_SRV)
O_NETCALC_SRV_V4 = $(S_NETCALC_SRV_V4:.c=.o)
T_NETCALC_SRV_V4 = netcalc_srv_v4

# version 5 server files
S_NETCALC_SRV_V5 = netcalc_srv_v5.c netcalc_srv_main.c $(S_COM_SRV)
O_NETCALC_SRV_V5 = $(S_NETCALC_SRV_V5:.c=.o)
T_NETCALC_SRV_V5 = netcalc_srv_v5

# version 6 server files
S_NETCALC_SRV_V6 = netcalc_srv_v6.c $(S_COM_SRV)
O_NETCALC_SRV_V6 = $(S_NETCALC_SRV_V6:.c=.o)
T_NETCALC_SRV_V6 = netcalc_srv_v6

#
# clients macros
#

# version 1 client files
S_NETCALC_CLNT = netcalc_clnt.c $(S_COM_CLNT)
O_NETCALC_CLNT = $(S_NETCALC_CLNT:.c=.o)
T_NETCALC_CLNT = netcalc_clnt

# version 2 client files
S_NETCALC_CLNT_V2 = netcalc_clnt_v2.c netcalc_clnt_main.c $(S_COM_CLNT)
O_NETCALC_CLNT_V2 = $(S_NETCALC_CLNT_V2:.c=.o)
T_NETCALC_CLNT_V2 = netcalc_clnt_v2

# version 3 client files
S_NETCALC_CLNT_V3 = netcalc_clnt_v3.c netcalc_clnt_main.c $(S_COM_CLNT)
O_NETCALC_CLNT_V3 = $(S_NETCALC_CLNT_V3:.c=.o)
T_NETCALC_CLNT_V3 = netcalc_clnt_v3

# version 4 client files
S_NETCALC_CLNT_V4 = netcalc_clnt_v4.c netcalc_clnt_main.c $(S_COM_CLNT)
O_NETCALC_CLNT_V4 = $(S_NETCALC_CLNT_V4:.c=.o)
T_NETCALC_CLNT_V4 = netcalc_clnt_v4

# UDP client files
S_NETCALC_CLNT_UDP = netcalc_clnt_udp.c $(S_COM_CLNT)
O_NETCALC_CLNT_UDP = $(S_NETCALC_CLNT_UDP:.c=.o)
T_NETCALC_CLNT_UDP = netcalc_clnt_udp

# X test file
S_NETCALC_CLNT_X = x.c $(S_COM_CLNT)
O_NETCALC_CLNT_X = $(S_NETCALC_CLNT_X:.c=.o)
T_NETCALC_CLNT_X = x

#
# testers
#

# test files
S_NETCALC_TEST = netcalc_test.c
O_NETCALC_TEST = $(S_NETCALC_TEST:.c=.o)
T_NETCALC_TEST = netcalc_test

# location of the base for header files and libraries
BASE_DIR= ../unpv13e

# compiler and compile flag
CFLAGS  = -g
INC_DIRS= -I$(BASE_DIR)/lib
CC      = gcc $(INC_DIRS)

# libraries
COM_LIB  = -lresolv -lnsl -lpthread # lsocket
BOOK_LIB = $(BASE_DIR)/libunp.a $(COM_LIB)

PROGS	= $(T_NETCALC_SRV) $(T_NETCALC_SRV_V2) $(T_NETCALC_SRV_V3) \
          $(T_NETCALC_SRV_V4) $(T_NETCALC_SRV_V5) $(T_NETCALC_SRV_V6) \
          $(T_NETCALC_CLNT) $(T_NETCALC_CLNT_V2) $(T_NETCALC_CLNT_V3) \
          $(T_NETCALC_CLNT_V4) \
          $(T_NETCALC_CLNT_UDP) $(T_NETCALC_CLNT_X)\
          $(T_NETCALC_TEST)

all     : $(PROGS)  

# server rules
$(T_NETCALC_SRV):   $(O_NETCALC_SRV)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV) $(BOOK_LIB)

$(T_NETCALC_SRV_V2):   $(O_NETCALC_SRV_V2)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV_V2) $(BOOK_LIB)

$(T_NETCALC_SRV_V3):   $(O_NETCALC_SRV_V3)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV_V3) $(BOOK_LIB)

$(T_NETCALC_SRV_V4):   $(O_NETCALC_SRV_V4)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV_V4) $(BOOK_LIB)

$(T_NETCALC_SRV_V5):   $(O_NETCALC_SRV_V5)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV_V5) $(BOOK_LIB)

$(T_NETCALC_SRV_V6):   $(O_NETCALC_SRV_V6)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV_V6) $(BOOK_LIB)

# client rules

$(T_NETCALC_CLNT):   $(O_NETCALC_CLNT)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT) $(BOOK_LIB)

$(T_NETCALC_CLNT_V2):   $(O_NETCALC_CLNT_V2)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT_V2) $(BOOK_LIB)

$(T_NETCALC_CLNT_V3):   $(O_NETCALC_CLNT_V3)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT_V3) $(BOOK_LIB)

$(T_NETCALC_CLNT_V4):   $(O_NETCALC_CLNT_V4)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT_V4) $(BOOK_LIB)

$(T_NETCALC_CLNT_UDP):   $(O_NETCALC_CLNT_UDP)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT_UDP) $(BOOK_LIB)

$(T_NETCALC_CLNT_X):   $(O_NETCALC_CLNT_X)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT_X) $(BOOK_LIB)

# tester rules
$(T_NETCALC_TEST):   $(O_NETCALC_TEST)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_TEST) $(BOOK_LIB)

# other rules
clean:          
	rm -f $(PROGS) *.o core
