# makefile for the Tetra parser

CC = clang++
CFLAGS = -W -Wall -g

all: dump-tree tags
	@echo "All done :)"

dump-tree: parser.gen.o lexer.o dump-tree.o types.o tetra.hpp node.o error.o
	$(CC) $(CFLAGS) parser.gen.o lexer.o dump-tree.o types.o node.o error.o -o dump-tree

dump-tree.o: dump-tree.cpp tetra.hpp
	$(CC) $(CFLAGS) -c dump-tree.cpp

types.o: types.cpp tetra.hpp
	$(CC) $(CFLAGS) -c types.cpp

node.o: node.cpp tetra.hpp
	$(CC) $(CFLAGS) -c node.cpp

error.o: error.cpp tetra.hpp
	$(CC) $(CFLAGS) -c error.cpp

parser.gen.o: parser.gen.cpp
	$(CC) $(CFLAGS) -c parser.gen.cpp

parser.gen.cpp: parser.y tetra.hpp
	bison -d -v parser.y -o parser.gen.cpp

lexer.o: lexer.cpp tetra.hpp
	$(CC) $(CFLAGS) -c lexer.cpp

.PHONY: clean all

clean:
	rm -f parser.gen.cpp parser.gen.hpp parser.gen.output dump-tree *.o tags

tags: tetra.hpp types.cpp parser.y lexer.cpp dump-tree.cpp node.cpp error.cpp
	ctags -R *.cpp *.hpp

