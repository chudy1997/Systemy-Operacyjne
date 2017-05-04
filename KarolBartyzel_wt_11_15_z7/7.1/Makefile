all:
	make compile run

compile: barber.c client.c clientBuilder.c
	gcc -o barber barber.c
	gcc -o client client.c
	gcc -o clientBuilder clientBuilder.c

run:
	./barber 5 &
	./clientBuilder 5 4
	sleep 10
	fg
	kill
