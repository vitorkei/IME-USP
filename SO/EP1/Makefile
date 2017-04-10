CFLAGS = -Wall -ansi -pedantic -O2 -Wno-unused-result
libs = -lreadline -lm -lpthread

all: ep1sh ep1

ep1sh: ep1sh.c
	gcc -o $@ $^ $(CFLAGS) $(libs)

ep1: ep1.c
	gcc -o $@ $^ $(CFLAGS) $(libs)
