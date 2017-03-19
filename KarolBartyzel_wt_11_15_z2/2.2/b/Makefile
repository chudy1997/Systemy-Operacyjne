DIRPATH=../..
SIZE=10000
FLAGS=-D_BSD_SOURCE -std=c99

all: nftwMain.c
	gcc $(FLAGS) -o nftwMain nftwMain.c
	./nftwMain $(DIRPATH) $(SIZE)
