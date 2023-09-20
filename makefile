simpleshell: simpleshell.o 
	g++ -o simpleshell simpleshell.o 
simpleshell.o: simpleshell.c
	g++ -c simpleshell.c
clean:
	rm *.o simpleshell