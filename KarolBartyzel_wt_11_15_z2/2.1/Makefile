AMO1=600
SIZ1=4
AMO2=1200
SIZ2=4
AMO3=500
SIZ3=512
AMO4=1000
SIZ4=512
AMO5=450
SIZ5=4096
AMO6=900
SIZ6=4096
AMO7=400
SIZ7=8192
AMO8=800
SIZ8=8192



all: comparator.o
	./comparator sys generate records.txt $(AMO1) $(SIZ1) > results.txt
	./comparator sys sort records.txt $(AMO1) $(SIZ1) >> results.txt
	./comparator lib sort records.txt $(AMO1) $(SIZ1) >> results.txt
	./comparator sys shuffle records.txt $(AMO1) $(SIZ1) >> results.txt
	./comparator lib shuffle records.txt $(AMO1) $(SIZ1) >> results.txt

		./comparator sys generate records.txt $(AMO2) $(SIZ2) >> results.txt
		./comparator sys sort records.txt $(AMO2) $(SIZ2) >> results.txt
		./comparator lib sort records.txt $(AMO2) $(SIZ2) >> results.txt
		./comparator sys shuffle records.txt $(AMO2) $(SIZ2) >> results.txt
		./comparator lib shuffle records.txt $(AMO2) $(SIZ2) >> results.txt

		./comparator sys generate records.txt $(AMO3) $(SIZ3) >> results.txt
		./comparator sys sort records.txt $(AMO3) $(SIZ3) >> results.txt
		./comparator lib sort records.txt $(AMO3) $(SIZ3) >> results.txt
		./comparator sys shuffle records.txt $(AMO3) $(SIZ3) >> results.txt
		./comparator lib shuffle records.txt $(AMO3) $(SIZ3) >> results.txt

		./comparator sys generate records.txt $(AMO4) $(SIZ4) >> results.txt
		./comparator sys sort records.txt $(AMO4) $(SIZ4) >> results.txt
		./comparator lib sort records.txt $(AMO4) $(SIZ4) >> results.txt
		./comparator sys shuffle records.txt $(AMO4) $(SIZ4) >> results.txt
		./comparator lib shuffle records.txt $(AMO4) $(SIZ4) >> results.txt

		./comparator sys generate records.txt $(AMO5) $(SIZ5) >> results.txt
		./comparator sys sort records.txt $(AMO5) $(SIZ5) >> results.txt
		./comparator lib sort records.txt $(AMO5) $(SIZ5) >> results.txt
		./comparator sys shuffle records.txt $(AMO5) $(SIZ5) >> results.txt
		./comparator lib shuffle records.txt $(AMO5) $(SIZ5) >> results.txt

		./comparator sys generate records.txt $(AMO6) $(SIZ6) >> results.txt
		./comparator sys sort records.txt $(AMO6) $(SIZ6) >> results.txt
		./comparator lib sort records.txt $(AMO6) $(SIZ6) >> results.txt
		./comparator sys shuffle records.txt $(AMO6) $(SIZ6) >> results.txt
		./comparator lib shuffle records.txt $(AMO6) $(SIZ6) >> results.txt

		./comparator sys generate records.txt $(AMO7) $(SIZ7) >> results.txt
		./comparator sys sort records.txt $(AMO7) $(SIZ7) >> results.txt
		./comparator lib sort records.txt $(AMO7) $(SIZ7) >> results.txt
		./comparator sys shuffle records.txt $(AMO7) $(SIZ7) >> results.txt
		./comparator lib shuffle records.txt $(AMO7) $(SIZ7) >> results.txt

		./comparator sys generate records.txt $(AMO8) $(SIZ8) >> results.txt
		./comparator sys sort records.txt $(AMO8) $(SIZ8) >> results.txt
		./comparator lib sort records.txt $(AMO8) $(SIZ8) >> results.txt
		./comparator sys shuffle records.txt $(AMO8) $(SIZ8) >> results.txt
		./comparator lib shuffle records.txt $(AMO8) $(SIZ8) >> results.txt
		make clean


comparator.o: comparator.c
	gcc -std=c99 -o comparator comparator.c

clean:
	 rm records.txt copy.txt
