CFLAGS= -g -Wall
CC=gcc

main:main.o database.o linkedlist.o alter.o  strlib.o bhy.o calc.o select.o compute_str.o
	$(CC) $^ -o $@ $(CFLAGS)
main.o:main.c database.h
	$(CC) -c main.c $(CFLAGS)
database.o:database.c database.h
	$(CC) $(CFLAGS) -c database.c
linkedlist.o:linkedlist.c linkedlist.h
	$(CC) $(CFLAGS) -c linkedlist.c
alter.o:alter.c 
	$(CC) $(CFLAGS) -c alter.c
bhy.o:bhy.c database.h
	$(CC) $(CFLAGS) -c bhy.c
strlib.o:strlib.c strlib.h
	$(CC) $(CFLAGS) -c strlib.c
calc.o:calc.c
	$(CC) $(CFLAGS) -c calc.c
compute_str.o:compute_str.c
	$(CC) $(CFLAGS) -c compute_str.c

test:test.o strlib.o linkedlist.o
	$(CC) $^ -o $@ $(CFLAGS)
test.o:test.c
	$(CC)  -c test.c

.PHONY:clean

clean:
	rm *.o main linkedlist database 
