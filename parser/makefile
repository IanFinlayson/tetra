# makefile for the Tetra parser

CC = clang++
CFLAGS = -W -Wall -g

parse-test: parser.gen.o lexer.o parse-test.o tetra.o
	$(CC) $(CFLAGS) parser.gen.o lexer.o parse-test.o tetra.o -o parse-test

parse-test.o: parse-test.cpp
	$(CC) $(CFLAGS) -c parse-test.cpp

tetra.o: tetra.cpp
	$(CC) $(CFLAGS) -c tetra.cpp

parser.gen.o: parser.gen.cpp
	$(CC) $(CFLAGS) -c parser.gen.cpp

parser.gen.cpp: parser.y
	bison -d parser.y -o parser.gen.cpp

lexer.o: lexer.cpp
	$(CC) $(CFLAGS) -c lexer.cpp

.PHONY: clean tags

clean:
	rm -f parser.gen.cpp parser.gen.hpp parse-test *.o

tags:
	ctags -R *.cpp *.hpp

