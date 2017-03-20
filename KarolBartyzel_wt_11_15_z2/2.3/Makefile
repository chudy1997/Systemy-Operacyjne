
all:
	make compile
	make run

compile: interactiveLocker.c
	gcc -std=c99 -o intLocker interactiveLocker.c

run: intLocker
	./intLocker sampleFile.txt
