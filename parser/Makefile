CC=g++
CFLAGS=-c -g -Wall -W

tetra: lex.yy.o node.o exit_funcs.o tetra.o parsetree.o string_buffer.o symbol_table.o hash_table.o dlinked_list.o
	$(CC) -Wall *.o -o tetra

tetra.o: tetra.cpp
	$(CC) $(CFLAGS) tetra.cpp

parsetree.o: parsetree.cpp
	$(CC) $(CFLAGS) parsetree.cpp

exit_funcs.o: exit_funcs.cpp
	$(CC) $(CFLAGS) exit_funcs.cpp

symbol_table.o: symbol_table.cpp
	$(CC) $(CFLAGS) symbol_table.cpp

lex.yy.o: lex.yy.c parser.tab.o
	$(CC) $(CFLAGS) lex.yy.c

lex.yy.c: lexer.l
	flex lexer.l

parser.tab.o: parser.tab.c
	$(CC) $(CFLAGS) parser.tab.c

parser.tab.c: parser.y
	bison -d parser.y

node.o: node.cpp
	${CC} ${CFLAGS} node.cpp

clean:
	rm -f parser lex.yy.c parser.tab.h parser.tab.c *.o tetra
