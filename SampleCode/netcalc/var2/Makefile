#
#  makefile for netcalc version 1 (no wrapper functions !)
#

# server files
O_NETCALC_SRV = netcalc_srv.o netcalc_app_srv.o netcalc_util.o
T_NETCALC_SRV = netcalc_srv

# client files
O_NETCALC_CLNT = netcalc_clnt.o netcalc_app_clnt.o netcalc_util.o
T_NETCALC_CLNT = netcalc_clnt

# compiler and compile flag
CFLAGS  = -g
CC      = gcc 

# libraries
LIBS     = 

# executables
PROGS	= $(T_NETCALC_SRV) $(T_NETCALC_CLNT)
all     : $(PROGS)  

# .o files are created by default rules

# executable rules
$(T_NETCALC_SRV):   $(O_NETCALC_SRV)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_SRV) $(LIBS)

$(T_NETCALC_CLNT):   $(O_NETCALC_CLNT)
	$(CC) $(CFLAGS) -o $@ $(O_NETCALC_CLNT) $(LIBS)

# other rules
clean:          
	rm -f $(PROGS) *.o core
