
all: clean build run

clean:
	rm -f *.o
	rm -f test1
	rm -f test2

build:
	scan-build gcc -std=c99 -ansi -Wall -pedantic -c lock-free-fifo.c
	scan-build gcc -std=c99 -ansi -Wall -pedantic -lpthread lock-free-fifo.c test1.c -o test1
	scan-build gcc -std=c99 -ansi -Wall -pedantic -lpthread lock-free-fifo.c test2.c -o test2
	strip -s test1
	strip -s test2

run:
	./test1
