CC = gcc
OPT=fast

all: test.c libcb.a libcb.so
	$(CC) -Wall -std=c99 -static test.c -L. -lcb -o staticTest
	$(CC) -Wall -std=c99 -L. -o sharedTest test.c -lcb
	$(CC) -Wall -std=c99 testDynam.c -ldl -o dynamTest
	echo optymalization=$(OPT) >> results$(OPT).txt
	echo  test1 >> results$(OPT).txt
	./staticTest >> results$(OPT).txt
	echo test2 >> results$(OPT).txt
	./sharedTest >> results$(OPT).txt
	echo test3 >> results$(OPT).txt
	./dynamTest >> results$(OPT).txt

libcb.so:	libcontact.o liblist.o libtree.o
	$(CC) -Wall -shared -o libcb.so libcontact.o liblist.o libtree.o

libcb.a: libcontact.o liblist.o libtree.o
	ar rcs libcb.a libcontact.o liblist.o libtree.o

libcontact.o: libcontact.c libcontact.h
	$(CC) -Wall -c -O$(OPT) -fpic libcontact.c -o libcontact.o

liblist.o: liblist.c liblist.h
	$(CC) -Wall -c -O$(OPT) -fpic liblist.c -o liblist.o

libtree.o: libtree.c libtree.h
	$(CC) -Wall -c -O$(OPT) -fpic libtree.c -o libtree.o

clean:
	rm -f *.o *.so libcb.a staticTest sharedTest dynamTest results*.txt
