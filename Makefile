all:
	g++ -Wall -Werror P1.c -o P1 -pthread	



.PHONY: run

run:
	./main 
