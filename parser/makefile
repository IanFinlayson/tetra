# makefile for the Tetra parser

CC = clang++
CFLAGS = -W -Wall -g

all: dump-tree tags
	@echo "All done :)"

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

.PHONY: clean all

clean:
	rm -f parser.gen.cpp parser.gen.hpp parser.gen.output dump-tree *.o

tags: tetra.hpp tetra.cpp parser.y lexer.cpp dump-tree.cpp 
	ctags -R *.cpp *.hpp

