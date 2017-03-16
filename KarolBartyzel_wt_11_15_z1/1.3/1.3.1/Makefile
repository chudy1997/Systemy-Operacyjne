CC = gcc

all: test.c libcb.a libcb.so
	$(CC) -Wall -std=c99 -static test.c -L. -lcb -o staticTest
	$(CC) -Wall -std=c99 -L. -o sharedTest test.c -lcb
	$(CC) -Wall -std=c99 testDynam.c -ldl -o dynamTest
	echo without optymalization >> results.txt
	echo  test1 >> results.txt
	./staticTest >> results.txt
	echo test2 >> results.txt
	./sharedTest >> results.txt
	echo test3 >> results.txt
	./dynamTest >> results.txt

libcb.so:	libcontact.o liblist.o libtree.o
	$(CC) -Wall -shared -o libcb.so libcontact.o liblist.o libtree.o

libcb.a: libcontact.o liblist.o libtree.o
	ar rcs libcb.a libcontact.o liblist.o libtree.o

libcontact.o: libcontact.c libcontact.h
	$(CC) -Wall -c -fpic libcontact.c -o libcontact.o

liblist.o: liblist.c liblist.h
	$(CC) -Wall -c -fpic liblist.c -o liblist.o

libtree.o: libtree.c libtree.h
	$(CC) -Wall -c -fpic libtree.c -o libtree.o

clean:
	rm -f *.o *.so libcb.a staticTest sharedTest dynamTest results.txt
