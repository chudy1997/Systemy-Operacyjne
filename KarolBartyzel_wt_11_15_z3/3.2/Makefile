
all:
	make compile run

run:
	./main batch.txt 5 100

compile: main.c
	gcc -std=c99 -o main main.c
	gcc -std=c99 -o test test.c

clean:
	rm main test
