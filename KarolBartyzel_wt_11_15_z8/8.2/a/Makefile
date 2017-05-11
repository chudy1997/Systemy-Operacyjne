all:
	make compile run

compile: main1.c main2.c main3.c main4.c main5.c recordsfile.c
	gcc -o main1 main1.c -lpthread
	gcc -o main2 main2.c -lpthread
	gcc -o main3 main3.c -lpthread
	gcc -o main4 main4.c -lpthread
	gcc -o main5 main5.c -lpthread
	gcc -o recordsfile recordsfile.c

run: recordsfile
	./recordsfile

clean:
	rm main1 main2 main3 main4 main5 recordsfile records.txt
