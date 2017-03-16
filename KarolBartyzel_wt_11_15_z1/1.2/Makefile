CC = gcc

all: test.c libcb.a
	$(CC) -Wall -std=c99 -static test.c -L. -lcb -o staticTest
	./staticTest

libcb.a: libcontact.o liblist.o libtree.o
	ar rcs libcb.a libcontact.o liblist.o libtree.o

libcontact.o: libcontact.c libcontact.h
	$(CC) -Wall -c -O$(OPT) -fpic libcontact.c -o libcontact.o

liblist.o: liblist.c liblist.h
	$(CC) -Wall -c -O$(OPT) -fpic liblist.c -o liblist.o

libtree.o: libtree.c libtree.h
	$(CC) -Wall -c -O$(OPT) -fpic libtree.c -o libtree.o

clean:
	rm -f *.o libcb.a staticTest
