all:
	make compile run

compile: barber.c client.c clientBuilder.c
	gcc -o barber barber.c -pthread -lrt
	gcc -o client client.c -pthread -lrt
	gcc -o clientBuilder clientBuilder.c

clean:
	rm barber client clientBuilder
