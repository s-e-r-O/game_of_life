IDIR=./inc
CC=gcc
CFLAGS=-I$(IDIR)
CLIBS=-lncurses

SRC_M = src/master/*.c src/utils/connection/*.c src/utils/*.c
SRC_S = src/slave/*.c src/utils/connection/*.c src/utils/*.c

all: master slave

master: $(SRC_M)
	$(CC) -o $@ $^ $(CFLAGS) $(CLIBS)

slave: $(SRC_S)
	$(CC) -o $@ $^ $(CFLAGS) 

clean:
	rm -f master *~ *.o
	rm -f slave *~ *.o 