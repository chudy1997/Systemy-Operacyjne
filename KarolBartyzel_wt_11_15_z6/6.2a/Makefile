
all:
	make compile run

compile: client.c server.c
	gcc -o server server.c -lrt
	gcc -o client client.c -lrt

run: client server
	./server &
	./client

clean:
	rm server client
