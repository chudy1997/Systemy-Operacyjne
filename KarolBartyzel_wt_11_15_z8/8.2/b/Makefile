all:
	make compile run

compile: main.c recordsfile.c
	gcc -o main main.c -lpthread
	gcc -o recordsfile recordsfile.c

run: recordsfile
	./recordsfile

clean:
	rm main recordsfile records.txt
