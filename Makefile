CC=gcc
CFLAGS=-Wall -g
OBJS=main.o functions.o

all: valhalla-c

valhalla-c: $(OBJS)
	$(CC) $(CFLAGS) -o valhalla-c $(OBJS)

main.o: main.c headers.h
	$(CC) $(CFLAGS) -c main.c

functions.o: functions.c headers.h
	$(CC) $(CFLAGS) -c functions.c

clean:
	rm -f *.o valhalla-c