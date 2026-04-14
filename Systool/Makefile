CC = gcc
CFLAGS = -Wall -Werror -g 
OBJS = main.o systool.o draw.o update.o child.o

all: systool

systool: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c systool.h draw.h update.h child.h
	$(CC) $(CFLAGS) -c main.c

systool.o: systool.c systool.h
	$(CC) $(CFLAGS) -c systool.c

draw.o: draw.c draw.h
	$(CC) $(CFLAGS) -c draw.c

update.o: update.c update.h
	$(CC) $(CFLAGS) -c update.c

child.o: child.c child.h systool.h
	$(CC) $(CFLAGS) -c child.c

clean:
	rm -f *.o systool
