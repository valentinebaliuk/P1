all:
	g++ -Wall -Werror test.c -o test -pthread	



.PHONY: run

run:
	./main 
