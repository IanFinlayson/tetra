#!/bin/sh

tetra: parser.gen.o lexer.gen.o tetra.o
	g++ parser.gen.o lexer.gen.o tetra.o -o tetra

tetra.o: tetra.cpp
	g++ -c tetra.cpp

parser.gen.o: parser.gen.cpp
	g++ -c parser.gen.cpp

parser.gen.cpp: parser.y
	bison -d parser.y -o parser.gen.cpp

lexer.gen.o: lexer.gen.cpp
	g++ -c lexer.gen.cpp

lexer.gen.cpp: lexer.l
	flex -o lexer.gen.cpp lexer.l

clean:
	rm -f lexer.gen.cpp parser.gen.cpp parser.gen.hpp tetra


