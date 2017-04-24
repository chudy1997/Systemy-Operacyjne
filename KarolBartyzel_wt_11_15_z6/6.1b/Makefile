all:
	make compile run

compile: client.c server.c testFile.c header.h
	gcc -o server server.c
	gcc -o client client.c
	gcc -std=c99 -o testFile testFile.c

run: client server
	./testFile 10

clean:
	rm client server testFile
