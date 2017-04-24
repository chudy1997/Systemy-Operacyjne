all:
	make compile run

compile: client.c server.c testFile.c header.h
	gcc -o server server.c -lrt
	gcc -o client client.c -lrt
	gcc -std=c99 -o testFile testFile.c

run: client server
	./testFile 10

clean:
	rm client server testFile
