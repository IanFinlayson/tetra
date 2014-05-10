#!/bin/sh

CC = g++
CFLAGS = -W -Wall -g

tetra: parser.gen.o lexer.gen.o tetra.o
	$(CC) $(CFLAGS) parser.gen.o lexer.gen.o tetra.o -o tetra

tetra.o: tetra.cpp
	$(CC) $(CFLAGS) -c tetra.cpp

parser.gen.o: parser.gen.cpp
	$(CC) $(CFLAGS) -c parser.gen.cpp

parser.gen.cpp: parser.y
	bison -d parser.y -o parser.gen.cpp

lexer.gen.o: lexer.gen.cpp
	$(CC) $(CFLAGS) -c lexer.gen.cpp

lexer.gen.cpp: lexer.l
	flex -o lexer.gen.cpp lexer.l

.PHONY: clean tags

clean:
	rm -f lexer.gen.cpp parser.gen.cpp parser.gen.hpp tetra *.o

tags:
	ctags -R *.cpp *.hpp

