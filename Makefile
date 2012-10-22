################################################################################
#                                                                              #
#                               Template Makefile                              #
#                                                                              #
################################################################################

CC=gcc
EXEC_SRC=main.c pim.c
CFLAGS=-W -Wall -std=c99
EXE_OUT=pim-reader
LDFLAGS=-lSDL

all:
	$(CC) $(CFLAGS) $(EXEC_SRC) $(LDFLAGS) -o $(EXE_OUT) 

clean:
	rm *.o
	
.PHONY: clean
