CC = gcc

all: test.c libcb.a
	ulimit -c unlimited
	$(CC) -Wall -std=c99 -static -g test.c -L. -lcb -o staticTest
	echo  test1
	./staticTest

libcb.a: libcontact.o liblist.o libtree.o
	ar rcs libcb.a libcontact.o liblist.o libtree.o

libcontact.o: libcontact.c libcontact.h
	$(CC) -Wall -c -fpic libcontact.c -o libcontact.o

liblist.o: liblist.c liblist.h
	$(CC) -Wall -c -fpic liblist.c -o liblist.o

libtree.o: libtree.c libtree.h
	$(CC) -Wall -c -fpic libtree.c -o libtree.o

clean:
	rm -f *.o libcb.a staticTest
