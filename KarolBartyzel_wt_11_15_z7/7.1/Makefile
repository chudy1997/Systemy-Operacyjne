all:
	make compile run

compile: barber.c client.c clientBuilder.c
	gcc -o barber barber.c
	gcc -o client client.c
	gcc -o clientBuilder clientBuilder.c

clean:
	rm barber client clientBuilder
