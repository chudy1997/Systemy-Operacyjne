all:
	make compile run

compile:
	gcc -std=c99 -o draw main.c
	gcc -std=c99 -o master master.c
	gcc -std=c99 -o slave slave.c -lm

run:
	./draw fifo 1000 100 10000
	./draw fifo 1000 100 50000
	./draw fifo 500 100 50000
	./draw fifo 1000 200 50000
	./draw fifo 1000 100 1000000
	./draw fifo 1000 100 5000000
	./draw fifo 500 100 5000000
	./draw fifo 1000 200 5000000
	sleep 5
	reset

clean:
	rm draw master slave data
