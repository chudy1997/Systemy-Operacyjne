
all:
	make compile run

compile: client.c server.c
	gcc -o server server.c
	gcc -o client client.c

run: client server
	./server &
	./client
