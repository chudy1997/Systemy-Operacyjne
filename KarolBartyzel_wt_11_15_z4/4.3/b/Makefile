all:
	make compile run

compile: main.c child.c controler.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o child child.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o controler controler.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o main main.c

run: main
	./main 5 3 0
	./main 50 28 0
	./main 5 3 1
	./main 50 28 1
	./main 5 3 2
	./main 50 28 2

clean:
	rm main child controler
