all:
	make compile run

compile: main.c child.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o child child.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o main main.c

run: main
	./main 10 1
	./main 10 2
	./main 10 3
	./main 50 1
	./main 50 2
	./main 50 3

clean:
	rm main child
