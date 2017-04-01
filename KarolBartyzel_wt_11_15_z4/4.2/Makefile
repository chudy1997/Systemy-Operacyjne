all:
	make compile run

compile: main.c child.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o child child.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o controler controler.c
	gcc -D_POSIX_C_SOURCE -std=c99 -o main main.c

run: main
	./main 5 3
	./main 15 7
	./main 25 17

clean:
	rm child controler main
