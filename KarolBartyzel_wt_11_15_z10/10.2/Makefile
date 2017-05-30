FLAGS = -std=gnu99 -pthread

compile: client.c server.c server.h client.h symulator.c
	gcc $(FLAGS) -o client client.c
	gcc $(FLAGS) -o server server.c
	gcc -o symulator symulator.c

clean:
	rm server client symulator
