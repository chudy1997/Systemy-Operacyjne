CC = gcc

all: libcontact.o liblist.o libtree.o
	ar rcs libcb.a libcontact.o liblist.o libtree.o
	$(CC) -shared -o libcb.so libcontact.o liblist.o libtree.o

libcontact.o: libcontact.c libcontact.h
	$(CC) -c -fpic libcontact.c -o libcontact.o

liblist.o: liblist.c liblist.h
	$(CC) -c -fpic liblist.c -o liblist.o

libtree.o: libtree.c libtree.h
	$(CC) -c -fpic libtree.c -o libtree.o

clean:
	rm -f *.o *.so libcb.a
