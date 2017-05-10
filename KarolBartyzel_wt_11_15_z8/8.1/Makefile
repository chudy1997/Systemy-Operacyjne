all:
	make compile run

compile: thrFindWord1.c thrFindWord2.c thrFindWord3.c recordsfile.c
	gcc -o thrFindWord1 thrFindWord1.c -lpthread
	gcc -o thrFindWord2 thrFindWord2.c -lpthread
	gcc -o thrFindWord3 thrFindWord3.c -lpthread
	gcc -o recordsfile recordsfile.c

run: recordsfile
	./recordsfile

clean:
	rm thrFindWord1 thrFindWord2 thrFindWord3 recordsfile records.txt
