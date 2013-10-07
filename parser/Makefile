CC=gcc
CFLAGS=-c

parser: lex.yy.c parser.tab.o
	$(CC) -Wall lex.yy.c parser.tab.o -o parser

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) parser.tab.c

parser.tab.c: parser.y
	bison -d parser.y

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o
