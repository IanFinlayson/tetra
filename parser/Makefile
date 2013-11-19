CC=gcc
CFLAGS=-c -Wall

tetra: lex.yy.o node.o exit_funcs.o tetra.o parsetree.o string_buffer.o
	$(CC) -Wall *.o -o tetra

tetra.o: tetra.c
	$(CC) $(CFLAGS) tetra.c

parsetree.o: parsetree.c
	$(CC) $(CFLAGS) parsetree.c

string_buffer.o: string_buffer.c
	$(CC) $(CFLAGS) string_buffer.c
    
exit_funcs.o: exit_funcs.c
	$(CC) $(CFLAGS) exit_funcs.c

lex.yy.o: lex.yy.c parser.tab.o
	$(CC) $(CFLAGS) lex.yy.c

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) parser.tab.c

parser.tab.c: parser.y
	bison -d parser.y

node.o: node.c
	${CC} ${CFLAGS} node.c

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o
