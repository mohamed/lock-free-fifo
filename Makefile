
all: clean build run

clean:
	rm test1
	rm test2

build:
	gcc -pedantic -Wall -lpthread lock-free-fifo.c test1.c -o test1
	gcc -pedantic -Wall -lpthread lock-free-fifo.c test2.c -o test2

run:
	./test1
