
parser: lex.yy.c parser.tab.c
	gcc lex.yy.c parser.tab.c -o parser

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.h: parser.tab.c

parser.tab.c: parser.y node.o
	bison -d parser.y

node.o: node.c
	gcc -c node.c

clean:
	rm -f parser *.o lex.yy.c parser.tab.h parser.tab.c
