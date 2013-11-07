CC=gcc
CFLAGS=-c

parser: lex.yy.c parser.tab.o node.o
	$(CC) -Wall lex.yy.c parser.tab.o node.o -o parser

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) parser.tab.c

parser.tab.c: parser.y
	bison -d parser.y

node.o: node.c
	gcc -c node.c

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o
