CC=gcc
CFLAGS= -Wall -g 

all: 6state prot bb84 bb84x skqd
	rm *.o

skqd: skqd.o
	$(CC) $(CFLAGS) $< -o $@  -lm

6state: 6state.o
	$(CC) $(CFLAGS) $< -o $@  -lm

bb84: bb84.o
	$(CC) $(CFLAGS) $< -o $@  -lm

bb84x: bb84x.o
	$(CC) $(CFLAGS) $< -o $@  -lm

prot: prot.o
	$(CC) $(CFLAGS) $< -o $@  -lm

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@  -lm

clean:
	rm -rf  bb84 bb84x 6state prot skqd *.o 


