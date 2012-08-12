
CC		:= clang
CLANG_ANALYZER	:= scan-build
FLAGS		:= -std=c99  -Wall -pedantic -Os -Wl,-s #-save-temps
LIBS		:= -pthread


all: clean build

clean:
	rm -f *.s
	rm -f *.i
	rm -f *.o
	rm -f *.map
	rm -f test1
	rm -f test2
	rm -f test3

build:
	$(CLANG_ANALYZER) $(CC) $(FLAGS) -c lock-free-fifo.c
	$(CLANG_ANALYZER) $(CC) $(FLAGS) $(LIBS) lock-free-fifo.o test1.c -o test1
	$(CLANG_ANALYZER) $(CC) $(FLAGS) $(LIBS) lock-free-fifo.o test2.c -o test2
	$(CLANG_ANALYZER) $(CC) $(FLAGS) $(LIBS) lock-free-fifo.o test3.c -o test3

run1: build
	./test1
run2: build
	./test2
run3: build
	./test3
