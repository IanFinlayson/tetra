CC=gcc
CFLAGS=-c -Wall
LEX=flex

all: lexer

lexer: lex.yy.o 
    $(CC) lex.yy.o -o lexer

lex.yy.o: lex.yy.c
    $(CC) $(CFLAGS) lex.yy.c

lex.yy.c: lexer.l
    $(LEX) lexer.l
