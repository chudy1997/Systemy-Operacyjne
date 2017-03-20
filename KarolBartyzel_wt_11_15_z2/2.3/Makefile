
all:
	make compile
	make run

compile: intLocker.c
	gcc -std=c99 -o intLocker intLocker.c

run: intLocker
	./intLocker sampleFile.txt
