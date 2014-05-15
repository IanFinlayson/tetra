# makefile for the Tetra parser

CC = clang++
CFLAGS = -W -Wall -g

dump-tree: parser.gen.o lexer.o dump-tree.o tetra.o tetra.hpp
	$(CC) $(CFLAGS) parser.gen.o lexer.o dump-tree.o tetra.o -o dump-tree

dump-tree.o: dump-tree.cpp tetra.hpp
	$(CC) $(CFLAGS) -c dump-tree.cpp

tetra.o: tetra.cpp tetra.hpp
	$(CC) $(CFLAGS) -c tetra.cpp

parser.gen.o: parser.gen.cpp
	$(CC) $(CFLAGS) -c parser.gen.cpp

parser.gen.cpp: parser.y tetra.hpp
	bison -d -v parser.y -o parser.gen.cpp

lexer.o: lexer.cpp tetra.hpp
	$(CC) $(CFLAGS) -c lexer.cpp

.PHONY: clean tags

clean:
	rm -f parser.gen.cpp parser.gen.hpp dump-tree *.o

tags:
	ctags -R *.cpp *.hpp

