IDIR=./inc
CC=gcc
CFLAGS=-I$(IDIR)
CLIBS=-lncurses
EDIR=./exec

SRC_M = src/master/*.c src/utils/connection/*.c src/utils/*.c
SRC_S = src/slave/*.c src/utils/connection/*.c src/utils/*.c

all: master slave

master: $(SRC_M)
	$(CC) -o $(EDIR)/$@ $^ $(CFLAGS) $(CLIBS)

slave: $(SRC_S)
	$(CC) -o $(EDIR)/$@ $^ $(CFLAGS) 

clean:
	rm -f ./exec/master *~ *.o
	rm -f ./exec/slave *~ *.o 